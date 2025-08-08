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
// Base address MCP47FEB28 ( A0 = A1 = A2 = GND)
#define MCP23017_I2C_ADDR  0x20 // 8 bits addressing 

// Register address
#define MCP23017_IODIRA		0x00
#define MCP23017_IPOLA 		0x02
#define MCP23017_GPINTENA 	0x04
#define MCP23017_DEFVALA 	0x06
#define MCP23017_INTCONA 	0x08
#define MCP23017_IOCONA 	0x0A
#define MCP23017_GPPUA 		0x0C
#define MCP23017_INTFA 		0x0E
#define MCP23017_INTCAPA 	0x10
#define MCP23017_GPIOA 		0x12
#define MCP23017_OLATA 		0x14


#define MCP23017_IODIRB 	0x01
#define MCP23017_IPOLB 		0x03
#define MCP23017_GPINTENB 	0x05
#define MCP23017_DEFVALB 	0x07
#define MCP23017_INTCONB 	0x09
#define MCP23017_IOCONB 	0x0B
#define MCP23017_GPPUB 		0x0D
#define MCP23017_INTFB 		0x0F
#define MCP23017_INTCAPB 	0x11
#define MCP23017_GPIOB 		0x13
#define MCP23017_OLATB 		0x15

// Commands


// Time out to rear / write
#define TIME_OUT_READ_WRITE 100 // In ms.

// Includes
#include "MCP23017.h"

// Variables
static dev_exp_t dev_exp;
static bool g_debug = false;

/*static bool readRegister(dev_exp_t* ctx, uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->read_reg(ctx->handle, reg, data, len);
    return ret;
}

 * static bool writeRegister( uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->write_function(ctx->handle, reg, data, len);
    return ret;
}*/

bool MCP23017_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function) {

    // TODO Check if I2C is started

    // Configure functions
    dev_exp.write_function = write_function;
    dev_exp.read_function = read_function;
    dev_exp.delay_function = delay_function;
    dev_exp.systick_function = systick_function;
    //dev_exp.handle = handle; // Not needed

    // Debug
    g_debug = debug;

    return true;
}

static bool MCP23017_WriteRegister(uint8_t registerAddr, uint8_t value) {

    if (g_debug) {
        printf("MCP23017_WriteRegister...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
        printf("Value           : %u\r\n", value);
    }

    // Prepare data to send
    uint8_t data[2];
    data[0] = registerAddr;
    data[1] = value;

    // Send
    return dev_exp.write_function(dev_exp.handle, MCP23017_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);
}

static bool MCP23017_ReadRegister(uint8_t registerAddr, uint8_t *value) {

    if (g_debug) {
        printf("MCP23017_WriteRegister...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
    }

    // Prepare data to send
    uint8_t receivedData;

    // Send
    bool result = dev_exp.read_function(dev_exp.handle, MCP23017_I2C_ADDR, &registerAddr, 1, &receivedData, 1, TIME_OUT_READ_WRITE);
    
    *value= receivedData;

    // Debug
    if (g_debug) {
        printf("Received Data []: %u\r\n", receivedData);
        printf("Result          : %u\r\n\r\n", result);
    }

    return result;
}

bool MCP23017_config(void) {

    // PIN as output
    MCP23017_WriteRegister(MCP23017_IODIRA, 0x00); // OUT A
    MCP23017_WriteRegister(MCP23017_IODIRB, 0x00); // OUT B  

    return true; // TODO 
}

bool MCP23017_SetPortA(uint8_t value) {
    return MCP23017_WriteRegister(MCP23017_GPIOA, value);
}

bool MCP23017_SetPortB(uint8_t value) {
    return MCP23017_WriteRegister(MCP23017_GPIOB, value);
}

bool MCP23017_GetPortA(uint8_t *value) {
    return MCP23017_ReadRegister(MCP23017_GPIOA, value);
}

bool MCP23017_GetPortB(uint8_t *value) {
    return MCP23017_ReadRegister(MCP23017_GPIOB, value);
}