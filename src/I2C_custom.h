/**
 * @file    I2C_custom.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

// Define to prevent recursive inclusion

#ifndef __I2C_CUSTOM_H_
#define __I2C_CUSTOM_H_

#include "stdbool.h"
#include "stdint.h"

// Function with timeout
bool I2C_customWrite(void *handle, uint16_t address, uint8_t *buffer, uint32_t len, uint32_t Timeout) ;
bool I2C_customRead(void *handle, uint16_t address, uint8_t *writeBuffer, uint32_t writeLen, uint8_t* readBuffer, uint32_t readLen, uint32_t Timeout) ;

#endif // __I2C_CUSTOM_H_




