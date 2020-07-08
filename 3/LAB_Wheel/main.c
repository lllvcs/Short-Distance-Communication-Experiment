// Standard includes
#include <stdio.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "hw_adc.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"
#include "adc.h"

// Common interface includes
#include "gpio_if.h"

#include "pinmux.h"

#define APPLICATION_VERSION     "1.1.1"

#include "LCD_TFT_ILI9341.h"
#include "Font_lib.h"
#include "LCD_Display.h"


//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif



void wait_ms(long i)
{
 while(i)
	  {__delay_cycles(20000);;i--;}

}

#define NO_OF_SAMPLES 		128

unsigned long pulAdcSamples[4096];
unsigned int  uiIndex=0;
//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
    //
    // Set vector table base
    //
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif

    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}


void Wheelinit(void)
{

    //
    // Pinmux for the selected ADC input pin
    //
    MAP_PinTypeADC(PIN_58,PIN_MODE_255);

    //
    // Configure ADC timer which is used to timestamp the ADC data samples
    //
    MAP_ADCTimerConfig(ADC_BASE,2^17);

    //
    // Enable ADC timer which is used to timestamp the ADC data samples
    //
    MAP_ADCTimerEnable(ADC_BASE);

    //
    // Enable ADC module
    //
    MAP_ADCEnable(ADC_BASE);

}

float Wheelgetvalue(void)
{
    unsigned long ulSample;
    float Vol_value;

    //
    // Initialize Array index for multiple execution
    //
    uiIndex=0;

    //
    // Enable ADC channel
    //

    MAP_ADCChannelEnable(ADC_BASE, ADC_CH_1);

    while(uiIndex < NO_OF_SAMPLES + 4)
    {
        if(MAP_ADCFIFOLvlGet(ADC_BASE, ADC_CH_1))
        {
            ulSample = MAP_ADCFIFORead(ADC_BASE, ADC_CH_1);
            pulAdcSamples[uiIndex++] = ulSample;
        }
    }

    MAP_ADCChannelDisable(ADC_BASE, ADC_CH_1);

    uiIndex = 0;

    //
    // Convert ADC samples
    //
    while(uiIndex < NO_OF_SAMPLES)
    {
     Vol_value=(((float)((pulAdcSamples[4+uiIndex] >> 2 ) & 0x0FFF))*1.75)/4096;
     uiIndex++;
    }
    return Vol_value;
}

/*
 * main.c
 */
void main(void) {

	float voltage;
    //
    // Initialize Board configurations
    //
    BoardInit();

    //
    // Power on the corresponding GPIO port B for 9,10,11.
    // Set up the GPIO lines to mode 0 (GPIO)
    //
    PinMuxConfig();

    lcd_init();
    LCD_ILI9341_TFT_background(White);
    LCD_ILI9341_TFT_foreground(Black);
    LCD_Show_StandbyPage();
    lcd_clear();

    Wheelinit();
	while(1)
	{
		voltage=Wheelgetvalue();
        char string[20];
        sprintf(string, "%f", voltage);
		LCD_StringDisplay(50,50,string);
	}
}


