/**
 * @file    Control.h
 * @brief   Application data and features implementation
 * @author  Marcos Ciancio / Alejandro Cumano.
 * @version V1.0
 * @date    2025/06
 *
 * @details This module handles the configuration and various features related to the electrosurgical
 *
 */

#include "control.h"
#include "stdio.h"
#include "definitions.h" 
#include "main.h"
#include "sett.h"
#include "i2c_custom.h"

#include "drivers/MCP47FEB28.h"
#include "drivers/24XX32A.h"
#include "drivers/MCP79410.h"
#include "drivers/MCP23017.h"


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Macros
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#define tonocorte  660    //tono de Corte monopolar
#define tonocoag  940   //tono de Coagulación monopolar
#define tonobcorte  440   //tono de Corte Bipolar
#define tonobcoag  550    //tono de Coagulación Bipolar
#define tonoCQM  660    //Tono de error CQM (emitir dos beep)

#define timeon  500
#define timeoff  1500
#define timemode  5

#define ledcorte 2
#define ledcoag 3
#define ledbcorte 4
#define ledbcoag 5
#define buzzer 9

#define Enmain 30   //EN_MAIN_UC

#define pcorte 49   //PCUT_UC
#define pcoag 48    //PCOAG_UC
#define pbcorte 37    //PBCUT_UC
#define pbcoag 36   //PBCOAG_UC

#define M1corte 47    //HCUT1_UC
#define M1coag 46   //HCOAG1_UC
#define M2corte 45    //HCUT2_UC
#define M2coag 44   //HCOAG2_UC
#define HBipo 35    //HBIPO_UC

#define CQM 43    //CQOM OK_UC
#define OscCQM 42   //Falla CQM_UC
#define RFOK 34   //Rfdet_UC
#define LineOK 33   //LineOK_UC
#define Line110Vmode 32 //110V mode_UC


// SPI
#define EnH 16
#define EnL 32
#define FAN 128

#define Remono1 4
#define Remono2 2
#define ReEN 8
#define Rebipo 16

#define ReAbipo 8
#define ReIrange 2
#define ReVsensor 4
#define ReA 64
#define ReB 32
//#define ReA 16
#define ReFilter 128

#define PrimBipo1 0
#define PrimBipo2 32
#define PrimAux 80
#define PrimMono1 64
#define PrimMono2 96
#define PrimMono3 112

#define Mod0_15 15
#define Mod1_15 14
#define Mod2_15 13
#define Mod3_15 12
#define Mod4_15 11
#define Mod5_15 10
#define Mod6_15 9
#define Mod7_15 8
#define Mod8_15 7
#define Mod9_15 6
#define Mod10_15 5
#define Mod11_15 4
#define Mod12_15 3
#define Mod13_15 2
#define Mod14_15 1
#define Mod15_15 0

#define ModPureCut Mod15_15
#define ModBld1Cut Mod9_15
#define ModBld1Cut Mod6_15
#define ModSprayCoag Mod1_15
#define ModNormalCoag Mod1_15
#define ModCARECoag Mod3_15
#define ModSoftCoag Mod3_15
#define ModBipoCoag Mod12_15
#define ModBipoCut Mod15_15

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Global variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

control_t CONTROL = {
    .state = STATE_OFF,
    .error = false,
};

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Private variables
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

// System settings
static settingAnalog_t settingAnalog = {
    .VFSWmax = 0,
    .Volumen = 0,
    .IFSWmax = 0,
    .IPAmax = 0,
    .Vref4 = 0,
    .IRF1max = 0,
    .IRF2max = 0,
    .IRFleakmax = 0,
    .VRFmax = 0,
};

// Timer ?

static uint32_t timer = 0;
static uint8_t stream[40] = {0};
static uint8_t index = 0;
static uint8_t rxByte;


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Function
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

/*static void delayMs(uint32_t delay) {
    timer = delay;

    while (timer);
}*/

void CONTROL_tick(void) {
    if (timer) timer--;
}

