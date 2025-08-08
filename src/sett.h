/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    sett.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

// Define to prevent recursive inclusion

#ifndef __SETT_H_
#define __SETT_H_

// Include
#include "stdbool.h"
#include "stdint.h"

#define SETT_MAGIC_NUMBER 0xD1234568     // Number for settings validation


typedef struct {
  uint32_t checksum;    // Checksum
  uint32_t magic;       // Magic number

  // Stand by
  uint8_t value1;  

} appSetting_t;

// Global Variables
extern appSetting_t appSettings;

extern volatile uint8_t g_remoteSOCbattery;

// Prototypes
bool SETT_init(void);
bool SETT_save(void);
bool SETT_read(bool defaultSettings);

#endif /* SETT_H_ */