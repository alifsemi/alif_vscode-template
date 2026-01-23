/* Copyright (C) 2023 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include "RTE_Device.h"
#include "RTE_Components.h"

#include "fault_handler.h"

#include CMSIS_device_header

#define STACK_DUMP_MAX_LINES 20

enum FaultType {
    FT_Undefined = 0,
    FT_PrefetchAbort = 1,
    FT_DataAbort = 2
};

static const char *const FaultNames[] = {
    [FT_Undefined]  = "Undefined Instruction",
    [FT_PrefetchAbort] = "Prefetch Abort",
    [FT_DataAbort] = "Data Abort"
};

static const char flag_names[] = "NZCVQ000000LGGGG000000EAIFT";

__attribute__((used))
static enum FaultType fault_type;
static uint32_t regs[17];
static void FaultDump(void);

__attribute__((used))
static bool fault_dump_enabled;
__attribute__((used))
static bool fault_handler_active;

bool in_fault_handler(void)
{
    return fault_handler_active;
}

/* External activation function serves to ensure this file is pulled in, and the weak default handlers are overridden */
void fault_dump_enable(bool enable)
{
    fault_dump_enabled = enable;

    CPSR_Type cpsr;
    cpsr.w = __get_CPSR();
    if (cpsr.b.M != CPSR_M_USR && cpsr.b.M != CPSR_M_SYS) {
        printf("!!! In exception mode %d !!!\n", cpsr.b.M);
    }
}

__attribute__((naked))
void Undef_Handler(void)
{
    __asm("PUSH {LR}; MOV LR, #0; B CommonAsmFaultHandler");
}

__attribute__((naked))
void PAbt_Handler(void)
{
    __asm("PUSH {LR}; MOV LR, #1; B CommonAsmFaultHandler");
}

__attribute__((naked))
void DAbt_Handler(void)
{
    __asm("PUSH {LR}; MOV LR, #2; B CommonAsmFaultHandler");
}

__attribute__((used))
__attribute__((naked))
static void CommonAsmFaultHandler(void /* int type */)
{
    __asm("PUSH  {LR}\n\t"                   // Stack now contains exception type and return address
          "LDR   LR, =regs\n\t"
          "STM   LR, {R0-R7}\n\t"            // Store unbanked registers R0-R7
          "POP   {R0,R1}\n\t"
          "STR   R1, [LR, #15*4]\n\t"        // Store return address as R15
          "MRS   R2, SPSR\n\t"               // Get SPSR into R2 - we'll need this for final return, so don't corrupt R2
          "STR   R2, [LR, #16*4]\n\t"        // Store saved PSR
          "MRS   R3, CPSR\n\t"
          "MOV   R4, R3\n\t"
          "BFI   R4, R2, #0, #5\n\t"         // Modify mode in our PSR
          "TST   R4, #0xF\n\t"               // Check for USR mode
          "ORREQ R4, R4, #0xF\n\t"           // Convert USR->SYS (same banked regs, but we can get out of SYS mode, unlike USR mode)
          "MSR   CPSR_c, R4\n\t"             // Switch to aborting mode
          "LDR   R7, =regs+8*4\n\t"
          "STM   R7, {R8-R14}\n\t"           // Store banked registers R8-R14
          "MSR   CPSR_c, R3\n\t"             // Switch back to our handler mode
          "LDR   R7, =fault_dump_enabled\n\t"
          "LDRB  R5, [R7]\n\t"               // If fault dump not enabled, stop now
          "CMP   R5, #0\n\t"
          "BEQ   stop\n\t"
          "MOV   R5, #0\n\t"                 // Disable fault dump, to prevent infinite loop
          "STRB  R5, [R7]\n\t"
          "LDR   R7, =fault_handler_active\n\t"
          "MOV   R5, #1\n\t"                 // Set fault handler active to allow possible ongoing prints to finish and our FaultDump to print
          "STRB  R5, [R7]\n\t"
          "LDR   R7, =fault_type\n\t"        // Store fault type
          "STRB  R0, [R7]\n\t"
          "LDR   LR, =FaultDump\n\t"         // Prepare to "return" to FaultDump
          "BFI   R2, LR, #5, #1\n\t"         // transfer Thumb indicator into SPSR
          "ORR   R2, R2, #0x17\n\t"          // Force Abort mode
          "BIC   R2, R2, #0xC0\n\t"          // clear IRQ and FIQ masks
          "BIC   R2, R2, #3 << 25\n\t"       // clear IT bits
          "BIC   R2, R2, #0x3F << 10\n\t"
          "MSR   SPSR, R2\n\t"
          "MOVS  PC, LR\n\t"                 // return from exception, but to FaultDump - hopefully system will be in a state it can print
    "stop: WFE\n\t"
          "B     stop");
}

static const char mode_names[] = {
    "USR\0"
    "FIQ\0"
    "IRQ\0"
    "SVC\0"
    "20?\0"
    "21?\0"
    "MON\0"
    "ABT\0"
    "24?\0"
    "25?\0"
    "HYP\0"
    "UND\0"
    "28?\0"
    "29?\0"
    "30?\0"
    "SYS"
};