bool SPI_ESU300(uint8_t Modcontrol, uint8_t ReOutcontrol, uint8_t ReFiltercontrol) {

    // Result value output
    bool result;
    uint8_t dummyData;

    printf("SPI_ESU300\r\n");
    printf("ReFiltercontrol  []: %u\r\n", ReFiltercontrol);
    printf("ReOutcontrol     []: %u\r\n", ReOutcontrol);
    printf("Modcontrol       []: %u\r\n", Modcontrol);

    // Transmit
    result = SERCOM1_SPI_WriteRead(&ReFiltercontrol, 1, &dummyData, 1);
    if (result == false)
        return false;

    MAIN_delay_ms(1);

    // Transmit
    result = SERCOM1_SPI_WriteRead(&ReOutcontrol, 1, &dummyData, 1);
    if (result == false)
        return false;

    MAIN_delay_ms(1);

    // Transmit
    result = SERCOM1_SPI_WriteRead(&Modcontrol, 1, &dummyData, 1);
    if (result == false)
        return false;

    MAIN_delay_ms(1);

    printf("OK TRANSMISSION\r\n");

    // Turn off slave
    SPI_slaveOFF();
    MAIN_delay_ms(20);

    // Turn on slave
    SPI_slaveON();
    MAIN_delay_ms(20);

    // Check data
    uint8_t dataReadSPI_1;
    uint8_t dataReadSPI_2;
    uint8_t dataReadSPI_3;

    dummyData = 00; // Do not care data

    // Receive
    result = SERCOM1_SPI_WriteRead(&dummyData, 1, &dataReadSPI_1, 1);
    if (result == false)
        return false;

    //printf("CHECK\r\n");
    //printf("OK 1-->%u\r\n", dataReadSPI_1);
    MAIN_delay_ms(1);

    // Receive
    dummyData = ReOutcontrol; // TODO
    result = SERCOM1_SPI_WriteRead(&dummyData, 1, &dataReadSPI_2, 1);
    if (result == false)
        return false;

    //printf("CHECK\r\n");
    //printf("OK 2-->%u\r\n", dataReadSPI_2);
    MAIN_delay_ms(1);

    // Receive
    result = SERCOM1_SPI_WriteRead(&dummyData, 1, &dataReadSPI_3, 1);
    if (result == false)
        return false;

    //printf("CHECK\r\n");
    //printf("OK 3-->%u\r\n", dataReadSPI_3);
    MAIN_delay_ms(1);

    // Comparing data
    if (dataReadSPI_1 != ReFiltercontrol)return false;
    if (dataReadSPI_2 != ReOutcontrol)return false;
    if (dataReadSPI_3 != Modcontrol)return false;

    //printf("SPI_ESU300 executed OK\r\n");
    return true;
}

// Helper
static const char stateHelper[12][21] = {
    "STATE_OFF           \0",
    "STATE_STARTING      \0",
    "STATE_ERROR         \0",
    "STATE_CALIBRACION   \0",
    "STATE_STANDBY       \0",
    "STATE_CORTE_MONO_1  \0",
    "STATE_COAG_MONO_1   \0",
    "STATE_CORTE_MONO_2  \0",
    "STATE_COAG_MONO_2   \0",
    "STATE_CORTE_BIPOLAR \0",
    "STATE_COAG_BIPOLAR  \0",
    "STATE_TURNING_OFF   \0",
};

/*
   @brief  platform specific delay (platform dependent)

   @param  ms        delay in ms

 */

static void platform_delay(uint32_t ms) {
    MAIN_delay_ms(ms);
}

static void printState(void) {
    // Call to process state
    static controlState_t state_old = sizeof (controlState_t) + 1; // Invalid state

    if (state_old != CONTROL.state) {
        if (state_old == sizeof (controlState_t) + 1)
            printf("[STATE] %s\r\n", stateHelper[CONTROL.state]);
        else
            printf("[STATE] %s ---> %s\r\n", stateHelper[state_old], stateHelper[CONTROL.state]);

        state_old = CONTROL.state;
    }
}

/**
 * @brief  HAL Status structures definition
 */


static bool initializeExternalDac(void) {

    if (MCP47FEB28_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick) == false) return false;

    // Set gain for all channels
    if (MCP47FEB28_WriteGAIN_all(GAIN_1X) == false) return false;

    // Set Vref for all channels
    // NOTE
    // external ref ( 2k2 / 2k2 ) --> 2.33V
    // external ref un buffered ( 2k2 / 2k2 ) --> 2.0V
    if (MCP47FEB28_WriteVREF_all(VREF_VDD) == false) return false;

    // Set zero value to channels
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_0, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_1, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_2, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_3, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_4, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_5, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_6, 0) == false) return false;
    if (MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_7, 0) == false) return false;

    return true;
}

