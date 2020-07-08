// Standard includes
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_gpio.h"
#include "hw_ints.h"
#include "spi.h"
#include "rom.h"
#include "gpio.h"
#include "pin.h"
#include "rom_map.h"
#include "utils.h"
#include "prcm.h"
#include "uart.h"
#include "interrupt.h"

// Common interface includes
#include "uart_if.h"
#include "pinmux.h"
#include "Font_lib.h"


FontSize fontSize[] = {
    { GB2312_15x16          , 16, 16 }, // Hanzi
    { ASCII_7x8             ,  7,  8 }, // 'd be square
    { GB2312_8x16           ,  8, 16 },
    { ASCII_8x16            ,  8, 16 }, // Alphabet
    { ASCII_5x7             ,  5,  7 },
    { ASCII_Arial           ,  0,  0 },
    { ASCII_8x16_Bold       ,  8, 16 },
    { ASCII_TimesNewRoman   ,  0,  0 },
};

uint32_t GT20GetFontAddr(FontType fontType, uint16_t fontCode)
{
    uint32_t fontAddr, baseAddr;
    uint16_t addrMSB, addrLSB;

    addrMSB = (fontCode >> 8) & 0xFF;
    addrLSB = fontCode & 0xFF;

    switch (fontType)
    {
    case GB2312_15x16:
        baseAddr = 0;
        if ((addrMSB == 0xA9) && (addrLSB >= 0xA1))
            fontAddr = (282 + (addrLSB - 0xA1 ))*32 + baseAddr;
        else if ((addrMSB >= 0xA1) && (addrMSB <= 0xA3) && (addrLSB >= 0xA1))
            fontAddr =((addrMSB - 0xA1)*94 + (addrLSB - 0xA1))*32 + baseAddr;
        else if ((addrMSB >= 0xB0) && (addrMSB <= 0xF7) && (addrLSB >= 0xA1))
            fontAddr = ((addrMSB - 0xB0)*94 + (addrLSB - 0xA1) + 846)*32 + baseAddr;
        break;
    case ASCII_7x8:
        baseAddr = 0x66c0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20 )*8 + baseAddr;
        break;
    case GB2312_8x16:
        baseAddr = 0x3B7D0;
        if ((fontCode >= 0xAAA1) && (fontCode <= 0xAAFE))
            fontAddr = (fontCode - 0xAAA1 )*16 + baseAddr;
        else if((fontCode >= 0xABA1) && (fontCode <= 0xABC0))
            fontAddr = (fontCode - 0xABA1 + 95)*16 + baseAddr;
        break;
    case ASCII_8x16:
        baseAddr = 0x3B7C0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*16 + baseAddr;
        break;
    case ASCII_5x7:
        baseAddr = 0x3BFC0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*8 + baseAddr;
        break;
    case ASCII_Arial:
        baseAddr = 0x3C2C0;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*34 + baseAddr;
        break;
    case ASCII_8x16_Bold:
        baseAddr = 0x3CF80;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20 )*16 + baseAddr;
        break;
    case ASCII_TimesNewRoman:
        baseAddr = 0x3D580;
        if ((fontCode >= 0x20) && (fontCode <= 0x7E))
            fontAddr = (fontCode - 0x20)*34 + baseAddr;
        break;
    }
    return fontAddr;
}

void FONT_SPI_writebyte(uint8_t data)
{
    unsigned long ulDummy;
    // USCI_B0 TX buffer ready?


    // Transmit data
    SPIDataPut(GSPI_BASE,data);

    // Wait for all TX/RX to finish


    // Dummy read to empty RX buffer and clear any overrun conditions
    SPIDataGet(GSPI_BASE,&ulDummy);

}

uint8_t LCD_SPI_readbyte(void)
{
	uint8_t data;
    unsigned long ulDummy;
    // USCI_B0 TX buffer ready?


    // Transmit data
    SPIDataPut(GSPI_BASE,0xff);

    // Wait for all TX/RX to finish

    // Dummy read to empty RX buffer and clear any overrun conditions
    SPIDataGet(GSPI_BASE,&ulDummy);
    data=ulDummy&0xff;

    return data;
}


void GT20Init(void)
{

    // Configure FONT_CS_PIN as output. CS is active high
	// disable chip select
	GPIOPinWrite(FONT_CS_PORT, FONT_CS_PIN, FONT_CS_PIN);

}

void GT20Read(FontType fontType, uint16_t unicode, uint8_t *font_column, uint8_t *font_row, uint16_t *fontBuffer)
{
    uint32_t spiWriteBuffer, spiReadBuffer;

//    uint16_t fontCode;

    uint32_t fontAddr;

    uint16_t fontBufferLen;


    // 1. write instruction to chip
	GPIOPinWrite(FONT_CS_PORT, FONT_CS_PIN, ~FONT_CS_PIN);


    // may be some conversion from unicode<=>fontcode
//    fontCode = ((unicode >> 8) & 0xFF) | ((unicode & 0xFF) << 8);

    fontAddr = GT20GetFontAddr(fontType, unicode);

    fontBufferLen = (fontSize[fontType].font_row*fontSize[fontType].font_column)/16;  // read by 16 bit


    spiWriteBuffer = GT20_READ;
//    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    FONT_SPI_writebyte(spiWriteBuffer);

    spiWriteBuffer = ((fontAddr >> 16) & 0xFF);
//    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    FONT_SPI_writebyte(spiWriteBuffer);

    spiWriteBuffer = ((fontAddr >> 8) & 0xFF);
//    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    FONT_SPI_writebyte(spiWriteBuffer);

    spiWriteBuffer = (fontAddr & 0xFF);
//    MAP_SSIDataPut(FONT_PIN_SPI_PORT, spiWriteBuffer);
    FONT_SPI_writebyte(spiWriteBuffer);

    while (SPIDataGetNonBlocking(GSPI_BASE, &spiReadBuffer));


    // 2. wait for data
    while (fontBufferLen --)
    {

       //SSIDataGet(FONT_PIN_SPI_PORT, &spiReadBuffer);
        spiReadBuffer=LCD_SPI_readbyte();
        *fontBuffer = spiReadBuffer & 0xFF;

       //SSIDataGet(FONT_PIN_SPI_PORT, &spiReadBuffer);
        spiReadBuffer=LCD_SPI_readbyte();
        *fontBuffer = ((*fontBuffer) | ((spiReadBuffer & 0xFF) << 8));

        fontBuffer ++;
    }


    //MAP_GPIOPinWrite(FONT_GPIO_SPI_CS, FONT_PIN_SPI_CS, FONT_PIN_SPI_CS);   // CS
	GPIOPinWrite(FONT_CS_PORT, FONT_CS_PIN, FONT_CS_PIN);



    *font_column = fontSize[fontType].font_column;
    *font_row  = fontSize[fontType].font_row;

}

