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
#define MCP47FEB28_I2C_ADDR  0x60 // 8 bits addressing 

// Non volatile register address
#define VOLATILE_DAC0_REGISTER 0x00
#define VOLATILE_DAC1_REGISTER 0x01
#define VOLATILE_DAC2_REGISTER 0x02
#define VOLATILE_DAC3_REGISTER 0x03
#define VOLATILE_DAC4_REGISTER 0x04
#define VOLATILE_DAC5_REGISTER 0x05
#define VOLATILE_DAC6_REGISTER 0x06
#define VOLATILE_DAC7_REGISTER 0x07
//----------------
#define VOLATILE_VREF_REGISTER 0x08
#define VOLATILE_POWER_DOWN_REGISTER 0x09
#define GAIN_STATUS_REGISTER 0x0A
#define WIPER_LOCK 0x0B

// Commands
#define WRITE_COMMAND 0x00
#define READ_COMMAND 0x03
#define ENABLE_CONFIGURATION_BITS 0x02
#define DISABLE_CONFIGURATION_BITS 0x01

// Time out to rear / write
#define TIME_OUT_READ_WRITE 100 // In ms.

// Includes
#include "MCP47FEB28.h"

// Variables
static dev_dac_t dev_dac;
static bool g_debug = false;

/*static bool readRegister(dev_dac_t* ctx, uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->read_reg(ctx->handle, reg, data, len);
    return ret;
}

 * static bool writeRegister( uint8_t reg, uint8_t *data, uint16_t len) {
    bool ret;
    ret = ctx->write_function(ctx->handle, reg, data, len);
    return ret;
}*/

bool MCP47FEB28_Init( bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function) {

    // TODO Check if I2C is started

    // Configure functions
    dev_dac.write_function = write_function;
    dev_dac.read_function = read_function;
    dev_dac.delay_function = delay_function;
    dev_dac.systick_function = systick_function;
    //dev_dac.handle = handle; // Not needed

    // Debug
    g_debug = debug;

    return true;
}

bool MCP47FEB28_ReadStatus( MCP47FEB28_Status_t *status) {
    uint8_t registerAddr;
    uint8_t command;

    // Prepare command
    registerAddr = GAIN_STATUS_REGISTER;
    command = (registerAddr << 3) + (READ_COMMAND << 1);

    // Debug
    if (g_debug) {
        printf("MCP47FEB28_ReadStatus...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
        printf("Command         : %u\r\n", command);
    }

    // Prepare data to send
    uint8_t data;
    uint8_t receivedData[2];
    data = command;

    // Send data
    bool result = dev_dac.read_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, &data, 1, receivedData, 2, TIME_OUT_READ_WRITE);

    // Debug
    if (g_debug) {
        printf("Received Data[0]: %u\r\n", receivedData[0]);
        printf("Received Data[1]: %u\r\n", receivedData[1]);
        printf("Result          : %u\r\n\r\n", result);
    }

    // Prepare data
    uint16_t raw = ((uint16_t) receivedData[0] << 8) | receivedData[1];

    // Parsed bit a bit:
    status->G7 = (raw >> 15) & 0x01;
    status->G6 = (raw >> 14) & 0x01;
    status->G5 = (raw >> 13) & 0x01;
    status->G4 = (raw >> 12) & 0x01;
    status->G3 = (raw >> 11) & 0x01;
    status->G2 = (raw >> 10) & 0x01;
    status->G1 = (raw >> 9) & 0x01;
    status->G0 = (raw >> 8) & 0x01;
    status->POR = (raw >> 7) & 0x01;
    status->EEWA = (raw >> 6) & 0x01;

    return result;
}

bool MCP47FEB28_WriteDAC( MCP47FEB28_Channel_t channel, uint16_t value) {
    uint8_t registerAddr;
    uint8_t command;

    // Sanity check
    if (value > 0x0FFF) return false;

    // Register address
    registerAddr = VOLATILE_DAC0_REGISTER + channel;

    // Prepare command
    command = (registerAddr << 3) + (WRITE_COMMAND << 1);

    if (g_debug) {
        printf("MCP47FEB28_WriteDAC...\r\n");
        printf("Register ADDR   : %u\r\n", registerAddr);
        printf("Command         : %u\r\n", command);
        printf("Value DAC       : %u\r\n", value);
    }

    // Prepare data to send
    uint8_t data[3];
    data[0] = command;
    data[1] = (value & 0xFF00) >> 8;
    data[2] = (value & 0x00FF);

    // Send
    return dev_dac.write_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);
}

