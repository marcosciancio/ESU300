/**
 * @file    control.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

#include "stdbool.h"
#include "stdint.h"

#ifndef _CONTROL_H    /* Guard against multiple inclusion */
#define _CONTROL_H


// Control state

typedef enum {
    STATE_OFF = 0,
    STATE_STARTING, // Inicializacion + mensaje
    STATE_ERROR, // Tipo de mensaje segun configuracion
    STATE_CALIBRACION,
    STATE_STANDBY,
    STATE_CORTE_MONO_1,
    STATE_COAG_MONO_1,
    STATE_CORTE_MONO_2,
    STATE_COAG_MONO_2,
    STATE_CORTE_BIPOLAR,
    STATE_COAG_BIPOLAR,
    STATE_TURNING_OFF,
} controlState_t;

typedef struct {
    uint16_t VFSWmax;  //
    uint16_t Volumen;   //
    uint16_t IFSWmax;    //
    uint16_t IPAmax ;   // 
    uint16_t Vref4;
    uint16_t IRF1max ;
    uint16_t IRF2max ;
    uint16_t IRFleakmax ;
    uint16_t VRFmax ;
} settingAnalog_t;

// Control variable

typedef struct {
    // Related to control the device
    controlState_t state; // state
    bool error;
} control_t;

// Expanded type SERCOM_I2C_ERROR!
/*typedef enum
{
  HAL_OK         = 0,
  HAL_ERROR_NAK  ,
  HAL_ERROR_BUS  ,
  HAL_BUSY       ,
  HAL_TIMEOUT    ,         
} HAL_I2C_customError;*/

// Variables
extern control_t CONTROL; // Control variables

// Functions
int ExampleFunction(int param1, int param2);

// Control prototypes
bool CONTROL_init(void);
void CONTROL_process();
void CONTROL_tick(void);

#endif /* _CONTROL_H */

/* *****************************************************************************
 End of File
 */
