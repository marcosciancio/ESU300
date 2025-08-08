/*******************************************************************************
 CLOCK PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clock.c

  Summary:
    CLOCK PLIB Implementation File.

  Description:
    None

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#include "plib_clock.h"
#include "device.h"
#include "interrupts.h"

static void SYSCTRL_Initialize(void)
{
    /* Configure External Oscillator */
    SYSCTRL_REGS->SYSCTRL_XOSC = SYSCTRL_XOSC_STARTUP(0U) | SYSCTRL_XOSC_GAIN(4U) | SYSCTRL_XOSC_XTALEN_Msk | SYSCTRL_XOSC_ENABLE_Msk;
    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_XOSCRDY_Msk) != SYSCTRL_PCLKSR_XOSCRDY_Msk)
    {
        /* Waiting for the XOSC Ready state */
    }

    SYSCTRL_REGS->SYSCTRL_OSC32K = 0x0U;
}




static void GCLK0_Initialize(void)
{

    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(0U);

    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(4U) | GCLK_GENDIV_ID(0U);
    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 0 synchronization */
    }
}


static void GCLK1_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(1U);

    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(128U) | GCLK_GENDIV_ID(1U);
    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 1 synchronization */
    }
}


static void GCLK2_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(2U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 2 synchronization */
    }
}


static void GCLK3_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(3U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 3 synchronization */
    }
}


static void GCLK4_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(4U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 4 synchronization */
    }
}


static void GCLK5_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(5U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 5 synchronization */
    }
}


static void GCLK6_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(6U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 6 synchronization */
    }
}


static void GCLK7_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(7U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 7 synchronization */
    }
}


static void GCLK8_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(0U) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(8U);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 8 synchronization */
    }
}





void CLOCK_Initialize (void)
{
    /* Function to Initialize the Oscillators */
    SYSCTRL_Initialize();

    GCLK3_Initialize();
    GCLK4_Initialize();
    GCLK5_Initialize();
    GCLK6_Initialize();
    GCLK7_Initialize();
    GCLK8_Initialize();
    GCLK0_Initialize();
    GCLK1_Initialize();
    GCLK2_Initialize();


    /* Selection of the Generator and write Lock for WDT */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(3U) | GCLK_CLKCTRL_GEN(0x1U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for SERCOM0_CORE */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(20U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for SERCOM1_CORE */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(21U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for SERCOM2_CORE */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(22U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for SERCOM3_CORE */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(23U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for SERCOM4_CORE */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(24U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for ADC */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(30U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for DAC */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(33U) | GCLK_CLKCTRL_GEN(0x0U)  | GCLK_CLKCTRL_CLKEN_Msk;

    /* Configure the APBC Bridge Clocks */
    PM_REGS->PM_APBCMASK = 0x5007cU;


    /*Disable RC oscillator*/
    SYSCTRL_REGS->SYSCTRL_OSC8M = 0x0U;


}
