/* Copyright (C) 2024 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */
#include <time.h>

#include "RTE_Components.h"
#include CMSIS_device_header

#include <stdio.h>

#include "board.h"
#include "fault_handler.h"


int main (void)
{
    // Init pinmux using boardlib
    BOARD_Pinmux_Init();

    fault_dump_enable(true);

    BOARD_LED1_Control(BOARD_LED_STATE_HIGH);

    printf("\r\nHello World!\r\n");

    while (1) __WFE();
}
