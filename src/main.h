/**
 * @file    main.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

#ifndef _MAIN_H
#define _MAIN_H

// Section: Included Files

#include "stdbool.h"

// MACROS

// Checked is running at 8MHZ
#define SystemCoreClock 8000000UL   // 
#define SYSTICK_INTERVAL_MS 10       // systick value in ms

// Provide C++ Compatibility
#ifdef __cplusplus
extern "C" {
#endif

    // Delay
    void MAIN_delay_ms(uint32_t ms);

    // WDT
    void MAIN_kickWDT(void);

    // Systick related
    void MAIN_systick_init(void);
    uint32_t MAIN_getsystemTick(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MAIN_H */

