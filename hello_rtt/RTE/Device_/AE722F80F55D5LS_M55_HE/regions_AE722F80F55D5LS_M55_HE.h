#ifndef REGIONS_AE722F80F55D5LS_M55_HE_H
#define REGIONS_AE722F80F55D5LS_M55_HE_H


//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------
//------ With VS Code: Open Preview for Configuration Wizard -------------------

// <n> Auto-generated using information from packs
// <i> Device Family Pack (DFP):   AlifSemiconductor::Ensemble@2.0.0-alpha

// <h> ROM Configuration
// =======================
// <h> __ROM0 (is rx memory: MRAM from DFP)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x80000000
//   <i> Contains Startup and Vector Table
#define __ROM0_BASE 0x80000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00580000
#define __ROM0_SIZE 0x00580000
// </h>

// <h> __ROM1 (unused)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
#define __ROM1_BASE 0
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
#define __ROM1_SIZE 0
// </h>

// <h> __ROM2 (unused)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
#define __ROM2_BASE 0
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
#define __ROM2_SIZE 0
// </h>

// <h> __ROM3 (unused)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
#define __ROM3_BASE 0
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
#define __ROM3_SIZE 0
// </h>

// </h>

// <h> RAM Configuration
// =======================
// <h> __RAM0 (is rwx memory: SRAM0 from DFP)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x02000000
//   <i> Contains uninitialized RAM, Stack, and Heap
#define __RAM0_BASE 0x02000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00400000
#define __RAM0_SIZE 0x00400000
// </h>

// <h> __RAM1 (is rwx memory: SRAM1 from DFP)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x08000000
#define __RAM1_BASE 0x08000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00280000
#define __RAM1_SIZE 0x00280000
// </h>

// <h> __RAM2 (is rwx memory: SRAM2 from DFP)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x50000000
#define __RAM2_BASE 0x50000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00040000
#define __RAM2_SIZE 0x00040000
// </h>

// <h> __RAM3 (is rwx memory: SRAM3 from DFP)
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region. Default: 0x50800000
#define __RAM3_BASE 0x50800000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region. Default: 0x00100000
#define __RAM3_SIZE 0x00100000
// </h>

// </h>

// <h> Stack / Heap Configuration
//   <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
//   <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __STACK_SIZE 0x00000200
#define __HEAP_SIZE 0x00000C00
// </h>

// <n> Resources that are not allocated to linker regions
// <i> rwx RAM:  SRAM4 from DFP:    BASE: 0x58000000  SIZE: 0x00040000
// <i> rwx RAM:  SRAM5 from DFP:    BASE: 0x58800000  SIZE: 0x00040000
// <i> rwx RAM:  SRAM6_A from DFP:  BASE: 0x62000000  SIZE: 0x00100000
// <i> rwx RAM:  SRAM6_B from DFP:  BASE: 0x62400000  SIZE: 0x00100000
// <i> rwx RAM:  SRAM7 from DFP:    BASE: 0x63000000  SIZE: 0x00080000
// <i> rwx RAM:  SRAM8 from DFP:    BASE: 0x63200000  SIZE: 0x00200000
// <i> rwx RAM:  SRAM9 from DFP:    BASE: 0x60000000  SIZE: 0x000C0000


#endif /* REGIONS_AE722F80F55D5LS_M55_HE_H */
