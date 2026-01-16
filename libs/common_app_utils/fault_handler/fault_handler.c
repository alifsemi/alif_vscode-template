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

// Suppress bogus warning:  Warning in inline assembly: "SP as destination register is unpredictable"
#if __ICCARM__
#pragma diag_suppress=Og014
#endif

#define MMFSR (((volatile uint8_t *) &SCB->CFSR)[0])
#define BFSR (((volatile uint8_t *) &SCB->CFSR)[1])
#define UFSR (((volatile uint16_t *) &SCB->CFSR)[1])

#define VTOR_STACK_TOP (((volatile uint32_t*)SCB->VTOR)[0])
#define xstr(s) str(s)
#define str(s) #s

// Enable placing fault handler related data to separate sections since they
// are not performance critical
#define FAULT_HANDLER_RW_MEMORY_LOCATION __attribute__((section(".bss.slow")))
#define FAULT_HANDLER_RO_MEMORY_LOCATION __attribute__((section(".rodata.slow")))
#define FAULT_HANDLER_XO_MEMORY_LOCATION __attribute__((section(".text.slow")))

#define STACK_DUMP_MAX_LINES 20

// Size of the fault stack in bytes
#define FAULT_STACK_SIZE 1024


#define FAULT_STACK_WORDS (FAULT_STACK_SIZE / 4)
static uint32_t fault_stack[FAULT_STACK_WORDS] FAULT_HANDLER_RW_MEMORY_LOCATION __ALIGNED(8);

// These values are written to R0 by magic number in dedicated fault handler before jumping
// to common one so they need to be in sync.
enum FaultType {
    FT_HardFault = 0,
    FT_MemManage = 1,
    FT_BusFault = 2,
    FT_UsageFault = 3,
    FT_SecureFault = 4,
    FT_DebugMonitor = 5
};

static const char *const FaultNames[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    [FT_HardFault]  = "HardFault",
    [FT_MemManage]  = "MemManage",
    [FT_BusFault]   = "BusFault",
    [FT_UsageFault] = "UsageFault",
    [FT_SecureFault] = "SecureFault",
    [FT_DebugMonitor] = "DebugMonitor"
};

static const char flag_names[] FAULT_HANDLER_RO_MEMORY_LOCATION = "NZCVQIIT00B0GGGG";

static enum FaultType fault_type FAULT_HANDLER_RW_MEMORY_LOCATION;
static uint32_t exc_return FAULT_HANDLER_RW_MEMORY_LOCATION;
static uint32_t regs[17] FAULT_HANDLER_RW_MEMORY_LOCATION;
static bool fault_dump_enabled;
static bool fault_handler_active;

static void FaultDump(void);

/* Compressed encoding of bits - bit number, then name, ending with space (32) */
/* Octal bit number encoding, because C - it's either that or hex */
static const char mmfsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\007MMARVALID"
    "\005MLSPERR"
    "\004MSTKERR"
    "\003MUNSTKERR"
    "\001DACCVIOL"
    "\000IACCVIOL"
    " "
};

static const char bfsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\007BFARVALID"
    "\005LSPERR"
    "\004STKERR"
    "\003UNSTKERR"
    "\002IMPRECISERR"
    "\001PRECISERR"
    "\000IBUSERR"
    " "
};

static const char ufsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\011DIVBYZERO"
    "\010UNALIGNED"
    "\004STKOF"
    "\003NOCP"
    "\002INVPC"
    "\001INVSTATE"
    "\000UNDEFINSTR"
    " "
};

static const char hfsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\037DEBUGEVT"
    "\036FORCED"
    "\001VECTBL"
    " "
};

static const char sfsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\007LSERR"
    "\006SFARVALID"
    "\005LSPERR"
    "\004INVTRAN"
    "\003AUVIOL"
    "\002INVER"
    "\001INVIS"
    "\000INVEP"
    " "
};

static const char dfsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\005PMU"
    "\004EXTERNAL"
    "\003VCATCH"
    "\002DWTTRAP"
    "\001BKPT"
    "\00HALTED"
    " "
};

