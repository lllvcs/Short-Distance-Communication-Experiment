//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHT2x Sample Code (V1.2)
// File      :  SHT2x.c
// Author    :  MST
// Controller:  NEC V850/SG3 (uPD70F3740)
// Compiler  :  IAR compiler for V850 (3.50A)
// Brief     :  Sensor layer. Functions for sensor access
//==============================================================================

// Standard includes
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "hwspinlock.h"
#include "i2c.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"

// Common interface include
#include "i2c_if.h"
#define I2C_BASE                I2CA0_BASE
#include "TMP75.h"

        unsigned char  Rotar;
        float Factor;
        unsigned short Mascara;
#define Address 0x48
void vSetConfigurationTMP175(unsigned char Config){
   
//   _Address=Address<<1;
   switch(Config&0x60){
      case 0x00:
         Factor=0.5;
         Mascara=0x00FF;
         Rotar=3;
      break;
      case 0x20:
         Factor=0.25;
         Mascara=0x01FF;
         Rotar=2;
      break;
      case 0x40:
         Factor=0.125;
         Mascara=0x03FF;
         Rotar=1;
      break;
      case 0x60:
         Factor=0.0625;
         Mascara=0x07FF;
         Rotar=0;
      break;
   }
   unsigned char data[2];
   data[0]=0x01;
   data[1]=Config;
   I2C_IF_Write(Address,data,2,1);

}
 
void vSetTemperatureLowTMP175(float Value){
   unsigned short Temp;
   
   if(Value>=0.0){
      Temp=Value/0.0625;
   }else{
      Temp=(Value+128.0)/0.0625;
      Temp!=0x0800;
   }
   Temp<<=4;
   unsigned char data[2];
   data[0]=*((unsigned char *)&Temp+1);
   data[1]=*((unsigned char *)&Temp);
   I2C_IF_Write(Address,data,2,1); 

}
 
float fReadTemperatureLowTMP175(void){
   unsigned short Temp;
   float Cal;
   
   unsigned char data[1];
   data[0]=0x02;
   I2C_IF_Write(Address,data,1,0); 
   unsigned char RxData[2]={0,0};
   I2C_IF_Read(Address,RxData,2);
   *((unsigned char *)&Temp+1)=RxData[1];
   *((unsigned char *)&Temp)=RxData[0];

   
   Temp>>=4;
   Cal=Temp*0.0625;
   if((0x0800&Temp)!=0x0000){
      Cal-=128.0;
   }
   
   return(Cal);
}
 
void vSetTemperatureHighTMP175(float Value){
   unsigned short Temp;
   
   if(Value>=0.0){
      Temp=Value/0.0625;
   }else{
      Temp=(Value+128.0)/0.0625;
      Temp!=0x0800;
   }
   Temp<<=4;
   
   unsigned char data[3];
   data[0]=0x03;
   data[1]=*((unsigned char *)&Temp+1);
   data[2]=*((unsigned char *)&Temp);
   I2C_IF_Write(Address,data,3,1); 

}
 
float fReadTemperatureHighTMP175(void){
   unsigned short Temp;
   float Cal;
   
   unsigned char data[1];
   data[0]=0x03;
   I2C_IF_Write(Address,data,1,0);
   unsigned char RxData[2]={0,0};
   I2C_IF_Read(Address,RxData,2);
   *((unsigned char *)&Temp+1)=RxData[1];
   *((unsigned char *)&Temp)=RxData[0];
   
   Temp>>=4;
   Cal=Temp*0.0625;
   if((0x0800&Temp)!=0x0000){
      Cal-=128.0;
   }
   
   return(Cal);
}
 
float fReadTemperatureTMP175(void){
   unsigned short Temp;
   float Cal;
   

   unsigned char data[1];
   data[0]=0x00;
   I2C_IF_Write(Address,data,1,0);
   unsigned char RxData[2]={0,0};
   I2C_IF_Read(Address,RxData,2);
   *((unsigned char *)&Temp+1)=RxData[1];
   *((unsigned char *)&Temp)=RxData[0];
   
   Temp>>=(Rotar+4);
   Cal=((float)Factor*(Temp&Mascara));
   if(((~Mascara)&Temp)!=0x0000){
      Cal-=128.0;
   }
   
   return(Cal);
}
 
// Only SHUTDOWN MODE 
void vStartSingleConversionTMP175(void){
   unsigned char Temp;
 

     unsigned char data[1];
     data[0]=0x01;
     I2C_IF_Write(Address,data,1,0);
     unsigned char RxData[1]={0};
     I2C_IF_Read(Address,RxData,1);
     Temp=RxData[0];
   
     unsigned char txdata[2];
     txdata[0]=0x01;
     txdata[1]=Temp|0x80;
     I2C_IF_Write(Address,txdata,2,1);

   
}
void TMP75int(void)
{
  unsigned char txdata[2];
  txdata[0]=0x01;
  txdata[1]=0x60;
  I2C_IF_Write(Address,txdata,2,1);
  
  unsigned char data[1];
  data[0]=0x00;
  I2C_IF_Write(Address,data,1,1);  

}

float readtemp(void)
{
  unsigned char RxData[2];
  I2C_IF_Read(Address,RxData,2);
  int TempSum = (((RxData[0] << 8) | RxData[1]) >> 4);
  
  float temp = (TempSum*0.0625);

  return temp;                           // Return the temperature value


}

