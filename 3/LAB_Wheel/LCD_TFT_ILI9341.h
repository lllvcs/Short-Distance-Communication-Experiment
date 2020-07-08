#ifndef LAB_LCDKEY_LCD_TFT_ILI9341_H_
#define LAB_LCDKEY_LCD_TFT_ILI9341_H_

// Standard includes
#include <string.h>
#include <stdio.h>
#include <stdint.h>
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


#define LCD_CS_PORT             GPIOA0_BASE
#define LCD_CS_PIN              GPIO_PIN_7

#define LCD_CD_PORT             GPIOA0_BASE
#define LCD_CD_PIN              GPIO_PIN_6

/**
  * @brief  LCD SPI Interface pins
  */


#define RGB(r,g,b)  (((r&0xF8)<<8)|((g&0xFC)<<3)|((b&0xF8)>>3)) //5 red | 6 green | 5 blue

///*define ILI9341 Commands */
//
//#define ILI9341_NOP 0x0
//#define ILI9341_SWRESET 0x01
//#define ILI9341_RDDID 0x04
//#define ILI9341_RDDST 0x09
//
//#define ILI9341_SLPIN  0x10
//#define ILI9341_SLPOUT  0x11
//#define ILI9341_PTLON  0x12
//#define ILI9341_NORON  0x13
//
//#define ILI9341_INVOFF 0x20
//#define ILI9341_INVON 0x21
//#define ILI9341_DISPOFF 0x28
//#define ILI9341_DISPON 0x29
//#define ILI9341_CASET 0x2A
//#define ILI9341_RASET 0x2B
//#define ILI9341_RAMWR 0x2C
//#define ILI9341_RAMRD 0x2E
//
//#define ILI9341_COLMOD 0x3A
//#define ILI9341_MADCTL 0x36
//
//
//#define ILI9341_FRMCTR1 0xB1
//#define ILI9341_FRMCTR2 0xB2
//#define ILI9341_FRMCTR3 0xB3
//#define ILI9341_INVCTR 0xB4
//#define ILI9341_DISSET5 0xB6
//
//#define ILI9341_PWCTR1 0xC0
//#define ILI9341_PWCTR2 0xC1
//#define ILI9341_PWCTR3 0xC2
//#define ILI9341_PWCTR4 0xC3
//#define ILI9341_PWCTR5 0xC4
//#define ILI9341_VMCTR1 0xC5
//
//#define ILI9341_RDID1 0xDA
//#define ILI9341_RDID2 0xDB
//#define ILI9341_RDID3 0xDC
//#define ILI9341_RDID4 0xDD
//
//#define ILI9341_PWCTR6 0xFC
//
//#define ILI9341_GMCTRP1 0xE0
//#define ILI9341_GMCTRN1 0xE1

/* Colors */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */

#define BPP         16                  // Bits per pixel

#define  orientation       1


//class ILI9341_TFT : public GraphicsDisplay {
// public:

extern void wait_ms(long i);
extern void LCD_CS_LOW(void);
extern void LCD_CS_HIGH(void);
extern void LCD_CD_LOW(void);
extern void LCD_CD_HIGH(void);

/** Create a ILI9341_TFT object connected to SPI and three pins. ILI9341 requires rs pin to toggle between data/command
 *
 * @param mosi,miso,sclk SPI
 * @param cs pin connected to CS of display (called SS for 'Slave Select' in ILI9341 datasheet)
 * @param rs pin connected to RS of display (called D/CX in ILI9341 datasheet)
 * @param reset pin connected to RESET of display
 *
 */
extern void LCD_PIN_Init(void);

extern void LCD_SPI_writebyte(uint8_t data);

/** Get the width of the screen in pixel
 *
 * @param
 * @returns width of screen in pixel
 *
 */
extern int LCD_ILI9341_TFT_width(void);

/** Get the height of the screen in pixel
 *
 * @returns height of screen in pixel
 *
 */
extern int LCD_ILI9341_TFT_height(void);

/** Write data to the LCD controller
*
* @param dat data written to LCD controller
*
*/
extern void LCD_ILI9341_TFT_wr_dat(int value);

/** Write a command the LCD controller
*
* @param cmd: command to be written
*
*/
extern void LCD_ILI9341_TFT_wr_cmd(int value);

/** Start data sequence to the LCD controller
*
*/
extern void LCD_ILI9341_TFT_wr_dat_start(void);

/** Stop of data writing to the LCD controller
*
*/
extern void LCD_ILI9341_TFT_wr_dat_stop(void);

///** write data to the LCD controller
//*
//* @param data to be written
//*                                           *
//*/
//extern void wr_dat_only(unsigned short dat);


/** Write a value to the to a LCD register
*
* @param reg register to be written
* @param val data to be written
*/
extern void LCD_ILI9341_TFT_wr_reg (unsigned char reg, unsigned short val);