static const char * const fault_status_text[32] = {
    [0x01] = "Alignment fault",
    [0x02] = "Debug exception",
    [0x03] = "Access flag fault, level 1",
    [0x04] = "Fault on instruction cache maintenance",
    [0x05] = "Translation fault, level 1",
    [0x06] = "Access flag fault, level 2",
    [0x07] = "Translation fault, level 2",
    [0x08] = "Synchronous External abort",
    [0x09] = "Domain fault, level 1",
    [0x0B] = "Domain fault, level 2",
    [0x0C] = "Synchronous External abort, on translation table walk, level 1",
    [0x0D] = "Permission fault, level 1",
    [0x0E] = "Synchronous External abort, on translation table walk, level 2",
    [0x0F] = "Permission fault, level 2",
    [0x10] = "TLB conflict abort",
    [0x14] = "Lockdown fault",
    [0x15] = "Unsupported Exclusive access fault",
    [0x16] = "SError interrupt",
    [0x18] = "SError interrupt, parity or ECC error on memory access",
    [0x19] = "Synchronous parity or ECC error on memory access",
    [0x1C] = "Synchronous parity or ECC error on translation table walk, level 1",
    [0x1E] = "Synchronous parity or ECC error on translation table walk, level 2",
};

__STATIC_FORCEINLINE uint32_t __get_TTBCR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 2, 0, 2);
  return result;
}

static const char *fault_status(uint32_t fsr)
{
    /* We only decode the ARMv7 format of FSR registers, which are used
     * when TTBCR.EAE=0. If we update the startup/MMU code to use ARMv8
     * mode (TTBCR.EAE=1), new decode is needed.
     */
    if (__get_TTBCR() & 0x80000000) {
        return "? (TTBCR.EAE = 1)";
    }
    unsigned fs = ((fsr & 0x400) >> 6) | (fsr & 0xF);
    const char *desc = fault_status_text[fs];
    return desc ? desc : "?";
}

__STATIC_FORCEINLINE uint32_t __get_DFAR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 6, 0, 0);
  return result;
}

__STATIC_FORCEINLINE uint32_t __get_IFAR(void)
{
  uint32_t result;
  __get_CP(15, 0, result, 6, 0, 2);
  return result;
}

__attribute__((used))
static void FaultDump(void)
{
    printf("\n==== %s exception ====\n\n", FaultNames[fault_type]);

    switch (fault_type) {
    case FT_DataAbort:
    {
        uint32_t dfsr = __get_DFSR();
        printf("DFSR = %08" PRIX32 " (%s)\n", dfsr, fault_status(dfsr));
        printf("DFAR = %08" PRIX32 "\n\n", __get_DFAR());
        break;
    }
    case FT_PrefetchAbort:
    {
        uint32_t ifsr = __get_IFSR();
        printf("IFSR = %08" PRIX32 " (%s)\n", ifsr, fault_status(ifsr));
        printf("IFAR = %08" PRIX32 "\n\n", __get_IFAR());
        break;
    }
    case FT_Undefined:
        break;
    }

    printf("Register dump (stored at &%08" PRIXPTR ") is:\n", (uintptr_t) regs);
    for (int i = 0; i < 16; i++) {
        printf("R%-3d= %08" PRIX32 "%c", i, regs[i], i % 4 < 3 ? ' ' : '\n');
    }
    printf("Mode %s flags set: ", &mode_names[(regs[16] & 0xF) * 4]);
    for (size_t i = 0, bit = 1u<<31; i < 38; i++) {
        if (i < sizeof flag_names - 1) {
            if (flag_names[i] != '0') {
                putchar(regs[16] & bit ? flag_names[i] : flag_names[i] + 'a' - 'A');
            }
            bit >>= 1;
        } else {
            putchar(' ');
        }
    }
    printf("PSR = %08" PRIX32 "\n", regs[16]);

    // Potential TODO: work out stack top
    uintptr_t stack_top = UINTPTR_MAX;

    printf("\n==== Stack dump ====\n\n");
    const uintptr_t stack_point = regs[13];

    // not using the default stack so we have to just
    // print the defined maximum (STACK_DUMP_MAX_LINES)
    if (stack_top < stack_point) {
        stack_top = UINTPTR_MAX;
    }

    // these are for readability(?), can't be changed without modifying code below
    #define VALUES_PER_LINE 4
    #define BYTES_IN_VALUE 4

    // start printing from aligned address
    const uintptr_t loop_start = stack_point - stack_point % (VALUES_PER_LINE * BYTES_IN_VALUE);

    // Dump uint32 values from SP until stack top or until defined number of lines is printed
    printf("Address  :     3 2 1 0     7 6 5 4     B A 9 8     F E D C       ASCII Data\n");
    //      80008010 :    FFEEAABB    CC001133    12345678    1A2B3C4D    ....3...xV".M<+.
    for (uint32_t *p = (uint32_t *)loop_start; p < ((uint32_t *)loop_start + STACK_DUMP_MAX_LINES * VALUES_PER_LINE) && p < (uint32_t *)stack_top; p += VALUES_PER_LINE) {

        printf("%08" PRIXPTR " :", (uintptr_t)p);
        // print the stack values for 1 line
        for (uint32_t *vp = p; vp < p + VALUES_PER_LINE; vp++) {
            if (vp >= (uint32_t*)stack_point && vp < (uint32_t*)stack_top) {
                printf("    %08" PRIX32, *vp);
            }
            else {
                printf("            ");
            }
        }
        printf("    ");

        // print the ascii characters for one line
        for (char *cp = (char *)p; cp < (char *)p + VALUES_PER_LINE * BYTES_IN_VALUE; cp++) {
            if (cp >= (char*)stack_point && cp < (char*)stack_top) {
                // only print printable ascii characters
                if(*cp > 31 && *cp < 127) {
                    putchar(*cp);
                }
                else {
                    putchar('.');
                }
            }
            else {
                putchar(' ');
            }
        }
        printf("\n");
    }

    for (;;) {
        __WFE();
    }
}