bool CONTROL_init(void) {

    // Debug
    printState(); // Print state 


    // Variables
    // Initialize
    settingAnalog.VFSWmax = 0;
    settingAnalog.IFSWmax = 0;
    settingAnalog.IPAmax = 0;
    settingAnalog.IRF1max = 0;
    settingAnalog.IRF2max = 0;
    settingAnalog.IRFleakmax = 0;
    settingAnalog.VRFmax = 0;
    settingAnalog.Volumen = 0;
    settingAnalog.Vref4 = 0;


    // Prepare the CONTROL variable
    CONTROL.state = STATE_OFF;
    CONTROL.error = false;

    // Demo for change state 
    timer = 10;


    // SPI test hardware
    if (SPI_ESU300(0x00, 0x80, 0x80) == false) // TODO check with another value
        printf("Initialize SPI main NO OK\r\n");
    printf("Initialize SPI main OK\r\n");

    if (SPI_ESU300(0x00, 0x00, 0x00) == false)
        printf("Initialize SPI main NO OK\r\n");
    printf("Initialize SPI main OK\r\n");

    // Dac external initialize
    if (initializeExternalDac() == false) // TODO
        printf("Initialize External Dac NO OK\r\n");
    printf("Initialize External Dac OK\r\n");

    // EEprom initialize
    if (MCP24XX32A_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick) == false)
        printf("Initialize Eeprom NO OK\r\n");
    printf("Initialize Eeprom OK\r\n");

    // RTC initialize
    if (MCP79410_Init(true, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick) == false)
        printf("Initialize RTC NO OK\r\n");
    printf("Initialize RTC OK\r\n");

    // EXPANDER initialize
    if (MCP23017_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick) == false)
        printf("Initialize Expander NO OK\r\n");
    printf("Initialize Expander OK\r\n");

    // Configure internal DAC
    DAC_DataWrite(0);


    // Restore
    ENL_Clear(); // TODO CHECK FOR ALL OUTPUTS

    return true;
}

static void process_STATE_OFF(void) {

    //
    if (CONTROL_init() == false) {

        // Change state to error
        CONTROL.state = STATE_ERROR;

        return;
    }


    //

    // Change state to error
    CONTROL.state = STATE_STANDBY;



    /* if (timer == 0) {

         timer = 10;
         return;
     }*/
}

//==============================================================================
// First test for SPI control

void SPI_slaveOFF(void) {

    LATCH_Set();
}

void SPI_slaveON(void) {

    LATCH_Clear();
}

//==============================================================================

/*void I2C_ScanBus(void) {
    uint8_t dummy = 0x00;
    bool found = false;

    for (uint8_t addr = 0x00; addr <= 0x7F; addr++) {

        SERCOM2_I2C_Write(addr, &dummy, 1);

        // Wait
        while (SERCOM2_I2C_IsBusy());

        // Check fot NACK
        if (SERCOM2_I2C_ErrorGet() == SERCOM_I2C_ERROR_NONE) {
            printf("I2C device found at 0x%02X\r\n", addr);
            found = true;
        }
    }

    if (!found) {
        printf("No I2C devices found.\r\n");
    }
}*/

static void test_rtc(void) {
    bool result;
    uint8_t registerValue = 0xAA;

    // Init
    MCP79410_Init(true, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick);

    // Write
    result = MCP79410_Enable_Oscilator();
    printf("Return           []: %u\r\n", result);

    // Read
    result = MCP79410_get_reg(&registerValue);
    printf("Return           []: %u\r\n", result);
    printf("registerValue    []: %u\r\n", registerValue);

    MAIN_delay_ms(2000);

    // Read
    result = MCP79410_get_reg(&registerValue);
    printf("Return           []: %u\r\n", result);
    printf("registerValue    []: %u\r\n", registerValue);

    MAIN_delay_ms(2000);

    // Read
    result = MCP79410_get_reg(&registerValue);
    printf("Return           []: %u\r\n", result);
    printf("registerValue    []: %u\r\n", registerValue);
}

static void test_expander(void) {

    // Init
    MCP23017_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick);

    MCP23017_config();

    // Set output
    MCP23017_SetPortA(0xAA);

    MCP23017_SetPortA(0xFF);
}

