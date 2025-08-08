/*****************************************************************************
    @file    SETT.c
    @brief   data
 ******************************************************************************

  file Name          : SETT.c
  Author             : Marcos Ciancio
  Version            : V1.0
  Date               : 2023/01
  Description        : Configuration & various features implementation of SETT
  Developed for      : Data
 ********************************************************************************

    Marcos ciancio 27/01/2023 00:00:
      1.  Details 1.
      2.  Details 2.

 *******************************************************************************/

// includes
#include "sett.h"
#include "stdio.h"

// Macros

#define DEFAULT_VALUE 25

// Variables
appSetting_t appSettings;

/**
   Load all default appSettings
 */

static bool SETT_setDefaults(void) {
    //default values

    // Set the default battery capacity
    appSettings.value1 = DEFAULT_VALUE;

    return true;
}

/**
   Init
 */
bool SETT_init(void) {
    //ret_code_t rc;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    //INIT FLAS FOR SAVE OR EXTERNAL EEPROM

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    return true;
}

/**
   @brief checksum calculation
 */

// It has to perform an update into a old Fw version

static uint32_t checksum(void) {
    uint8_t *p;
    uint32_t ck = 0;

    // Pointer as uint8_t * to structure
    p = (uint8_t *) & appSettings + 4;

    //printf("SIZE OF APP_SETTINGS  %u", sizeof(appSettings));

    for (uint8_t i = 0; i < (sizeof (appSettings) - 4); i++) {
        ck += *p;
        p++;
        //printf("Value [%u]: %u \t total : %u", i, *p, ck);
    }

    // Return
    return ck;
}

/**
   @brief save all data into NVM

 */
bool SETT_save(void) {
    //uint32_t rc;

    // Calculate the checksum
    appSettings.checksum = checksum();

    // Set the magic number
    appSettings.magic = SETT_MAGIC_NUMBER;

    printf("Writing to FLASH ");
    printf("Checksum To Save: %u ",(unsigned int) appSettings.checksum);

    // Wait for ready
    //wait_for_flash_ready(&fstorage);

    // Erase the page
    /*rc = nrf_fstorage_erase(&fstorage, FLASH_ADDRESS_START, NUMBER_PAGES_FLASH, NULL);
    if (rc != NRF_SUCCESS)
        return false; // Return false is erase action couldn't be performed

    // Wait for erasing
    wait_for_flash_ready(&fstorage);

    // Write
    rc = nrf_fstorage_write(&fstorage, FLASH_ADDRESS_START, &appSettings, sizeof (appSettings), NULL);
    if (rc != NRF_SUCCESS)
        return false; // Return false is write action couldn't be performed

    // Wait for write
    wait_for_flash_ready(&fstorage);*/

    return true;
}

/**
   @brief Read all data from flash

---------------------------------------------------------------------------
This functions return the APP settings stored on non volatile memory
---------------------------------------------------------------------------

It will return a FALSE if the reading actions is not possible.
It will return a TRUE if the reading actions is possible.
If checksum fails, the default values will be get in to appSettings variable.

---------------------------------------------------------------------------
Parameters
---------------------------------------------------------------------------
If the defaultValues is set, the function Will copy the default values into appSettings variable.
If the onlyRem. Only the remote MAC ADDRESS part will be modified.

 */

bool SETT_read(bool defaultSettings) {
    //uint32_t rc;

    // If I want to get some default values
    if (defaultSettings) {

        if (defaultSettings)
            SETT_setDefaults(); // Read the default values for all Settings.


        //SETT_save();
        return true;
    }

    // No default values. Just reading the values in NVM
    /*rc = nrf_fstorage_read(&fstorage, FLASH_ADDRESS_START, &appSettings, sizeof (appSettings));
    if (rc != NRF_SUCCESS)
        return false;*/

    // Wait for reading
    //wait_for_flash_ready(&fstorage);

    printf("Reading from FLASH ");
    printf("Checksum            : %lu ", (uint32_t) appSettings.checksum);
    printf("Checksum Calculated : %lu ", (uint32_t) checksum());

    if (appSettings.checksum != checksum() || appSettings.magic != SETT_MAGIC_NUMBER) {
        SETT_setDefaults();
        SETT_save();
        return true;
    }

    return true;
}

