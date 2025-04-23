/* Copyright (C) 2023 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

/******************************************************************************
 * @file     board.h
 * @brief    BOARD API
 *
 *           copy this file to your project and remove #if 0 / #endif
 ******************************************************************************/
#if 1
#ifndef __BOARD_LIB_H
#define __BOARD_LIB_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
// <o> Alif Development Kit variant
//     <0>-<3> Deprecated boards
//     <4=> Alif Development Kit (Generation 2 Silicon Rev B / Board Rev A, B, C)
//     <5=> Alif AI/ML Application Kit (Generation 2 Silicon Rev B / Board Rev A)
//     <6=> Alif Development Kit for E1C Silicon Rev A
#ifndef BOARD_ALIF_DEVKIT_VARIANT
#define BOARD_ALIF_DEVKIT_VARIANT       0
#endif

#if (BOARD_ALIF_DEVKIT_VARIANT >= 0 && BOARD_ALIF_DEVKIT_VARIANT <= 3)
#error "Support for legacy boards has been deprecated"
#elif (BOARD_ALIF_DEVKIT_VARIANT == 4)
#define BOARD_IS_ALIF_DEVKIT_B0_VARIANT
#include "devkit_gen2/board_defs.h"
#elif (BOARD_ALIF_DEVKIT_VARIANT == 5)
#define BOARD_IS_ALIF_APPKIT_B1_VARIANT
#include "appkit_gen2/board_defs.h"
#elif (BOARD_ALIF_DEVKIT_VARIANT == 6)
#define BOARD_IS_ALIF_DEVKIT_E1C_VARIANT
#include "devkit_e1c/board_defs.h"
#else
#error "Unknown board variant"
#endif

// <o> ILI9806E LCD panel variant
//     <0=> E43RB_FW405
//     <1=> E43GB_MW405
//     <2=> E50RA_MW550
// <i> Defines ILI9806E panel variant
// <i> Default: E43RB_FW405
#define BOARD_ILI9806E_PANEL_VARIANT    0

void BOARD_Pinmux_Init();
void BOARD_Clock_Init();
void BOARD_Power_Init();

typedef void (*BOARD_Callback_t) (uint32_t event);

typedef enum {
	BOARD_BUTTON_ENABLE_INTERRUPT = 1,  /**<BUTTON interrupt enable>*/
	BOARD_BUTTON_DISABLE_INTERRUPT,     /**<BUTTON interrupt disable>*/
} BOARD_BUTTON_CONTROL;

typedef enum {
    BOARD_BUTTON_STATE_LOW,             /**<BUTTON state is LOW>*/
    BOARD_BUTTON_STATE_HIGH,            /**<BUTTON state is HIGH>*/
} BOARD_BUTTON_STATE;

typedef enum {
    BOARD_LED_STATE_LOW,                /**<Drive LED output state LOW>*/
    BOARD_LED_STATE_HIGH,               /**<Drive LED output state HIGH>*/
    BOARD_LED_STATE_TOGGLE,             /**<Toggle LED output state>*/
} BOARD_LED_STATE;

void BOARD_BUTTON1_Init(BOARD_Callback_t user_cb);
void BOARD_BUTTON2_Init(BOARD_Callback_t user_cb);
void BOARD_BUTTON1_Control(BOARD_BUTTON_CONTROL control);
void BOARD_BUTTON2_Control(BOARD_BUTTON_CONTROL control);
void BOARD_BUTTON1_GetState(BOARD_BUTTON_STATE *state);
void BOARD_BUTTON2_GetState(BOARD_BUTTON_STATE *state);
void BOARD_LED1_Control(BOARD_LED_STATE state);
void BOARD_LED2_Control(BOARD_LED_STATE state);
#ifdef __cplusplus
}
#endif
#endif
#endif
