/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "stdint.h"
#include "stdio.h"
#include "sam.h"
#include "samd21j18a.h"
#include "definitions.h"  // Includes all headers of Harmony

// Own Modules
#include "MAIN.h" // Main control module

// Modules
#include "CONTROL.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Macros
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Global variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Private variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

static volatile uint32_t tick_ms = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Function
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void MAIN_delay_ms(uint32_t ms) {

    // Checked for 8MHZ!
    for (uint32_t i = 0; i < ms * 1600; i++) {
        __asm("nop");
    }
}

void SysTick_Handler(void) {
    // Call all tick functions
    CONTROL_tick();

    // Inc variable for time
    tick_ms = tick_ms + SYSTICK_INTERVAL_MS;
}

uint32_t MAIN_getsystemTick(void) {
    return tick_ms;
}

void MAIN_systick_init(void) {
    // config SysTick
    uint32_t ticks = (SystemCoreClock / 1000) * SYSTICK_INTERVAL_MS;

    SysTick->LOAD = ticks - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // Core clock
            SysTick_CTRL_TICKINT_Msk | // Enable int interrupción
            SysTick_CTRL_ENABLE_Msk; // Enable SysTick
}

void gclk1_init(void) {
    // Habilitar OSCULP32K si no lo está
    /*SYSCTRL_REGS->SYSCTRL_OSCULP32K.bit.ENABLE = 1;
    while (!(SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSCULP32KRDY_Msk));

    // Divisor para GCLK1: no dividir (div=1)
    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_ID(1) | GCLK_GENDIV_DIV(1);

    // Fuente: OSCULP32K, habilitar generador
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_ID(1) |
            GCLK_GENCTRL_SRC_OSCULP32K |
            GCLK_GENCTRL_GENEN;

    // Esperar sincronización
    while (GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk);*/
}

void MAIN_kickWDT(void) {
    // Kick WDT
    WDT_Clear();
}

int main(void) {

    // Initialize all modules
    SYS_Initialize(NULL);

    // Welcome message
    printf("=====================================================\r\n");
    printf("             BISTURI SYSTEM STARTED                  \r\n");
    printf("=====================================================\r\n");

    // TODO 
    // WDT_Enable();

    // Initialize SysTick
    MAIN_systick_init();

    // Initialize all modules
    CONTROL_init();

    // RUN
    while (true) {
        // Maintain state machines of all polled MPLAB Harmony modules.
        SYS_Tasks();

        // Call all process functions
        CONTROL_process();

        // Kick WDT
        WDT_Clear();
    }

    // Execution should not come here during normal operation 
    return ( EXIT_FAILURE);
}

/*******************************************************************************
 End of File
 */

