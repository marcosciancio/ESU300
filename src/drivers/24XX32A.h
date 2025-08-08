/**
 * @file    24XX32A.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.USART_RXC_ISR
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 * 
 *  8 channels, 12 bits, does not include EEPROM
 *
 */

#ifndef _MCP_24XX32A_H_
#define _MCP_24XX32A_H_

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
} dev_eeprom_t;

// Function prototypes
bool MCP24XX32A_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function);

// Function to read and write a byte by byte
bool MCP24XX32A_write(uint16_t address, uint8_t value);
bool MCP24XX32A_read(uint16_t address, uint8_t *value);

// TODO need to implement Function to read and write a page or several bytes.
//bool MCP24XX32A_write_page(dev_ctx_t *ctx, uint16_t address, uint8_t *data,uint32_t len, uint32_t Timeout);
//bool MCP24XX32A_read_buffer(dev_ctx_t *ctx, uint16_t address, uint8_t *data,uint32_t len, uint32_t Timeout);

#endif // _MCP_24XX32A_H_