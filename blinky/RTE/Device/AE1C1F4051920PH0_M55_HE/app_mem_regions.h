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
//   <o> RTSS HE Base address <0x80000000-0x8018C000:8>
//   <i> Defines base address of RTSS HE application memory region.
//   <i> Default: 0x80000000
#define APP_MRAM_HE_BASE       0x80000000
//   <o> RTSS HE Region size [bytes] <0x0-0x0018C000:8>
//   <i> Defines size of RTSS HE application memory region.
//   <i> Default: 0x0018C000
#define APP_MRAM_HE_SIZE       0x0018C000

// </h>

// <h>RAM Configuration
// =======================
// <h> SRAM4
//   <o> Base address <0x58000000-0x58080000:8>
//   <i> Defines base address of SRAM4 memory region.
//   <i> Default: 0x58000000
#define APP_SRAM4_BASE         0x58000000

//   <o> Region size [bytes] <0x0-0x00080000:8>
//   <i> Defines size of SRAM4 memory region.
//   <i> Default: 0x00080000
#define APP_SRAM4_SIZE         0x00080000

//   <q>No zero initialize
//   <i> Excludes SRAM4 region from zero initialization.
#define APP_SRAM4_NOINIT       0
// </h>

// <h> SRAM5
//   <o> Base address <0x58800000-0x58980000:8>
//   <i> Defines base address of SRAM5 memory region.
//   <i> Default: 0x58800000
#define APP_SRAM5_BASE         0x58800000

//   <o> Region size [bytes] <0x0-0x00180000:8>
//   <i> Defines size of SRAM5 memory region.
//   <i> Default: 0x00180000
#define APP_SRAM5_SIZE         0x00180000

//   <q>No zero initialize
//   <i> Excludes SRAM5 region from zero initialization.
#define APP_SRAM5_NOINIT       0
// </h>

// </h>

// <h>Stack / Heap Configuration

// <h>RTSS HE
//   <o0> Stack Size (in Bytes) <0x0-0x400000:8>
//   <o1> Heap Size (in Bytes) <0x0-0x400000:8>
#define APP_HE_STACK_SIZE      0x00002000
#define APP_HE_HEAP_SIZE       0x00004000
// </h>

// </h>

// <h>ITCM & DTCM Base Address
//   <o0> ITCM Base address <0x0-0x400000:8>
//   <o1> DTCM Base address <0x20000000-0x400000:8>
#define APP_ITCM_BASE          0x00000000
#define APP_DTCM_BASE          0x20000000
// </h>

// <h>OSPI XIP Configuration
// =======================
//   <q> Boot from OSPI
//   <i> Enable XIP from OSPI
#define APP_BOOT_OSPI_FLASH    0
//   <o> RTSS HE Base address <0xB0000000-0xBFFFFFFF:8>
//   <i> Defines base address of RTSS HE application memory region.
//   <i> Default: 0xB0000000
#define APP_OSPI_FLASH_HE_BASE 0xB0000000
//   <o> RTSS HE Region size [bytes] <0x0-0x10000000:8>
//   <i> Defines size of RTSS HE application memory region.
//   <i> Default: 0x00200000
#define APP_OSPI_FLASH_HE_SIZE 0x00200000

// </h>

#define APP_HE_ITCM_SIZE       APP_SRAM4_SIZE
#define APP_HE_DTCM_SIZE       APP_SRAM5_SIZE

#define TGU_BLOCK_SIZE         131072

#define __HAS_BULK_SRAM        0

#endif /* APP_MEM_REGIONS_H */
