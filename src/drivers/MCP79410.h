/**
 * @file    MCP47FEB28.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 * 
 *  8 channels, 12 bits, does not include EEPROM
 *
 */

#ifndef _MCP79410_H_
#define _MCP79410_H_

#include "stdbool.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"

// Datatypes
typedef bool (*dev_write_ptr)(void*, uint16_t, uint8_t*, uint32_t, uint32_t);
typedef bool (*dev_read_ptr) (void*, uint16_t, uint8_t*, uint32_t, uint8_t*, uint32_t, uint32_t);
typedef void (*dev_delay_ptr) (uint32_t);
typedef uint32_t(*dev_sysTick) (void);

typedef struct {
    // Component mandatory fields
    dev_write_ptr write_function;
    dev_read_ptr read_function;
    dev_delay_ptr delay_function;
    dev_sysTick systick_function;
    // Customizable optional pointer
    void *handle;
} dev_rtc_t;

// Channel

// Gain options

/*typedef enum {
    GAIN_1X = 0,
    GAIN_2X = 1
} MCP79410_Gain_t;*/

// Gain options

// Function prototypes
bool MCP79410_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function);

// Read status
bool MCP79410_Enable_Oscilator(void);

bool MCP79410_get_reg(uint8_t *buffer) ;


#endif