static void test_adc(void) {

    // Test internal ADC
    uint16_t adcValue;

    // 2.2V --> Full range. x2 gain --> overflow
    // Read ADC
    ADC_ChannelSelect(ADC_POSINPUT_PIN3, ADC_NEGINPUT_GND); // TODO ADC_INPUTCTRL_MUXNEG_IOGND?
    MAIN_delay_ms(1);
    ADC_ConversionStart();
    while (ADC_ConversionStatusGet() == true);
    adcValue = ADC_ConversionResultGet();
    printf("PIN3 VRF2        []: %u\r\n", adcValue);

    // Read ADC
    ADC_ChannelSelect(ADC_POSINPUT_PIN4, ADC_NEGINPUT_GND); // TODO ADC_INPUTCTRL_MUXNEG_IOGND?
    MAIN_delay_ms(1);
    ADC_ConversionStart();
    while (ADC_ConversionStatusGet() == true);
    adcValue = ADC_ConversionResultGet();
    printf("PIN4 VTEMP       []: %u\r\n", adcValue);

    // Read ADC
    ADC_ChannelSelect(ADC_POSINPUT_PIN5, ADC_NEGINPUT_GND); // TODO ADC_INPUTCTRL_MUXNEG_IOGND?
    MAIN_delay_ms(1);
    ADC_ConversionStart();
    while (ADC_ConversionStatusGet() == true);
    adcValue = ADC_ConversionResultGet();
    printf("PIN5 VCQM        []: %u\r\n", adcValue);

    // Read ADC
    ADC_ChannelSelect(ADC_POSINPUT_PIN6, ADC_NEGINPUT_GND); // TODO ADC_INPUTCTRL_MUXNEG_IOGND?
    MAIN_delay_ms(1);
    ADC_ConversionStart();
    while (ADC_ConversionStatusGet() == true);
    adcValue = ADC_ConversionResultGet();
    printf("PIN6 IFSW        []: %u\r\n", adcValue);

    // Read ADC
    ADC_ChannelSelect(ADC_POSINPUT_PIN9, ADC_NEGINPUT_GND); // TODO ADC_INPUTCTRL_MUXNEG_IOGND?
    MAIN_delay_ms(1);
    ADC_ConversionStart();
    while (ADC_ConversionStatusGet() == true);
    adcValue = ADC_ConversionResultGet();
    printf("PIN9 V_AUTOBIPO  []: %u\r\n", adcValue);

    // TODO error INPUT

    // Read INPUT
    bool digValue;

    // Read
    MAIN_delay_ms(1);
    digValue = IRFleak_Get();
    printf("IRFleak          []: %u\r\n", digValue);

    // Read
    MAIN_delay_ms(1);
    digValue = IRF2_Get();
    printf("IRF2             []: %u\r\n", digValue);

    // Read
    MAIN_delay_ms(1);
    digValue = VRF1_Get();
    printf("VRF1             []: %u\r\n", digValue);

    // Read
    MAIN_delay_ms(1);
    digValue = IRF1_Get();
    printf("IRF1             []: %u\r\n", digValue);

    // Read
    MAIN_delay_ms(1);
    digValue = IPA_Get();
    printf("IPA              []: %u\r\n", digValue);

    // Read
    MAIN_delay_ms(1);
    digValue = VFSW_Get();
    printf("VFSW_Get         []: %u\r\n", digValue);

    MAIN_delay_ms(1);
}

