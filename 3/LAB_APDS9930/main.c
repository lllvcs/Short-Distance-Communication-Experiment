// Standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "hw_common_reg.h"
#include "rom.h"
#include "rom_map.h"
#include "interrupt.h"
#include "gpio.h"
#include "pin.h"
#include "prcm.h"
#include "utils.h"
#include "uart.h"

// Common interface includes
#include "uart_if.h"
#include "i2c_if.h"
#include "gpio_if.h"

#include "pinmux.h"
//#include "APDS9930.h"

//*****************************************************************************
//                      MACRO DEFINITIONS
//*****************************************************************************
#define APPLICATION_VERSION     "1.1.0"
#define APP_NAME                "APDS9930"
#define UART_PRINT              Report
#define FOREVER                 1
#define CONSOLE                 UARTA0_BASE
#define FAILURE                 -1
#define SUCCESS                 0
#define RETERR_IF_TRUE(condition) {if(condition) return FAILURE;}
#define RET_IF_ERR(Func)          {int iRetVal = (Func); \
                                   if (SUCCESS != iRetVal) \
                                     return  iRetVal;}



#define  APDS_Addr  0x39

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//****************************************************************************
//                      LOCAL FUNCTION DEFINITIONS                          
//****************************************************************************


//*****************************************************************************
//
//! Display the buffer contents over I2C
//!
//! \param  pucDataBuf is the pointer to the data store to be displayed
//! \param  ucLen is the length of the data to be displayed
//!
//! \return none
//! 
//*****************************************************************************
void 
DisplayBuffer(unsigned char *pucDataBuf, unsigned char ucLen)
{
    unsigned char ucBufIndx = 0;
    UART_PRINT("Read contents");
    UART_PRINT("\n\r");
    while(ucBufIndx < ucLen)
    {
        UART_PRINT(" 0x%x, ", pucDataBuf[ucBufIndx]);
        ucBufIndx++;
        if((ucBufIndx % 8) == 0)
        {
            UART_PRINT("\n\r");
        }
    }
    UART_PRINT("\n\r");
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{

    Report("\n\n\n\r");
    Report("\t\t *************************************************\n\r");
    Report("\t\t      CC3200 %s Application       \n\r", AppName);
    Report("\t\t *************************************************\n\r");
    Report("\n\n\n\r");
}

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


//*****************************************************************************
// write data--byte
//*****************************************************************************
void I2C_writedata(unsigned char command,unsigned char data)
{
  unsigned char TxData[2]={0,0};
  TxData[0]=command;
  TxData[1]=data;
  I2C_IF_Write(APDS_Addr,TxData,2,1);
}

//*****************************************************************************
// clear the interrupt
//*****************************************************************************
void I2C_writecom(unsigned char command)
{
  unsigned char *p;
  p=&command;
  I2C_IF_Write(APDS_Addr,p,1,1);
}

//*****************************************************************************
// write data--word
//*****************************************************************************
void I2C_writeword(unsigned char command,unsigned int value)
{
  unsigned char data[3];
  data[0] = command;
  data[2] = value >> 8;
  data[1] = value & 0xFF;
  I2C_IF_Write(APDS_Addr,data,3,1);   
}

//*****************************************************************************
// read data--byte
//*****************************************************************************
unsigned char I2C_readdata(unsigned char command)
{
  unsigned char *p1;
  p1=&command;
  I2C_IF_Write(APDS_Addr,p1,1,0);
  unsigned char data;
  I2C_IF_Read(APDS_Addr,&data,1);
  return data;
}

//*****************************************************************************
// read data--word
//*****************************************************************************
unsigned int I2C_readword(unsigned char command)
{
  unsigned char *p2;
  p2=&command;
  I2C_IF_Write(APDS_Addr,p2,1,0);
  unsigned char RxData[2]={0,0};
  I2C_IF_Read(APDS_Addr,RxData,2);
  return RxData[1] << 8 | RxData[0];
}

void apds_init(void)
{
   unsigned char ATIME;
   unsigned char PTIME;
   unsigned char WTIME; 
   unsigned char PPULSE; 
   
    ATIME = 0xff; // 2.7 ms - minimum ALS integration time 
    WTIME = 0xff; // 2.7 ms - minimum Wait time 
    PTIME = 0xff; // 2.7 ms - minimum Prox integration time 
    PPULSE = 8; // Minimum prox pulse count 
    
    I2C_writedata(0x80|0x00,0);//Disable and Powerdown 
    I2C_writedata(0x80|0x01,ATIME);
    I2C_writedata(0x80|0x02,PTIME);
    I2C_writedata(0x80|0x03,WTIME);
    I2C_writedata(0x80|0x0d,0);
    I2C_writedata(0x80|0x0e,PPULSE);

   unsigned char PDRIVE;
   unsigned char PDIODE;
   unsigned char PGAIN;
   unsigned char AGAIN; 
  
    PDRIVE = 0; //100mA of LED Power 
    PDIODE = 0x20; // CH1 Diode 
    PGAIN = 0; //1x Prox gain 
    AGAIN = 0; //1x ALS gain 
  
    I2C_writedata(0x80|0x0f,PDRIVE | PDIODE | PGAIN | AGAIN);

  
  
  unsigned char WEN, PEN, AEN, PON; 
  WEN = 8; // Enable Wait 
  PEN = 4; // Enable Prox 
  AEN = 2; // Enable ALS 
  PON = 1; // Enable Power On 
  
  I2C_writedata(0x80|0x00,WEN | PEN | AEN | PON);

  MAP_UtilsDelay(400000); //Wait for 12 ms 

}

unsigned int apds_readProximity(void)
{
  unsigned int data;
  data=I2C_readword(0xa0|0x18);
  return data;
}
unsigned int apds_readALS(unsigned char ch)
{
  unsigned int data;
  data=I2C_readword(0xa0|ch);
  return data;
}
//*****************************************************************************
//
//! Main function handling the I2C example
//!
//! \param  None
//!
//! \return None
//! 
//*****************************************************************************
void main()
{
    
     // variables
    unsigned char  ID = 0;   

    //
    // Initialize board configurations
    //
    BoardInit();
    
    //
    // Configure the pinmux settings for the peripherals exercised
    //
    PinMuxConfig();
            
    //
    // Configuring UART
    //
    InitTerm();
    
    //
    // I2C Init
    //
    I2C_IF_Open(I2C_MASTER_MODE_FST);
    
    //
    // Display the banner followed by the usage description
    //
    DisplayBanner(APP_NAME);
    
    UART_PRINT("APDS_9930-TEST \n");
    
    // clear the interrupt
    I2C_writecom(0xe7);
    
    ID=I2C_readdata(0x80|0x12);
    UART_PRINT("ID: %d \n",ID);
    
   // Initialize APDS-9960 (configure I2C and initial values)
   apds_init();
  
//  // Adjust the Proximity sensor gain
//  apds_setProximityGain();
//  
//  // Start running the APDS-9960 proximity sensor (no interrupts)
//  apds_enableProximitySensor(false);
    
    while(FOREVER)
    {

      
      //status
        unsigned char status;
        status=I2C_readdata(0x80|0x13);
        UART_PRINT("status: %d \n",status);
       
      // Read the ALS value  
//       unsigned int ch0,ch1; 
//       ch0=apds_readALS(0x14);
//       ch1=apds_readALS(0x16);
//       UART_PRINT("ch0data: %d \n",ch0);
//       UART_PRINT("ch1data: %d \n",ch1); 
      
        unsigned int a,b,c,d,e,f;
        a=I2C_readdata(0x80|0x14);
        b=I2C_readdata(0x80|0x15);
        c=I2C_readdata(0x80|0x16);
        d=I2C_readdata(0x80|0x17);
        e=b<<8 | a;
        f=d<<8 | c;
        UART_PRINT("ch0data: %d \n",e);
        UART_PRINT("ch1data: %d \n",f); 
        
      // Read the proximity value
        unsigned int t1,t2,t3;
        //    unsigned int   proximity_data=0;
        t1=I2C_readdata(0x80|0x18);t2=I2C_readdata(0x80|0x19);
        t3=t2<<8 | t1;  
        UART_PRINT("proximity_data: %d \n",t3);
       //UART_PRINT("proximity_data: %d \n",proximity_data);
       
                 // clear the interrupt
    I2C_writecom(0xe5);

  
  // Wait 250 ms before next reading
  MAP_UtilsDelay(20000000);

      
    }
}
