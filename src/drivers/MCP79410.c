/**
 * @file    MCP47FEB28.c
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
#define MCP79410_I2C_ADDR  0x6F // 8 bits addressing 

// Non volatile register address
#define  MCP79410_SEC          0x00       //  Seconds register
#define  MCP79410_MIN          0x01       //  Minutes register

// Commands


// Time out to rear / write
#define TIME_OUT_READ_WRITE 100 // In ms.

// Includes
#include "MCP79410.h"

// Variables
static dev_rtc_t dev_rtc;
static bool g_debug = false;

/*static bool readRegister(dev_rtc_t* ctx, uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->read_reg(ctx->handle, reg, data, len);
    return ret;
}

 * static bool writeRegister( uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->write_function(ctx->handle, reg, data, len);
    return ret;
}*/

bool MCP79410_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function) {

    // TODO Check if I2C is started

    // Configure functions
    dev_rtc.write_function = write_function;
    dev_rtc.read_function = read_function;
    dev_rtc.delay_function = delay_function;
    dev_rtc.systick_function = systick_function;
    //dev_rtc.handle = handle; // Not needed

    // Debug
    g_debug = debug;

    return true;
}

bool MCP79410_Enable_Oscilator(void) {
    uint8_t registerAddr;
    uint8_t value;

    // Register address
    registerAddr = MCP79410_SEC;

    // Prepare command
    value = 1 << 7;

    if (g_debug) {
        printf("MCP79410_Enable_Oscilator...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
        printf("Value           : %u\r\n", value);
    }

    // Prepare data to send
    uint8_t data[2];
    data[0] = registerAddr;
    data[1] = value;

    // Send
    return dev_rtc.write_function(dev_rtc.handle, MCP79410_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);
}

bool MCP79410_get_reg(uint8_t *buffer) {
    uint8_t registerAddr;

    // Register address
    registerAddr = MCP79410_SEC;

    if (g_debug) {
        printf("MCP79410 read seconds register...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
    }

    // Prepare data to send
    uint8_t data = registerAddr;

    // Send
    return dev_rtc.read_function(dev_rtc.handle, MCP79410_I2C_ADDR, &data, 1, buffer, 1, TIME_OUT_READ_WRITE);
}