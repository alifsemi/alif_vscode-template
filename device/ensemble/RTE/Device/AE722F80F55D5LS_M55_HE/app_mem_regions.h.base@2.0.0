/* Copyright (C) 2025 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

#ifndef APP_MEM_REGIONS_H
#define APP_MEM_REGIONS_H

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>MRAM XIP Configuration
// =======================
//   <o> RTSS HE Base address <0x80000000-0x8057FFFF:8>
//   <i> Defines base address of RTSS HE application memory region.
//   <i> Default: 0x80000000
#define APP_MRAM_HE_BASE 0x80000000
//   <o> RTSS HE Region size [bytes] <0x0-0x00580000:8>
//   <i> Defines size of RTSS HE application memory region.
//   <i> Default: 0x00200000
#define APP_MRAM_HE_SIZE 0x00200000
//   <o> RTSS HP Base address <0x80000000-0x8057FFFF:8>
//   <i> Defines base address of RTSS HP application memory region.
//   <i> Default: 0x80000000
#define APP_MRAM_HP_BASE 0x80200000
//   <o> RTSS HP Region size [bytes] <0x0-0x00580000:8>
//   <i> Defines size of RTSS HP application memory region.
//   <i> Default: 0x00200000
#define APP_MRAM_HP_SIZE 0x00200000
// </h>

// <h>RAM Configuration
// =======================
// <h> SRAM0
//   <o> Base address <0x02000000-0x023FFFFF:8>
//   <i> Defines base address of SRAM0 memory region.
//   <i> Default: 0x02000000
#define APP_SRAM0_BASE 0x02000000
//   <o> Region size [bytes] <0x0-0x00400000:8>
//   <i> Defines size of SRAM0 memory region.
//   <i> Default: 0x00400000
#define APP_SRAM0_SIZE 0x00400000
//   <q>No zero initialize
//   <i> Excludes SRAM0 region from zero initialization.
#define APP_SRAM0_NOINIT 1
// </h>

// <h> SRAM1
//   <o> Base address <0x08000000-0x0827FFFF:8>
//   <i> Defines base address of SRAM1 memory region.
//   <i> Default: 0x08000000
#define APP_SRAM1_BASE 0x08000000
//   <o> Region size [bytes] <0x0-0x00280000:8>
//   <i> Defines size of SRAM1 memory region.
//   <i> Default: 0x00280000
#define APP_SRAM1_SIZE 0x00280000
//   <q>No zero initialize
//   <i> Excludes SRAM1 region from zero initialization.
#define APP_SRAM1_NOINIT 1
// </h>

// <h> SRAM2
//   <o> Base address <0x50000000-0x5003FFFF:8>
//   <i> Defines base address of SRAM2 memory region.
//   <i> Default: 0x50000000
#define APP_SRAM2_BASE 0x50000000
//   <o> Region size [bytes] <0x0-0x00040000:8>
//   <i> Defines size of SRAM2 memory region.
//   <i> Default: 0x00040000
#define APP_SRAM2_SIZE 0x00040000
//   <q>No zero initialize
//   <i> Excludes SRAM2 region from zero initialization.
#define APP_SRAM2_NOINIT 0
// </h>

// <h> SRAM3
//   <o> Base address <0x50800000-0x508FFFFF:8>
//   <i> Defines base address of SRAM3 memory region.
//   <i> Default: 0x50800000
#define APP_SRAM3_BASE 0x50800000
//   <o> Region size [bytes] <0x0-0x00100000:8>
//   <i> Defines size of SRAM3 memory region.
//   <i> Default: 0x00100000
#define APP_SRAM3_SIZE 0x00100000
//   <q>No zero initialize
//   <i> Excludes SRAM3 region from zero initialization.
#define APP_SRAM3_NOINIT 0
// </h>

// <h> SRAM4
//   <o> Base address <0x58000000-0x5803FFFF:8>
//   <i> Defines base address of SRAM4 memory region.
//   <i> Default: 0x58000000
#define APP_SRAM4_BASE 0x58000000
//   <o> Region size [bytes] <0x0-0x00040000:8>
//   <i> Defines size of SRAM4 memory region.
//   <i> Default: 0x00040000
#define APP_SRAM4_SIZE 0x00040000
//   <q>No zero initialize
//   <i> Excludes SRAM4 region from zero initialization.
#define APP_SRAM4_NOINIT 0
// </h>

// <h> SRAM5
//   <o> Base address <0x58800000-0x5883FFFF:8>
//   <i> Defines base address of SRAM5 memory region.
//   <i> Default: 0x58800000
#define APP_SRAM5_BASE 0x58800000
//   <o> Region size [bytes] <0x0-0x00040000:8>
//   <i> Defines size of SRAM5 memory region.
//   <i> Default: 0x00040000
#define APP_SRAM5_SIZE 0x00040000
//   <q>No zero initialize
//   <i> Excludes SRAM5 region from zero initialization.
#define APP_SRAM5_NOINIT 0
// </h>

// </h>

// <h>Stack / Heap Configuration

// <h>RTSS HE
//   <o0> Stack Size (in Bytes) <0x0-0x400000:8>
//   <o1> Heap Size (in Bytes) <0x0-0x400000:8>
#define APP_HE_STACK_SIZE 0x00002000
#define APP_HE_HEAP_SIZE 0x00004000
// </h>

// <h>RTSS HP
//   <o0> Stack Size (in Bytes) <0x0-0x400000:8>
//   <o1> Heap Size (in Bytes) <0x0-0x400000:8>
#define APP_HP_STACK_SIZE 0x00002000
#define APP_HP_HEAP_SIZE 0x00004000
// </h>

// </h>

// <h>ITCM & DTCM Base Address
//   <o0> ITCM Base address <0x0-0x400000:8>
//   <o1> DTCM Base address <0x20000000-0x400000:8>
#define APP_ITCM_BASE    0x00000000
#define APP_DTCM_BASE    0x20000000
// </h>

// <h>OSPI XIP Configuration
// =======================
//   <q> Boot from OSPI
//   <i> Enable XIP from OSPI
#define APP_BOOT_OSPI_FLASH 0
//   <o> RTSS HE Base address <0xC0000000-0xDFFFFFFF:8>
//   <i> Defines base address of RTSS HE application memory region.
//   <i> Default: 0xC0000000
#define APP_OSPI_FLASH_HE_BASE 0xC0000000
//   <o> RTSS HE Region size [bytes] <0x0-0x20000000:8>
//   <i> Defines size of RTSS HE application memory region.
//   <i> Default: 0x00200000
#define APP_OSPI_FLASH_HE_SIZE 0x00200000
//   <o> RTSS HP Base address <0xC0000000-0xDFFFFFFF:8>
//   <i> Defines base address of RTSS HP application memory region.
//   <i> Default: 0x80000000
#define APP_OSPI_FLASH_HP_BASE 0xC0200000
//   <o> RTSS HP Region size [bytes] <0x0-0x20000000:8>
//   <i> Defines size of RTSS HP application memory region.
//   <i> Default: 0x00200000
#define APP_OSPI_FLASH_HP_SIZE 0x00200000
// </h>

#define APP_HE_ITCM_SIZE      APP_SRAM4_SIZE
#define APP_HE_DTCM_SIZE      APP_SRAM5_SIZE

#define APP_HP_ITCM_SIZE      APP_SRAM2_SIZE
#define APP_HP_DTCM_SIZE      APP_SRAM3_SIZE

#define TGU_BLOCK_SIZE        16384

#define __HAS_BULK_SRAM     1

#endif /* APP_MEM_REGIONS_H */