#if __CORTEX_M == 55
static const char afsr_bits[] FAULT_HANDLER_RO_MEMORY_LOCATION = {
    "\036FPOISON"
    "\035FTGU"
    "\034FECC"
    "\030FMAXI"
    "\033FMAXITYPE=DECERR"
    "\026FDTCM"
    "\025FITCM"
    "\023PPOISON"
    "\022PTGU"
    "\021PECC"
    "\017PIPPB"
    "\016PEPPB"
    "\015PMAXI"
    "\020PMAXITYPE=DECERR"
    "\014PPAHB"
    "\013PDTCM"
    "\012PITCM"
    "\011IPOISON"
    "\007IECC"
    "\004IEPPB"
    "\003IMAXI"
    "\006IMAXITYPE=DECERR"
    "\002IPAHB"
    "\001IDTCM"
    "\000IITCM"
    " "
};
#endif

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_fsrbits(uint32_t val, const char *bitnames)
{
     bool first = true;

     while (val != 0 && *bitnames != ' ') {
         int bit = *bitnames++;
         const char *next_bitname = bitnames;
         while (*next_bitname > ' ') {
             next_bitname++;
         }
         if (val & (1U << bit)) {
             val &=~ (1U << bit);
             printf("%s%.*s", first ? " (" : ", ", next_bitname - bitnames, bitnames);
             first = false;
         }
         bitnames = next_bitname;
     } ;

     puts(first ? "" : ")");
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_memmanage(void)
{
     uint32_t mmfsr = MMFSR;
     if (mmfsr == 0) {
         return;
     }
     printf("MMFSR = %02" PRIX32, mmfsr);
     print_fsrbits(mmfsr, mmfsr_bits);
     if (mmfsr & 0x80) {
        printf("MMFAR = %08" PRIX32 "\n", SCB->MMFAR);
     }
     MMFSR = mmfsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_busfault(void)
{
     uint32_t bfsr = BFSR;
     if (bfsr == 0) {
         return;
     }
     printf("BFSR  = %02" PRIX32, bfsr);
     print_fsrbits(bfsr, bfsr_bits);
     if (bfsr & 0x80) {
        printf("BFAR  = %08" PRIX32 "\n", SCB->BFAR);
     }
#if __CORTEX_M == 55
     uint32_t afsr = SCB->AFSR;
     printf("AFSR  = %08" PRIX32, afsr);

     print_fsrbits(afsr, afsr_bits);
     SCB->AFSR = afsr;
#endif
     BFSR = bfsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_usagefault(void)
{
     uint32_t ufsr = UFSR;
     if (ufsr == 0) {
         return;
     }
     printf("UFSR  = %04" PRIX32, ufsr);
     print_fsrbits(ufsr, ufsr_bits);
     UFSR = ufsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_hardfault(void)
{
     uint32_t hfsr = SCB->HFSR;
     if (hfsr == 0) {
         return;
     }
     printf("HFSR  = %08" PRIX32, hfsr);
     print_fsrbits(hfsr, hfsr_bits);
     SCB->HFSR = hfsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_securefault(void)
{
     uint32_t sfsr = SCB->SFSR;
     if (sfsr == 0) {
         return;
     }
     printf("SFSR  = %08" PRIX32, sfsr);
     print_fsrbits(sfsr, sfsr_bits);
     SCB->SFSR = sfsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_debugfault(void)
{
     uint32_t dfsr = SCB->DFSR;
     if (dfsr == 0) {
         return;
     }
     printf("DFSR  = %08" PRIX32, dfsr);
     print_fsrbits(dfsr, dfsr_bits);
     SCB->DFSR = dfsr;
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void print_faults(void)
{
    print_usagefault();
    print_memmanage();
    print_busfault();
    print_securefault();
    print_debugfault();
    print_hardfault();
}

bool in_fault_handler(void)
{
    return fault_handler_active;
}

/* External activation function serves to ensure this file is pulled in, and the weak default handlers are overridden */
void fault_dump_enable(bool enable)
{
    fault_dump_enabled = enable;
    fault_handler_active = false;

    /* Show any pending faults (shouldn't be any) and clear registers */
    print_faults();

    IPSR_Type ipsr;
    ipsr.w = __get_IPSR();
    if (ipsr.b.ISR != 0) {
        printf("!!! In exception %d !!!\n", ipsr.b.ISR);
    }
}

__attribute__((naked))
void HardFault_Handler(void)
{
    __asm("MOVS R0, #0\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((naked))
void MemManage_Handler(void)
{
    __asm("MOVS R0, #1\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((naked))
void BusFault_Handler(void)
{
    __asm("MOVS R0, #2\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((naked))
void UsageFault_Handler(void)
{
    __asm("MOVS R0, #3\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((naked))
void SecureFault_Handler(void)
{
    __asm("MOVS R0, #4\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((naked))
void DebugMonitor_Handler(void)
{
    __asm("MOVS R0, #5\n\t"
          "B CommonAsmFaultHandler\n\t");
}

__attribute__((used))
__attribute__((naked))
FAULT_HANDLER_XO_MEMORY_LOCATION
static void CommonAsmFaultHandler(void /* int type */)
{
    // FaultType is set to R0 by specific handler functions before jumping here
    // This prepares the remaining arguments for CommonFaultHandler in R1-R3
    __asm("LDR   R1, =regs+4*4\n\t"
          "STM   R1, {R4-R11}\n\t"  // store R4-R11
          "TST   LR, #1<<2\n\t"     // check which stack was in used when fault happened
          "ITTEE EQ\n\t"
          "MOVEQ R1, SP\n\t"        // MSP was used, store pointer to R1 and limit to R3
          "MRSEQ R3, MSPLIM\n\t"
          "MRSNE R1, PSP\n\t"       // PSP was used, store pointer to R1 and limit to R3
          "MRSNE R3, PSPLIM\n\t"
          "MOV   R2, LR\n\t"        // store LR to R2
          "LDR   SP, =fault_stack+4*" xstr(FAULT_STACK_SIZE) "-32\n\t" // switch to dedicated fault dump stack
          "LDR   R4, =fault_stack\n\t"
          "MSR   MSPLIM, R4\n\t"    // set proper stack limit for the dedicated fault dump stack
          "B     CommonFaultHandler");
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void JumpToDump(uint32_t exc_return, uint32_t* sp)
{
    __asm("MOV   SP, %0\n\t" // sp parameter to SP
          "BX    %1" :: "r"(sp), "r"(exc_return));
}

__attribute__((used))
FAULT_HANDLER_XO_MEMORY_LOCATION
static void CommonFaultHandler(enum FaultType type, uint32_t * restrict sp, uint32_t lr, uint32_t* orig_splim)
{
    if (!fault_dump_enabled) {
        while (1) {
            __WFE();
        }
    }

    // Prevent infinite loop in case fault dumping generates a new fault
    fault_dump_enabled = false;

    // Mark fault handler active for retarget printing logic
    fault_handler_active = true;

    exc_return = lr;
    fault_type = type;
    uint32_t retpsr = sp[7];

    // Assembler fills in R4-R11, we do the rest
    regs[0] = sp[0];
    regs[1] = sp[1];
    regs[2] = sp[2];
    regs[3] = sp[3];
    regs[12] = sp[4];

    // For SP, we need to see if we pushed an integer-only or FP context, and also check for doubleword alignment adjustment
    if((UFSR & 0x10) && sp == orig_splim) {
        regs[13] = (uint32_t)sp;
        retpsr = 0;  // can't trust that stack frame contains sane values as this is stack overflow fault
    }
    else {
        regs[13] = (uint32_t)sp + (exc_return & 0x10 ? 0x20 : 0x68) + (retpsr & 0x200 ? 4 : 0);
    }
    regs[14] = sp[5];
    regs[15] = sp[6];
    regs[16] = sp[7];

    // initialize stack values for 'return' to FaultDump
    fault_stack[FAULT_STACK_WORDS-1] = (1 << 24) | (retpsr & 0x1FF); // RETPSR, only set T32 state and exception state
    fault_stack[FAULT_STACK_WORDS-2] = (uint32_t) FaultDump;  // Return Address, this is where EXC_RETURN will return to later on
    fault_stack[FAULT_STACK_WORDS-3] = 0xEFFFFFFE;  // LR, deliberately set to 'illegal' execution address as we're not expecting FaultDump to return
    fault_stack[FAULT_STACK_WORDS-4] = 0;  // R12
    fault_stack[FAULT_STACK_WORDS-5] = 0;  // R3
    fault_stack[FAULT_STACK_WORDS-6] = 0;  // R2
    fault_stack[FAULT_STACK_WORDS-7] = 0;  // R1
    fault_stack[FAULT_STACK_WORDS-8] = 0;  // R0

    // 'return' to FaultDump by setting SP so stack only contains standard stack frame
    // which was filled above and executing EXC_RETURN. EXC_RETURN is explicitly set
    // to standard stack frame type instead of possible extended stack frame in the original
    // faulting point to guarantee the above stack fill is valid.
    uint32_t new_exc_return = exc_return | 0x10;
    uint32_t* new_sp = fault_stack + FAULT_STACK_WORDS - 8;

    JumpToDump(new_exc_return, new_sp);
}

FAULT_HANDLER_XO_MEMORY_LOCATION
static void FaultDump(void)
{
    printf("\n==== %s exception ====\n\n", FaultNames[fault_type]);

    print_faults();

    printf("\nEXC_RETURN = %08" PRIX32 "\n\n"
           "Register dump (stored at &%08" PRIXPTR ") is:\n", exc_return, (uintptr_t) regs);
    for (int i = 0; i < 13; i++) {
        printf("R%-3d= %08" PRIX32 "%c", i, regs[i], i % 4 < 3 ? ' ' : '\n');
    }
    printf("SP  = %08" PRIX32 " LR  = %08" PRIX32 " PC  = %08" PRIX32 "\n", regs[13], regs[14], regs[15]);
    printf("Mode %-8sflags set: ", exc_return & 8 ? "Thread" : "Handler");
    for (size_t i = 0, bit = 1u<<31; i < 24; i++) {
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
    if (!(exc_return & 8)) {
        printf("Exception %" PRId32 "\n", regs[16] & 0x1FF);
    }

    uintptr_t stack_top = VTOR_STACK_TOP;
    printf("Stack top from VTOR: %08" PRIXPTR "\n", stack_top);
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
