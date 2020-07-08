#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>//为了让函数能够接收可变参数
#include <stdlib.h>
//#include <malloc.h>

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
#include "LCD_display.h"


#define MAX_MSG_LEN             256             // generic message length
#define MAX_FONT_BUFFER_LEN     32

bool is_fontchip_ok = false;

int IntToStr(int uiInteger,char *cStr){
    int ilen = 0,i;
    char ctemp;

    *cStr = uiInteger % 10;
    while(uiInteger){
        *(cStr + ilen) = uiInteger % 10 + '0';
        uiInteger = uiInteger / 10;
        ilen ++;
    }

    for(i=0;i<ilen/2;i++){
        ctemp = cStr[i];
        cStr[i] = cStr[ilen - i-1];
        cStr[ilen - i-1] = ctemp;
    }
    return ilen;
}

void lcd_init(void)
{
#define TEST_GB_CODE    0xBBB6  // '欢'

#ifdef USE_FONT_CHIP
    uint8_t font_nfo;
    uint8_t font_buffer[MAX_FONT_BUFFER_LEN] = { 0 };
#endif // USE_FONT_CHIP

    // init lcd module
    LCD_ILI9341_TFT_Init();

#ifdef USE_FONT_CHIP
    // init font module
    GT20Init();
    // test if font chip works
    GT20Read(GB2312_15x16, TEST_GB_CODE, &font_nfo, &font_nfo, (uint16_t *)font_buffer);
    // let's test 3 bytes
    if ((font_buffer[0] | font_buffer[MAX_FONT_BUFFER_LEN/4] | font_buffer[MAX_FONT_BUFFER_LEN/2]) > 0)
    {
        is_fontchip_ok = true;
//        LCD_TFT_ShowString(20,20,"FONT_OK",2,Black,White);

    }
    else
    {
        //lcd_printf(0, 0, "FONT CHIP ERR");
//        LCD_TFT_ShowString(20,20,"ERROR",2,Black,White);
        while(1);
    }
#endif // USE_FONT_CHIP
}

void lcd_clear(void)
{
	LCD_ILI9341_TFT_cls(White);
}

void LCD_ILI9341_TFT_fontpixel(unsigned int x, unsigned int y, uint16_t color) {
    unsigned char color_h;
    unsigned char color_l;
    color_h = (color >> 0x08);
    color_l = (color & 0xff);

  if ((x >= LCD_ILI9341_TFT_width()) || (y >= LCD_ILI9341_TFT_height())) return;

  LCD_ILI9341_TFT_window(y,x,y+1,x+1);

  // setup for data//   _rs = 1;
	LCD_CD_HIGH();

//  _cs = 0;
	LCD_CS_LOW();

	LCD_SPI_writebyte(color_h);
	LCD_SPI_writebyte(color_l);

//  _cs = 1;
	LCD_CS_HIGH();

}
//ASCII_8x16
void LCD_font2char(uint16_t x,uint16_t y,uint8_t font_column,uint8_t font_row,uint8_t *fontbuffer,uint16_t pointColor,uint16_t backColor)
{
    int i,j;

    unsigned char uctemp;

	uint16_t xBuf = 0;
	xBuf = x;  //备份x 坐标值

    for(i=0;i<font_row/2;i++)
    {
        uctemp = fontbuffer[i];
        for(j=0;j<font_column;j++)
        {

            if(uctemp & 0x01)
            	{LCD_ILI9341_TFT_fontpixel(x, y, Black);}
            else
            	{LCD_ILI9341_TFT_fontpixel(x, y, White);}
            uctemp >>= 1;
            x++;
        }
        x = xBuf;
        y++;
    }

    for(i=font_row/2;i<font_row;i++)
    {
        uctemp = fontbuffer[i];
        for(j=0;j<font_column;j++)
        {
            if(uctemp & 0x01)
            	{LCD_ILI9341_TFT_fontpixel(x+font_row/2, y-font_column, Black);}
            else
            	{LCD_ILI9341_TFT_fontpixel(x+font_row/2, y-font_column, White);}
            uctemp >>= 1;
            x++;
        }
        x = xBuf;
        y++;
    }

}

//
//GT20 FONT format
//eg:ASCII_8x16 mean 8 col 16 row
//row/8 mean byte numbers in a col,eg:16 row is 2 bytes in a col;
//
//
void LCD_AsciiDisplay(unsigned int x, unsigned int y,unsigned char ucAsciiWord)
{
    int t;

    uint8_t font_row,font_column;
    uint16_t fontBuffer[50];
    uint8_t font_buffer[50];

    GT20Read(ASCII_8x16, ucAsciiWord, &font_column, &font_row, fontBuffer);//8 column 16 row

    for(t=0;t<font_row;t++)
    {
    	font_buffer[t*2]=fontBuffer[t] & 0xff;
    	font_buffer[t*2+1]=(fontBuffer[t]>>8) & 0xff;
    }

    LCD_font2char(y,x,font_column,font_row,font_buffer,Black,White);

}