static void test_inputs(void) {

    // Read INPUT
    bool digValue;

    // Read
    digValue = PCUT_Get();
    printf("PCUT             []: %u\r\n", digValue);

    digValue = PCOAG_Get();
    printf("PCOAG            []: %u\r\n", digValue);

    digValue = PBCOAG_Get();
    printf("PBCOAG           []: %u\r\n", digValue);

    digValue = PBCUT_Get();
    printf("PBCUT            []: %u\r\n", digValue);

    digValue = V110V_mode_Get();
    printf("110V mode        []: %u\r\n", digValue);

    digValue = HCOAG1_Get();
    printf("HCOAG1           []: %u\r\n", digValue);

    digValue = HBipo_Get();
    printf("HBipo            []: %u\r\n", digValue);

    digValue = LineOK_Get();
    printf("LineOK           []: %u\r\n", digValue);

    digValue = HCUT1_Get();
    printf("HCUT1            []: %u\r\n", digValue);

    digValue = HCOAG2_Get();
    printf("HCOAG2           []: %u\r\n", digValue);

    digValue = HCUT2_Get();
    printf("HCUT2            []: %u\r\n", digValue);

    digValue = Falla_CQM_Get();
    printf("Falla_CQM        []: %u\r\n", digValue);

    digValue = RFdet_Get();
    printf("RFdet            []: %u\r\n", digValue);

    digValue = CQM_OK_Get();
    printf("CQM_OK           []: %u\r\n", digValue);

    digValue = INTERRUPT_1_Get();
    printf("INTERRUPT_1      []: %u\r\n", digValue);

    digValue = INTERRUPT_2_Get();
    printf("INTERRUPT_2      []: %u\r\n", digValue);

    digValue = PB_31_Get();
    printf("PB_31            []: %u\r\n", digValue);

    digValue = PB_31_Get();
    printf("PB_00            []: %u\r\n", digValue);
}

static void test_eeprom(void) {
    // bool result;

    uint8_t dataReaded = 0;
    //  printf("dataReaded    []: %u\r\n", dataReaded);

    // Init memory
    MCP24XX32A_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick);

    // Write
    //MCP24XX32A_write(0x1C9, 0x22);

    // Read
    dataReaded = 0;
    MCP24XX32A_read(0x1C9, &dataReaded);
    printf("dataReaded       []: %u\r\n", dataReaded);
}

static void test_external_dac(void) {
    bool result;

    MCP47FEB28_Init(false, I2C_customWrite, I2C_customRead, platform_delay, MAIN_getsystemTick);


    // Set gain for all channels
    result = MCP47FEB28_WriteGAIN_all(GAIN_1X);

    // Set Vref for all channels
    //NOTE
    // external ref ( 2k2 / 2k2 ) --> 2.33V
    // external ref un buffered ( 2k2 / 2k2 ) --> 2.0V
    result = MCP47FEB28_WriteVREF_all(VREF_VDD);

    // Set value to channel 3 max value
    result = MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_0, 1024);
    printf("Result           []: %u\r\n", result);

    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_1, 1024);
    /*MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_2, 1024);
    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_3, 1024);
    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_4, 1024);
    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_5, 1024);
    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_6, 4095);
    MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_7, 2048);*/
}

static void commonTasks(void) {
}

static void process_STATE_STARTING(void) {
};

static void process_STATE_ERROR(void) {
};

static void process_STATE_STANDBY(void) {
    // Check for inputs
    bool value = PBCUT_Get();

    if (value == false) {
        // Change state to error
        CONTROL.state = STATE_CORTE_BIPOLAR;

        // --------------- SEND VALUES
        SPI_ESU300(0, 16, 10);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_0, 512);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_1, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_2, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_3, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_4, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_5, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_6, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_7, 2048);
        DAC_DataWrite(512); // NOTE DAC 10 bits  

        ENL_Set();
        printf("CUT BIPOLAR\r\n ");
        // tone(buzzer, tonobcorte);
        //------------------
        return;
    }
};

static void process_STATE_CORTE_MONO_1(void) {
};

static void process_STATE_COAG_MONO_1(void) {
};

static void process_STATE_CORTE_MONO_2(void) {
};

static void process_STATE_COAG_MONO_2(void) {
};

static uint8_t hexToVal(uint8_t ascii) {
    if (ascii >= '0' && ascii <= '9') return ascii - '0';
    if (ascii >= 'A' && ascii <= 'F') return ascii - 'A' + 10;
    if (ascii >= 'a' && ascii <= 'f') return ascii - 'a' + 10;
    return 0; // todo
}

