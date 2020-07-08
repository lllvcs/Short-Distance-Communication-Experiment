
#ifndef LAB_LCDKEY_LCD_DISPLAY_H_
#define LAB_LCDKEY_LCD_DISPLAY_H_

#include "LCD_TFT_ILI9341.h"
#include "Font_lib.h"
//#include "Hal_board.h"

#define USE_FONT_CHIP
#define PRECISION               10

extern void lcd_init(void);

extern void lcd_clear(void);

extern void LCD_ILI9341_TFT_fontpixel(unsigned int x,unsigned int y, uint16_t color);

extern void LCD_font2char(uint16_t x,uint16_t y,uint8_t font_column,uint8_t font_row,uint8_t *fontbuffer,uint16_t pointColor,uint16_t backColor);

extern void LCD_AsciiDisplay(unsigned int x, unsigned int y,unsigned char ucAsciiWord);

extern void LCD_font2hanzi(uint16_t x,uint16_t y,uint8_t font_column,uint8_t font_row,uint8_t *fontbuffer,uint16_t pointColor,uint16_t backColor);

extern void LCD_ChineseDisplay(unsigned int x, unsigned int y,uint16_t ucHanZi);

extern void LCD_FontDisplay(unsigned int x, unsigned int y,uint16_t ucFont);

extern void LCD_StringDisplay(unsigned int x, unsigned int y,char *Str);

extern void LCD_FloatDisplay(unsigned int uiCol, unsigned int uiRow,float Str);

extern int lcd_printf(unsigned int x, unsigned int y,char *pcFormat, ...);
//extern void

#endif /* LAB_LCDKEY_LCD_DISPLAY_H_ */