//GB2312_15x16
void LCD_font2hanzi(uint16_t x,uint16_t y,uint8_t font_column,uint8_t font_row,uint8_t *fontbuffer,uint16_t pointColor,uint16_t backColor)
{

    int i,j;

    unsigned char uctemp;

	uint16_t xBuf = 0;
	xBuf = x;  //备份x 坐标值

    for(i=0;i<font_row;i++)
    {
        uctemp = fontbuffer[i];
        for(j=0;j<font_column;j++)
        {

            if(uctemp & 0x01)
            	{LCD_ILI9341_TFT_fontpixel(x, y, Black);}
            else
            	{LCD_ILI9341_TFT_fontpixel(x, y, White);}
            uctemp >>= 1;
            x++;
        }
        x = xBuf;
        y++;
    }

    for(i=font_row;i<font_row*2;i++)
    {
        uctemp = fontbuffer[i];
        for(j=0;j<font_column;j++)
        {
            if(uctemp & 0x01)
            	{LCD_ILI9341_TFT_fontpixel(x+font_row/2, y-font_column, Black);}
            else
            	{LCD_ILI9341_TFT_fontpixel(x+font_row/2, y-font_column, White);}
            uctemp >>= 1;
            x++;
        }
        x = xBuf;
        y++;
    }

}

void LCD_ChineseDisplay(unsigned int x, unsigned int y,uint16_t ucHanZi)
{
    int t;

    uint8_t font_row,font_column;
    uint16_t fontBuffer[50];
    uint8_t font_buffer[50];

    GT20Read(GB2312_15x16, ucHanZi, &font_column, &font_row, fontBuffer);//15 column 16 row

    for(t=0;t<font_row;t++)
    {
    	font_buffer[t*2]=fontBuffer[t] & 0xff;
    	font_buffer[t*2+1]=(fontBuffer[t]>>8) & 0xff;
    }

    LCD_font2hanzi(y,x,font_column,font_row,font_buffer,Black,White);
}

void LCD_FontDisplay(unsigned int x, unsigned int y,uint16_t ucFont)
{
    if(ucFont > 127)  //why 127 for the ascii is 0~127
    	LCD_ChineseDisplay(x,y,ucFont);
    else
    	LCD_AsciiDisplay(x,y,ucFont);
}

void LCD_StringDisplay(unsigned int x, unsigned int y,char *Str)
{
    int i=0;
    uint16_t unicode;

    while(*Str > 0)
    {
        if(*Str < 128)
        {
            unicode = *Str & 0x00ff;
            Str ++;
            LCD_FontDisplay(x+i*8,y,unicode);//ASCII_8x16
            i ++;
        }
        else
        {
            unicode = ((*(Str) << 8)& 0xff00) | (*(Str+1) & 0x00ff);
            Str += 2;
            LCD_FontDisplay(x+i*8,y,unicode);//GB2312_15x16
            i += 2;
        }

    }
}



void LCD_FloatDisplay(unsigned int uiCol, unsigned int uiRow,float Str){
    int i,t=0,ilen;
    char c[50];
    uint16_t unicode;

    unsigned int uiInteger,uiDecimal;
    //整数部分处理
    uiInteger = (int)Str;
    ilen = IntToStr(uiInteger,c);
    //小数部分处理
    uiDecimal = (int)(Str * PRECISION) % PRECISION;

    c[ilen] = '.';
    c[ilen+1] = (unsigned char)((uiDecimal) & (0x0000ff))+'0';
    i = 0;
    while(t < ilen+2){
        unicode = c[t] & 0x00ff;
        LCD_FontDisplay(uiCol+i*8,uiRow,unicode);
        i ++;
        t ++;
    }
}








////按照格式输出函数
//int lcd_printf(unsigned int x, unsigned int y,char *pcFormat, ...){
//    int iLen;
//    char *pcBuffer,*pcTemp;
//    int iSize = 256;
//    va_list list;
//
//    pcBuffer = (char*)malloc(iSize);
//    if(pcBuffer == NULL)
//    {
//        return -1;
//    }
//    while(1){
//        va_start(list,pcFormat);
//        iLen = vsnprintf(pcBuffer,iSize,pcFormat,list);
//        va_end(list);
//
//        if(iLen > -1 && iLen < iSize){
//            break;
//        }
//        else{
//            iSize*=2;
//            if((pcTemp=(char*)realloc(pcBuffer,iSize))==NULL)
//            {
//            	LCD_StringDisplay(0,0,"Could not reallocate memory\n\r");
//                iLen = -1;
//                break;
//            }
//            else
//            {
//                pcBuffer=pcTemp;
//            }
//        }
//    }
//    LCD_StringDisplay(x,y,pcBuffer);
//    free(pcBuffer);
//
//    return iLen;
//}
