#include "RTE_Components.h"
#include CMSIS_device_header

#include "Driver_IO.h"
#include "board_config.h"

#define _GET_DRIVER_REF(ref, peri, chan) \
    extern ARM_DRIVER_##peri Driver_##peri##chan; \
    static ARM_DRIVER_##peri * ref = &Driver_##peri##chan;
#define GET_DRIVER_REF(ref, peri, chan) _GET_DRIVER_REF(ref, peri, chan)

#if defined(BOARD_RGB_LED_INSTANCE) && (BOARD_RGB_LED_INSTANCE == 0)
    GET_DRIVER_REF(gpio_b, GPIO, BOARD_LEDRGB0_B_GPIO_PORT);
    GET_DRIVER_REF(gpio_g, GPIO, BOARD_LEDRGB0_G_GPIO_PORT);
    GET_DRIVER_REF(gpio_r, GPIO, BOARD_LEDRGB0_R_GPIO_PORT);
    #define BOARD_LEDRGB_B_GPIO_PIN BOARD_LEDRGB0_B_GPIO_PIN
    #define BOARD_LEDRGB_G_GPIO_PIN BOARD_LEDRGB0_G_GPIO_PIN
    #define BOARD_LEDRGB_R_GPIO_PIN BOARD_LEDRGB0_R_GPIO_PIN
#else
    GET_DRIVER_REF(gpio_b, GPIO, BOARD_LEDRGB1_B_GPIO_PORT);
    GET_DRIVER_REF(gpio_g, GPIO, BOARD_LEDRGB1_G_GPIO_PORT);
    GET_DRIVER_REF(gpio_r, GPIO, BOARD_LEDRGB1_R_GPIO_PORT);
    #define BOARD_LEDRGB_B_GPIO_PIN BOARD_LEDRGB1_B_GPIO_PIN
    #define BOARD_LEDRGB_G_GPIO_PIN BOARD_LEDRGB1_G_GPIO_PIN
    #define BOARD_LEDRGB_R_GPIO_PIN BOARD_LEDRGB1_R_GPIO_PIN
#endif
int main (void)
{
    // Initialize pinmuxes
    int32_t ret = board_pins_config();
    if (ret != 0) {
        while(1);
    }

    gpio_b->Initialize(BOARD_LEDRGB_B_GPIO_PIN, NULL);
    gpio_b->PowerControl(BOARD_LEDRGB_B_GPIO_PIN, ARM_POWER_FULL);
    gpio_b->SetDirection(BOARD_LEDRGB_B_GPIO_PIN, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_b->SetValue(BOARD_LEDRGB_B_GPIO_PIN, GPIO_PIN_OUTPUT_STATE_LOW);

    gpio_r->Initialize(BOARD_LEDRGB_R_GPIO_PIN, NULL);
    gpio_r->PowerControl(BOARD_LEDRGB_R_GPIO_PIN, ARM_POWER_FULL);
    gpio_r->SetDirection(BOARD_LEDRGB_R_GPIO_PIN, GPIO_PIN_DIRECTION_OUTPUT);
    gpio_r->SetValue(BOARD_LEDRGB_R_GPIO_PIN, GPIO_PIN_OUTPUT_STATE_LOW);

    #ifdef CORE_M55_HE
    SysTick_Config(SystemCoreClock/10);
#else
    SysTick_Config(SystemCoreClock/25);
#endif

    while (1) __WFI();
}

void SysTick_Handler (void)
{
#ifdef CORE_M55_HE
    gpio_b->SetValue(BOARD_LEDRGB_B_GPIO_PIN, GPIO_PIN_OUTPUT_STATE_TOGGLE);
#else
    gpio_r->SetValue(BOARD_LEDRGB_R_GPIO_PIN, GPIO_PIN_OUTPUT_STATE_TOGGLE);
#endif
}

// Stubs to suppress missing stdio definitions for nosys
#define TRAP_RET_ZERO  {__BKPT(0); return 0;}
int _close(int val) TRAP_RET_ZERO
int _lseek(int val0, int val1, int val2) TRAP_RET_ZERO
int _read(int val0, char * val1, int val2) TRAP_RET_ZERO
int _write(int val0, char * val1, int val2) TRAP_RET_ZERO
