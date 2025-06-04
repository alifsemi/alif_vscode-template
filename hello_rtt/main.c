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

#include "board_config.h"
#include "fault_handler.h"


int main (void)
{
    // Initialize pinmuxes
    int32_t ret = board_pins_config();
    if (ret != 0) {
        while(1);
    }

    fault_dump_enable(true);

    printf("\r\nHello World!\r\n");

    while (1) __WFE();
}
