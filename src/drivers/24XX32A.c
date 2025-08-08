/**
 * @file    24XX32A.c
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

// Macros
// Base address MCP47FEB28 ( A0 = A1 = GND)
#define MCP24XX32A_I2C_ADDR  0x50 // 8 bits addressing 

#define TIME_OUT_READ_WRITE 100

// Includes
#include "24XX32A.h"

// Variables
static dev_eeprom_t dev_eeprom;
static bool g_debug = false;

bool MCP24XX32A_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function) {
    // TODO Check if I2C is started

    // Configure functions
    dev_eeprom.write_function = write_function;
    dev_eeprom.read_function = read_function;
    dev_eeprom.delay_function = delay_function;
    dev_eeprom.systick_function = systick_function;
    //dev_eeprom.handle = handle; // Not needed

    // Debug
    g_debug = debug;

    return true;
}

bool MCP24XX32A_write(uint16_t address, uint8_t value) {

    uint8_t data[3];
    data[0] = address >> 8;
    data[1] = address & 0x00FF;
    data[2] = value;

    if (g_debug) {
        printf("MCP24XX32A_write...\r\n");
        printf("address high    : %u\r\n", data[0]);
        printf("address low     : %u\r\n", data[1]);
        printf("data            : %u\r\n", data[2]);
    }

    bool result = dev_eeprom.write_function(dev_eeprom.handle, MCP24XX32A_I2C_ADDR, data, 3, TIME_OUT_READ_WRITE);

    return result;
}

bool MCP24XX32A_read(uint16_t address, uint8_t * value) {

    uint8_t data[2];

    // Prepare to send
    data[0] = address >> 8;
    data[1] = address & 0x00FF;

    if (g_debug) {
        printf("MCP24XX32A_read...\r\n");
        printf("address high    : %u\r\n", data[0]);
        printf("address low     : %u\r\n", data[1]);
        printf("data            : %u\r\n", data[2]);
    }

    // Send data
    bool result = dev_eeprom.read_function(dev_eeprom.handle, MCP24XX32A_I2C_ADDR, data, sizeof (data), value, 1, TIME_OUT_READ_WRITE);

    if (g_debug) {
        printf("result          : %u\r\n", result);
        printf("receivedData    : %u\r\n", *value);
    }

    return result;
}