/** Set the orientation of the screen
 *  x,y: 0,0 is always top left
 *
 * @param o direction to use the screen (0-3) 90&#65533; Steps
 *
 */
extern void LCD_ILI9341_TFT_set_orientation(void);

/** Set draw window region
 *
 * @param x horizontal position
 * @param y vertical position
 * @param w window width in pixel
 * @param h window height in pixels
 */
extern void LCD_ILI9341_TFT_window (unsigned int x, unsigned int y, unsigned int w, unsigned int h);

/** Set draw window region to whole screen
 *
 */
extern void LCD_ILI9341_TFT_WindowMax (void);


/** Init the ILI9341 controller
 *
 */
extern  void LCD_ILI9341_TFT_tft_reset(void);

extern void LCD_ILI9341_TFT_Init(void);

  /** Draw a pixel at x,y with color
   *
   * @param x horizontal position
   * @param y vertical position
   * @param color 16 bit pixel color
   */
extern void LCD_ILI9341_TFT_pixel(int x, int y, uint16_t colour);

/** Fill the screen with _backgroun color
 *
 */
extern void LCD_ILI9341_TFT_cls (uint16_t colour);

  /** draw a circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   */

extern  void LCD_ILI9341_TFT_circle(int x, int y, int r, uint16_t colour);

  /** draw a filled circle
   *
   * @param x0,y0 center
   * @param r radius
   * @param color 16 bit color                                                                 *
   *
   * use circle with different radius,
   * can miss some pixel
   */
extern  void LCD_ILI9341_TFT_fillcircle(int x, int y, int r, uint16_t colour);

/** draw a horizontal line
 *
 * @param x0 horizontal start
 * @param x1 horizontal stop
 * @param y vertical position
 * @param color 16 bit color
 *
 */
extern void LCD_ILI9341_TFT_hline(int x0, int x1, int y, uint16_t colour);

/** draw a vertical line
 *
 * @param x horizontal position
 * @param y0 vertical start
 * @param y1 vertical stop
 * @param color 16 bit color
 */
extern void LCD_ILI9341_TFT_vline(int y0, int y1, int x, uint16_t colour);

  /** draw a 1 pixel line
   *
   * @param x0,y0 start point
   * @param x1,y1 stop point
   * @param color 16 bit color
   *
   */
extern  void LCD_ILI9341_TFT_line(int x0, int y0, int x1, int y1, uint16_t colour);

  /** draw a rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *                                                   *
   */
extern  void LCD_ILI9341_TFT_rect(int x0, int y0, int x1, int y1, uint16_t colour);

  /** draw a filled rect
   *
   * @param x0,y0 top left corner
   * @param x1,y1 down right corner
   * @param color 16 bit color
   *
   */
extern  void LCD_ILI9341_TFT_fillrect(int x0, int y0, int x1, int y1, uint16_t colour);

  /** setup cursor position
   *
   * @param x x-position (top left)
   * @param y y-position
   */
extern void LCD_ILI9341_TFT_locate(int x, int y);

extern void LCD_ILI9341_TFT_foreground(uint16_t colour);

extern void LCD_ILI9341_TFT_background(uint16_t colour);


  /** calculate the max number of char in a line
   *
   * @returns max columns
   * depends on actual font size
   *
   */
extern int LCD_ILI9341_TFT_columns(void);

  /** calculate the max number of columns
   *
   * @returns max column
   * depends on actual font size
   *
   */
extern int LCD_ILI9341_TFT_rows(void);

extern void LCD_TFT_ShowString(uint16_t column,uint16_t line,uint8_t *ArrayPoint,uint8_t Font,uint16_t pointColor,uint16_t backColor); //显示一个字符( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT"

void LCD_TFT_ShowChar(uint16_t line,uint16_t column,uint8_t AscNum,uint8_t Font,uint16_t pointColor,uint16_t backColor); //显示字符串( 指定坐标,字体大小,颜色)-"彩屏LCD_TFT"

extern void LCD_Show_ChFont2424(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor);

extern void LCD_Show_StandbyPage(void);
  /** draw a character on given position out of the active font to the TFT
   *
   * @param x x-position of char (top left)
   * @param y y-position
   * @param c char to print
   *
   */
//extern void LCD_ILI9341_TFT_character(int x, int y, int c);

  /** put a char on the screen
   *
   * @param value char to print
   * @returns printed char
   *
   */
//  int _putc(int value);

//extern unsigned char ToOrd(unsigned char ch);
//
//extern void  DispOneChar(unsigned char ord,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);
//
//extern void DispStr(unsigned char *str,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);
//
//extern void DispInt(unsigned int i,unsigned int Xstart,unsigned int Ystart,unsigned int TextColor,unsigned int BackColor);

#endif /* LAB_LCDKEY_LCD_TFT_ILI9341_H_ */