bool MCP47FEB28_WriteGAIN( MCP47FEB28_GainRegister_t *gainRegister) {

    uint8_t command;
    uint8_t reg_addr;

    // Register address
    reg_addr = GAIN_STATUS_REGISTER;

    // Prepare command
    command = (reg_addr << 3) + (WRITE_COMMAND << 1);

    uint8_t data[3];
    data[0] = command;

    data[1] = gainRegister->gainChannel_0 +
            (gainRegister->gainChannel_1 << 1)+
            (gainRegister->gainChannel_2 << 2)+
            (gainRegister->gainChannel_3 << 3)+
            (gainRegister->gainChannel_4 << 4)+
            (gainRegister->gainChannel_5 << 5)+
            (gainRegister->gainChannel_6 << 6)+
            (gainRegister->gainChannel_7 << 7);

    data[2] = 0; // TODO Check

    if (g_debug) {
        printf("MCP47FEB28_WriteGAIN_ALL...\r\n");
        printf("reg_addr        : %u\r\n", reg_addr);
        printf("command         : %u\r\n", command);
    }

    bool result = dev_dac.write_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);

    return result;
}

bool MCP47FEB28_WriteVREF( MCP47FEB28_VREF_Register_t* vrefRegister) {
    uint8_t command;
    uint8_t reg_addr;

    //Register address
    reg_addr = VOLATILE_VREF_REGISTER;

    // Prepare command
    command = (reg_addr << 3) + (WRITE_COMMAND << 1);

    uint8_t data[3];
    data[0] = command;
    data[1] = vrefRegister->VrefChannel_0 + (vrefRegister->VrefChannel_1 << 2)+ (vrefRegister->VrefChannel_2 << 4)+ (vrefRegister->VrefChannel_3 << 6);
    data[2] = vrefRegister->VrefChannel_4 + (vrefRegister->VrefChannel_5 << 2)+ (vrefRegister->VrefChannel_6 << 4)+ (vrefRegister->VrefChannel_7 << 6);

    if (g_debug) {
        printf("MCP47FEB28_WriteVREF_ALL...\r\n");
        printf("reg_addr        : %u\r\n", reg_addr);
        printf("command         : %u\r\n", command);
    }

    bool result = dev_dac.write_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);

    return result;
}

bool MCP47FEB28_WriteVREF_all( MCP47FEB28_VREF_Source_t vref) {
    uint8_t command;
    uint8_t reg_addr;

    //Register address
    reg_addr = VOLATILE_VREF_REGISTER;

    // Prepare command
    command = (reg_addr << 3) + (WRITE_COMMAND << 1);

    uint8_t data[3];
    data[0] = command;
    data[1] = vref + (vref << 2)+ (vref << 4)+ (vref << 6);
    data[2] = vref + (vref << 2)+ (vref << 4)+ (vref << 6);

    if (g_debug) {
        printf("MCP47FEB28_WriteVREF_ALL...\r\n");
        printf("reg_addr        : %u\r\n", reg_addr);
        printf("command         : %u\r\n", command);
        printf("VREF            : %u\r\n", vref);
    }

    bool result = dev_dac.write_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);

    return result;
}

bool MCP47FEB28_WriteGAIN_all( MCP47FEB28_Gain_t gain) {
    uint8_t command;
    uint8_t reg_addr;

    // Register address
    reg_addr = GAIN_STATUS_REGISTER;

    // Prepare command
    command = (reg_addr << 3) + (WRITE_COMMAND << 1);

    uint8_t data[3];
    data[0] = command;
    data[1] = gain + (gain << 1)+ (gain << 2)+ (gain << 3)+ (gain << 4)+ (gain << 5)+ (gain << 6)+ (gain << 7);
    data[2] = 0;

    if (g_debug) {
        printf("MCP47FEB28_WriteGAIN_ALL...\r\n");
        printf("reg_addr        : %u\r\n", reg_addr);
        printf("command         : %u\r\n", command);
        printf("gain            : %u\r\n", gain);
    }

    bool result = dev_dac.write_function(dev_dac.handle, MCP47FEB28_I2C_ADDR, data, sizeof (data), TIME_OUT_READ_WRITE);

    return result;
}
