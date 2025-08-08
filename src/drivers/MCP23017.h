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

#ifndef _MCP23017_H_
#define _MCP23017_H_

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
} dev_exp_t;


typedef enum {
	MCP23017_IODIR		= 0x00,
	MCP23017_IPOL		= 0x01,
	MCP23017_GPINTEN	= 0x02,
	MCP23017_DEFVAL	= 0x03,
	MCP23017_INTCON	= 0x04,
	MCP23017_IOCON		= 0x05,
	MCP23017_GPPU		= 0x06,
	MCP23017_INTF		= 0x07,
	MCP23017_INTCAP	= 0x08,
	MCP23017_GPIO		= 0x09,
	MCP23017_OLAT		= 0x0A
} mcp23017_reg_t;

// Channel

// Gain options

/*typedef enum {
    GAIN_1X = 0,
    GAIN_2X = 1
} MCP23017_Gain_t;*/

// Gain options

// Function prototypes
bool MCP23017_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function);

// Config
bool MCP23017_config(void);

// Set command
bool MCP23017_SetPortA(uint8_t value);
bool MCP23017_SetPortB(uint8_t value);

bool MCP23017_GetPortA(uint8_t *value);
bool MCP23017_GetPortB(uint8_t *value);

#endif