static void process_STATE_CORTE_BIPOLAR(void) {
    // Check for inputs
    bool value = PBCUT_Get();

    if (value == true) {
        // Change state to error
        CONTROL.state = STATE_STANDBY;

        // --------------- SEND VALUES
        ENL_Clear();
        DAC_DataWrite(0); // NOTE DAC 10 bits
        MAIN_delay_ms(10);
        SPI_ESU300(0, 0, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_0, 2048);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_1, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_2, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_3, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_4, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_5, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_6, 0);
        MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_7, 0);
        printf("INACTIVE ");
        //------------------
    }

    uint8_t rxByte;

    //=================================================================
    //  START   value1   value2   value3   Output   END
    //  $02       xx       xx       xx       xx     $03        
    //=================================================================
    if (SERCOM3_USART_ReceiverIsReady()) {
        rxByte = SERCOM3_USART_ReadByte();
        printf("echo : %u \r\n", rxByte);

        // ECHO 

        // Process data 
        // Process data 
        switch (rxByte) {
            case 2:
                printf("State of frame\r\n");
                memset(stream, 0, sizeof (stream));
                index = 0;
                break;
            case 3:
                printf("End of frame\r\n");

                printf("Data %X\r\n", stream[0]);
                printf("Data %X\r\n", stream[1]);
                printf("Data %X\r\n", stream[2]);

                // External DACF
                printf("DAC 1 %X %X\r\n", stream[3], stream[4]);
                printf("DAC 2 %X %X\r\n", stream[5], stream[6]);
                printf("DAC 3 %X %X\r\n", stream[7], stream[8]);
                printf("DAC 4 %X %X\r\n", stream[9], stream[10]);
                printf("DAC 5 %X %X\r\n", stream[11], stream[12]);
                printf("DAC 6 %X %X\r\n", stream[13], stream[14]);
                printf("DAC 7 %X %X\r\n", stream[15], stream[16]);
                printf("DAC 8 %X %X\r\n", stream[17], stream[18]);

                // Internal dac
                printf("dac   %X %X\r\n", stream[19], stream[20]);

                // Data
                SPI_ESU300(stream[0], stream[1], stream[2]);

                // Dac
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_0, hexToVal(stream[3])*256 + hexToVal(stream[4]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_1, hexToVal(stream[7])*256 + hexToVal(stream[6]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_2, hexToVal(stream[9])*256 + hexToVal(stream[8]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_3, hexToVal(stream[11])*256 + hexToVal(stream[10]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_4, hexToVal(stream[13])*256 + hexToVal(stream[12]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_5, hexToVal(stream[15])*256 + hexToVal(stream[14]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_6, hexToVal(stream[17])*256 + hexToVal(stream[16]));
                MCP47FEB28_WriteDAC(MCP47FEB28_CHANNEL_7, hexToVal(stream[19])*256 + hexToVal(stream[18]));

                //Dac
                DAC_DataWrite(hexToVal(stream[19])*256 + hexToVal(stream[20]));

                // Clear
                memset(stream, 0, sizeof (stream));
                index = 0;

                break;
            default:
                printf("Default\r\n");
                if (index < (sizeof (stream) - 1))
                    stream[index++] = rxByte;
                break;
        }
    }
};

static void process_STATE_STATE_COAG_BIPOLAR(void) {
};

static void process_STATE_TURNING_OFF(void) {
};

void CONTROL_process(void) {
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    // State machine
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    printState();

    switch (CONTROL.state) {
        case STATE_OFF:
            process_STATE_OFF();
            break;
        case STATE_STARTING:
            commonTasks();
            process_STATE_STARTING();
            break;
        case STATE_ERROR:
            commonTasks();
            process_STATE_ERROR();
            break;
        case STATE_STANDBY:
            commonTasks();
            process_STATE_STANDBY();
            break;
        case STATE_CORTE_MONO_1:
            commonTasks();
            process_STATE_CORTE_MONO_1();
            break;
        case STATE_COAG_MONO_1:
            commonTasks();
            process_STATE_COAG_MONO_1();
            break;
        case STATE_CORTE_MONO_2:
            commonTasks();
            process_STATE_CORTE_MONO_2();
            break;
        case STATE_COAG_MONO_2:
            commonTasks();
            process_STATE_COAG_MONO_2();
            break;
        case STATE_CORTE_BIPOLAR:
            commonTasks();
            process_STATE_CORTE_BIPOLAR();
            break;
        case STATE_COAG_BIPOLAR:
            commonTasks();
            process_STATE_STATE_COAG_BIPOLAR();
            break;
        case STATE_TURNING_OFF:
            commonTasks();
            process_STATE_TURNING_OFF();
            break;
        default:
            NVIC_SystemReset(); // Unexpected state, then reset
            break;
    }
}

/* *****************************************************************************
 End of File
 */
