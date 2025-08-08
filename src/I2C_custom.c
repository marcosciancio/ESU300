/**
 * @file    I2C_custom.c
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

#include "i2c_custom.h"
#include "control.h"
#include "stdio.h"
#include "definitions.h" 
#include "main.h"
#include "sett.h"

static inline bool checkBusBusy(uint32_t Timeout) {

    // Start of tick. Set up the initial tick
    uint32_t startTick = MAIN_getsystemTick();

    // Check if BUS is busy.
    bool busy;
    do {
        busy = SERCOM2_I2C_IsBusy();
    } while (((uint32_t) (MAIN_getsystemTick() - startTick) <= Timeout) && (busy != false)); // TODO check rollover!

    if (busy != false)
        return true;

    return false;
}

// Function with timeout

bool I2C_customWrite(void *handle, uint16_t address, uint8_t *buffer, uint32_t len, uint32_t Timeout) {

    // Check if busy
    if (checkBusBusy(Timeout) == true) return false;

    // Send data
    if (SERCOM2_I2C_Write(address, buffer, len) == false) return false;

    MAIN_delay_ms(1); // TODO Do I need this delay here?

    // Check if there was ACK on SCK bus
    if (SERCOM2_I2C_ErrorGet() != SERCOM_I2C_ERROR_NONE)
        return false;

    return true;
}

bool I2C_customRead(void *handle, uint16_t address, uint8_t *writeBuffer, uint32_t writeLen, uint8_t* readBuffer, uint32_t readLen, uint32_t Timeout) {

    // Check if busy
    if (checkBusBusy(Timeout) == true) return false;

    // Send data
    if (SERCOM2_I2C_WriteRead(address, writeBuffer, writeLen, readBuffer, readLen) == false) return false;

    MAIN_delay_ms(1); // TODO Do I need this delay here?

    // Check if there was ACK on SCK bus
    if (SERCOM2_I2C_ErrorGet() != SERCOM_I2C_ERROR_NONE)
        return false;

    return true;
}