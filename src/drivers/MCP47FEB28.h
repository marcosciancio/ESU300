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

#ifndef _MCP4725_H_
#define _MCP4725_H_

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
} dev_dac_t;

// Channel

typedef enum {
    MCP47FEB28_CHANNEL_0 = 0,
    MCP47FEB28_CHANNEL_1,
    MCP47FEB28_CHANNEL_2,
    MCP47FEB28_CHANNEL_3,
    MCP47FEB28_CHANNEL_4,
    MCP47FEB28_CHANNEL_5,
    MCP47FEB28_CHANNEL_6,
    MCP47FEB28_CHANNEL_7
} MCP47FEB28_Channel_t;

// Voltage reference

typedef enum {
    VREF_VDD = 0x00, // Device VDD. VREF pin disconnected.
    VREF_EXT_UNBUF = 0x01, // External unbuffered.
    VREF_EXT_BUF = 0x02, // External buffered. Current amplifier if source current not enough.
    VREF_BANDGAP = 0x03 // Internal Band-Gap. NOTE VREF is an output, but a very slow current It can give.
} MCP47FEB28_VREF_Source_t;

// Gain options

typedef enum {
    GAIN_1X = 0,
    GAIN_2X = 1
} MCP47FEB28_Gain_t;

// Gain options

typedef struct {
    MCP47FEB28_Gain_t gainChannel_0;
    MCP47FEB28_Gain_t gainChannel_1;
    MCP47FEB28_Gain_t gainChannel_2;
    MCP47FEB28_Gain_t gainChannel_3;
    MCP47FEB28_Gain_t gainChannel_4;
    MCP47FEB28_Gain_t gainChannel_5;
    MCP47FEB28_Gain_t gainChannel_6;
    MCP47FEB28_Gain_t gainChannel_7;
} MCP47FEB28_GainRegister_t;

typedef struct {
    MCP47FEB28_VREF_Source_t VrefChannel_0;
    MCP47FEB28_VREF_Source_t VrefChannel_1;
    MCP47FEB28_VREF_Source_t VrefChannel_2;
    MCP47FEB28_VREF_Source_t VrefChannel_3;
    MCP47FEB28_VREF_Source_t VrefChannel_4;
    MCP47FEB28_VREF_Source_t VrefChannel_5;
    MCP47FEB28_VREF_Source_t VrefChannel_6;
    MCP47FEB28_VREF_Source_t VrefChannel_7;
} MCP47FEB28_VREF_Register_t;

// Status register 

typedef struct {
    uint16_t G7 : 1;
    uint16_t G6 : 1;
    uint16_t G5 : 1;
    uint16_t G4 : 1;
    uint16_t G3 : 1;
    uint16_t G2 : 1;
    uint16_t G1 : 1;
    uint16_t G0 : 1;
    uint16_t POR : 1;
    uint16_t EEWA : 1;
    uint16_t unused : 6;
} MCP47FEB28_Status_t;

// Function prototypes
bool MCP47FEB28_Init(bool debug, dev_write_ptr write_function, dev_read_ptr read_function, dev_delay_ptr delay_function, dev_sysTick systick_function);

// Read status
bool MCP47FEB28_ReadStatus(MCP47FEB28_Status_t *status);

// Write gain and Vref
bool MCP47FEB28_WriteGAIN_all(MCP47FEB28_Gain_t gain);
bool MCP47FEB28_WriteVREF_all(MCP47FEB28_VREF_Source_t vref);

// For a particular output
bool MCP47FEB28_WriteGAIN(MCP47FEB28_GainRegister_t *gainRegister);
bool MCP47FEB28_WriteVREF(MCP47FEB28_VREF_Register_t* vrefRegister);

// Write the output value
bool MCP47FEB28_WriteDAC(MCP47FEB28_Channel_t channel, uint16_t value);

#endif