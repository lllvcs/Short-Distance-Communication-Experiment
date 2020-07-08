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


#include "LCD_TFT_ILI9341.h"
#include "LCD_TFT_ILI9341_FONT.h"
#include "Font_lib.h"


////////////////////////////////////////////////////////////////////////////
//--**���������С**------------------------------------------------//
#define FONT1206   (1) //����12*6
#define FONT1608   (2) //����16*8

#define ROW  240	//Y	  //��ʾ������
#define COL  320    //X    //��ʾ������


#define M_Space_ASCII      (0x20) //"�ո�"��ASCII��

#define M_Vertical_FONT (16) //����"��ֱ���"   //������ʾ"�о�"
#define M_Horizontal_FONT (8) //����"ˮƽ���"   //������ʾ"�ַ����"


unsigned int char_x;
unsigned int char_y;

uint16_t _foreground;
uint16_t _background;

unsigned char* font;

void LCD_CS_LOW(void){
	 GPIOPinWrite(LCD_CS_PORT, LCD_CS_PIN, ~LCD_CS_PIN);
}

void LCD_CS_HIGH(void){
	 GPIOPinWrite(LCD_CS_PORT, LCD_CS_PIN, LCD_CS_PIN);
}

void LCD_CD_LOW(void){
	 GPIOPinWrite(LCD_CD_PORT, LCD_CD_PIN, ~LCD_CD_PIN);
}

void LCD_CD_HIGH(void){
	 GPIOPinWrite(LCD_CD_PORT, LCD_CD_PIN, LCD_CD_PIN);
}


void LCD_PIN_Init(void)
{
    //
    // Configure the pins that connect to the LCD as outputs.
    //

    // Configure LCD_CS_PIN as output. CS is active low
	 GPIOPinWrite(LCD_CS_PORT, LCD_CS_PIN, ~LCD_CS_PIN);

    // Configure LCD_CD_PIN as output and clear output to set it in command
    // mode
	 GPIOPinWrite(LCD_CD_PORT, LCD_CD_PIN, ~LCD_CD_PIN);

    // Configure LCD_SIMO_PIN


    // Configure LCD_CLK_PIN  option select CLK


    // Configure SD_SOMI_PIN


	  //
	  // Reset the peripheral
	  //
	  MAP_PRCMPeripheralReset(PRCM_GSPI);

	  SPIReset(GSPI_BASE);
	  SPIConfigSetExpClk(GSPI_BASE,PRCMPeripheralClockGet(PRCM_GSPI),10000000, SPI_MODE_MASTER,SPI_SUB_MODE_0,
	  (SPI_HW_CTRL_CS | SPI_3PIN_MODE | SPI_TURBO_OFF | SPI_WL_8));
	  MAP_SPIEnable(GSPI_BASE);
}


/***************************************************************************//**
 * @brief   Write one byte via 3 wire SPI
 * @param   the byte prepared to write
 * @return  None
 ******************************************************************************/

void LCD_SPI_writebyte(uint8_t data)
{
    unsigned long ulDummy;

    // TX buffer ready?


    // Transmit data
    SPIDataPut(GSPI_BASE,data);

    // Wait for all TX/RX to finish


    // Dummy read to empty RX buffer and clear any overrun conditions
    SPIDataGet(GSPI_BASE,&ulDummy);

}


int LCD_ILI9341_TFT_width(void) {
    if (orientation == 0 || orientation == 2) return 240;
    else return 320;
}


int LCD_ILI9341_TFT_height(void) {
    if (orientation == 0 || orientation == 2) return 320;
    else return 240;
}

void LCD_ILI9341_TFT_wr_cmd(int cmd)
{
  //	_cs = 0;
	LCD_CS_LOW();

  //    _rs = 0; // rs low, cs low for transmitting command
	LCD_CD_LOW();

	LCD_SPI_writebyte(cmd);

//	_cs = 1;
	LCD_CS_HIGH();
}



void LCD_ILI9341_TFT_wr_dat(int dat)
{
//	_cs = 0;
	LCD_CS_LOW();

//    _rs = 1; // rs high, cs low for transmitting data
	LCD_CD_HIGH();

	LCD_SPI_writebyte(dat);

//	_cs = 1;
	LCD_CS_HIGH();
}



void LCD_ILI9341_TFT_wr_dat_start(void) {
//    _rs = 1; //  rs high, cs low for transmitting data
	LCD_CD_HIGH();

//	_cs = 0;
	LCD_CS_LOW();
}



void LCD_ILI9341_TFT_wr_dat_stop (void) {
//    _cs = 1;
	LCD_CS_HIGH();
}


void LCD_ILI9341_TFT_wr_reg (unsigned char reg, unsigned short val) {

	LCD_ILI9341_TFT_wr_cmd(reg);
	LCD_ILI9341_TFT_wr_dat(val);
}

void LCD_ILI9341_TFT_set_orientation(void) {

    switch (orientation) {
        case 0:
        	LCD_ILI9341_TFT_wr_reg(0x36, 0x48);
            break;
        case 1:
        	LCD_ILI9341_TFT_wr_reg(0x36, 0x28);
            break;
        case 2:
        	LCD_ILI9341_TFT_wr_reg(0x36, 0x88);
            break;
        case 3:
        	LCD_ILI9341_TFT_wr_reg(0x36, 0xE8);
            break;
    }
}

void LCD_ILI9341_TFT_window (unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
  LCD_ILI9341_TFT_wr_cmd(0x2A);  // column addr set
  LCD_ILI9341_TFT_wr_dat(x>>8);
  LCD_ILI9341_TFT_wr_dat(x);   // XSTART
  LCD_ILI9341_TFT_wr_dat((x+w-1)>>8);
  LCD_ILI9341_TFT_wr_dat((x+w-1));   // XEND

  LCD_ILI9341_TFT_wr_cmd(0x2B);  // row addr set
  LCD_ILI9341_TFT_wr_dat(y>>8);
  LCD_ILI9341_TFT_wr_dat(y);    // YSTART
  LCD_ILI9341_TFT_wr_dat((y+h-1)>>8);
  LCD_ILI9341_TFT_wr_dat((y+h-1));    // YEND

  LCD_ILI9341_TFT_wr_cmd(0x2C);  // write to RAM
}


void LCD_ILI9341_TFT_WindowMax (void) {
	LCD_ILI9341_TFT_window(0, 0, LCD_ILI9341_TFT_width(),  LCD_ILI9341_TFT_height());
}


void LCD_ILI9341_TFT_tft_reset() {

    // init SPI
    LCD_PIN_Init();

    /* Start Initial Sequence ----------------------------------------------------*/
    LCD_ILI9341_TFT_wr_cmd(0x01);                         /* SW Reset                       */
    wait_ms(150);
    LCD_ILI9341_TFT_wr_cmd(0x28);                         /* display off               */
    wait_ms(500);

    LCD_ILI9341_TFT_wr_cmd(0xCF);
    LCD_ILI9341_TFT_wr_dat(0x00);
    LCD_ILI9341_TFT_wr_dat(0x83);
    LCD_ILI9341_TFT_wr_dat(0x30);

    LCD_ILI9341_TFT_wr_cmd(0xED);
    LCD_ILI9341_TFT_wr_dat(0x64);
    LCD_ILI9341_TFT_wr_dat(0x03);
    LCD_ILI9341_TFT_wr_dat(0x12);
    LCD_ILI9341_TFT_wr_dat(0x81);

    LCD_ILI9341_TFT_wr_cmd(0xEB);
    LCD_ILI9341_TFT_wr_dat(0x85);
    LCD_ILI9341_TFT_wr_dat(0x01);
    LCD_ILI9341_TFT_wr_dat(0x79);

    LCD_ILI9341_TFT_wr_cmd(0xCB);
    LCD_ILI9341_TFT_wr_dat(0x39);
    LCD_ILI9341_TFT_wr_dat(0x2C);
    LCD_ILI9341_TFT_wr_dat(0x00);
    LCD_ILI9341_TFT_wr_dat(0x34);
    LCD_ILI9341_TFT_wr_dat(0x02);


    LCD_ILI9341_TFT_wr_cmd(0xF7);
    LCD_ILI9341_TFT_wr_dat(0x20);

    LCD_ILI9341_TFT_wr_cmd(0xEA);
    LCD_ILI9341_TFT_wr_dat(0x00);
    LCD_ILI9341_TFT_wr_dat(0x00);

    LCD_ILI9341_TFT_wr_cmd(0xC0);      // POWER_CONTROL_1
    LCD_ILI9341_TFT_wr_dat(0x26);

    LCD_ILI9341_TFT_wr_cmd(0XC1);     // POWER_CONTROL_2
    LCD_ILI9341_TFT_wr_dat(0x11);

    LCD_ILI9341_TFT_wr_cmd(0xC5);     // VCOM_CONTROL_1
    LCD_ILI9341_TFT_wr_dat(0x35);
    LCD_ILI9341_TFT_wr_dat(0x3E);

    LCD_ILI9341_TFT_wr_cmd(0XC7);     // VCOM_CONTROL_2
    LCD_ILI9341_TFT_wr_dat(0xBE);

    LCD_ILI9341_TFT_wr_cmd(0x36);     // MEMORY_ACCESS_CONTROL
    LCD_ILI9341_TFT_wr_dat(0x48);

    LCD_ILI9341_TFT_wr_cmd(0x3A);     // COLMOD_PIXEL_FORMAT_SET
    LCD_ILI9341_TFT_wr_dat(0x55);     // 16 bit pixel


    LCD_ILI9341_TFT_wr_cmd(0xB1);     // Frame Rate
    LCD_ILI9341_TFT_wr_dat(0x00);
    LCD_ILI9341_TFT_wr_dat(0x1B);

    LCD_ILI9341_TFT_wr_cmd(0XF2);    // Gamma Function Disable
    LCD_ILI9341_TFT_wr_dat(0x08);

    LCD_ILI9341_TFT_wr_cmd(0x26);
    LCD_ILI9341_TFT_wr_dat(0x01);   // gamma set for curve 01/2/04/08

    /* Gamma settings  -----------------------------------------------------------*/

    LCD_ILI9341_TFT_wr_cmd(0xE0);  // positive gamma correction
    LCD_ILI9341_TFT_wr_dat(0x1F);
    LCD_ILI9341_TFT_wr_dat(0x1A);
    LCD_ILI9341_TFT_wr_dat(0x18);
    LCD_ILI9341_TFT_wr_dat(0x0A);
    LCD_ILI9341_TFT_wr_dat(0x0F);
    LCD_ILI9341_TFT_wr_dat(0x06);
    LCD_ILI9341_TFT_wr_dat(0x45);
    LCD_ILI9341_TFT_wr_dat(0x87);
    LCD_ILI9341_TFT_wr_dat(0x32);
    LCD_ILI9341_TFT_wr_dat(0x0A);
    LCD_ILI9341_TFT_wr_dat(0x07);
    LCD_ILI9341_TFT_wr_dat(0x02);
    LCD_ILI9341_TFT_wr_dat(0x07);
    LCD_ILI9341_TFT_wr_dat(0x05);
    LCD_ILI9341_TFT_wr_dat(0x00);

  LCD_ILI9341_TFT_wr_cmd(0xE1);    // negativ gamma correction
  LCD_ILI9341_TFT_wr_dat(0x00);
  LCD_ILI9341_TFT_wr_dat(0x25);
  LCD_ILI9341_TFT_wr_dat(0x27);
  LCD_ILI9341_TFT_wr_dat(0x05);
  LCD_ILI9341_TFT_wr_dat(0x10);
  LCD_ILI9341_TFT_wr_dat(0x09);
  LCD_ILI9341_TFT_wr_dat(0x3A);
  LCD_ILI9341_TFT_wr_dat(0x78);
  LCD_ILI9341_TFT_wr_dat(0x4D);
  LCD_ILI9341_TFT_wr_dat(0x05);
  LCD_ILI9341_TFT_wr_dat(0x18);
  LCD_ILI9341_TFT_wr_dat(0x0D);
  LCD_ILI9341_TFT_wr_dat(0x38);
  LCD_ILI9341_TFT_wr_dat(0x3A);
  LCD_ILI9341_TFT_wr_dat(0x1F);

  LCD_ILI9341_TFT_WindowMax ();


    LCD_ILI9341_TFT_wr_cmd(0xB7);   // entry mode
    LCD_ILI9341_TFT_wr_dat(0x07);   //

    LCD_ILI9341_TFT_wr_cmd(0xB6);   // display function control
    LCD_ILI9341_TFT_wr_dat(0x0A);            //
    LCD_ILI9341_TFT_wr_dat(0x82);
    LCD_ILI9341_TFT_wr_dat(0x27);
    LCD_ILI9341_TFT_wr_dat(0x00);


  LCD_ILI9341_TFT_wr_cmd(0x11);    // sleep out
  wait_ms(100);

  LCD_ILI9341_TFT_wr_cmd(0x29);  // display on
  wait_ms(100);

}

void LCD_ILI9341_TFT_pixel(int x, int y, uint16_t color) {
    unsigned char color_h;
    unsigned char color_l;
    color_h = (color >> 0x08);
    color_l = (color & 0xff);


  if ((x >= LCD_ILI9341_TFT_width()) || (y >= LCD_ILI9341_TFT_height())) return;

  LCD_ILI9341_TFT_window(x,y,x+1,y+1);

  // setup for data
//   _rs = 1;
	LCD_CD_HIGH();

//  _cs = 0;
	LCD_CS_LOW();

	LCD_SPI_writebyte(color_h);
	LCD_SPI_writebyte(color_l);

//  _cs = 1;
	LCD_CS_HIGH();

}


void LCD_ILI9341_TFT_cls (uint16_t color) {
    uint32_t i;

    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    LCD_ILI9341_TFT_WindowMax();

//    wr_dat_start();
    LCD_ILI9341_TFT_wr_dat_start();

    for (i = 0; i<76800; i++) {

    	LCD_SPI_writebyte(color_h);
    	LCD_SPI_writebyte(color_l);
    }

//    wr_dat_stop();
    LCD_ILI9341_TFT_wr_dat_stop();
}

void LCD_ILI9341_TFT_Init(void)
{
    char_x = 0;

    LCD_ILI9341_TFT_tft_reset();

    LCD_ILI9341_TFT_set_orientation();

    LCD_ILI9341_TFT_WindowMax();

    LCD_ILI9341_TFT_cls(White);
}




void LCD_ILI9341_TFT_circle(int x0, int y0, int r, uint16_t color) {

    int draw_x0, draw_y0;
    int draw_x1, draw_y1;
    int draw_x2, draw_y2;
    int draw_x3, draw_y3;
    int draw_x4, draw_y4;
    int draw_x5, draw_y5;
    int draw_x6, draw_y6;
    int draw_x7, draw_y7;
    int xx, yy;
    int di;
    LCD_ILI9341_TFT_WindowMax();
    if (r == 0) {       /* no radius */
        return;
    }

    draw_x0 = draw_x1 = x0;
    draw_y0 = draw_y1 = y0 + r;
    if (draw_y0 < LCD_ILI9341_TFT_height()) {
    	LCD_ILI9341_TFT_pixel(draw_x0, draw_y0, color);     /* 90 degree */
    }

    draw_x2 = draw_x3 = x0;
    draw_y2 = draw_y3 = y0 - r;
    if (draw_y2 >= 0) {
    	LCD_ILI9341_TFT_pixel(draw_x2, draw_y2, color);    /* 270 degree */
    }

    draw_x4 = draw_x6 = x0 + r;
    draw_y4 = draw_y6 = y0;
    if (draw_x4 < LCD_ILI9341_TFT_width()) {
    	LCD_ILI9341_TFT_pixel(draw_x4, draw_y4, color);     /* 0 degree */
    }

    draw_x5 = draw_x7 = x0 - r;
    draw_y5 = draw_y7 = y0;
    if (draw_x5>=0) {
    	LCD_ILI9341_TFT_pixel(draw_x5, draw_y5, color);     /* 180 degree */
    }

    if (r == 1) {
        return;
    }

    di = 3 - 2*r;
    xx = 0;
    yy = r;
    while (xx < yy) {

        if (di < 0) {
            di += 4*xx + 6;
        } else {
            di += 4*(xx - yy) + 10;
            yy--;
            draw_y0--;
            draw_y1--;
            draw_y2++;
            draw_y3++;
            draw_x4--;
            draw_x5++;
            draw_x6--;
            draw_x7++;
        }
        xx++;
        draw_x0++;
        draw_x1--;
        draw_x2++;
        draw_x3--;
        draw_y4++;
        draw_y5++;
        draw_y6--;
        draw_y7--;

        if ( (draw_x0 <= LCD_ILI9341_TFT_width()) && (draw_y0>=0) ) {
        	LCD_ILI9341_TFT_pixel(draw_x0, draw_y0, color);
        }

        if ( (draw_x1 >= 0) && (draw_y1 >= 0) ) {
        	LCD_ILI9341_TFT_pixel(draw_x1, draw_y1, color);
        }

        if ( (draw_x2 <= LCD_ILI9341_TFT_width()) && (draw_y2 <= LCD_ILI9341_TFT_height()) ) {
        	LCD_ILI9341_TFT_pixel(draw_x2, draw_y2, color);
        }

        if ( (draw_x3 >=0 ) && (draw_y3 <= LCD_ILI9341_TFT_height()) ) {
        	LCD_ILI9341_TFT_pixel(draw_x3, draw_y3, color);
        }

        if ( (draw_x4 <= LCD_ILI9341_TFT_width()) && (draw_y4 >= 0) ) {
        	LCD_ILI9341_TFT_pixel(draw_x4, draw_y4, color);
        }

        if ( (draw_x5 >= 0) && (draw_y5 >= 0) ) {
        	LCD_ILI9341_TFT_pixel(draw_x5, draw_y5, color);
        }
        if ( (draw_x6 <=LCD_ILI9341_TFT_width()) && (draw_y6 <= LCD_ILI9341_TFT_height()) ) {
        	LCD_ILI9341_TFT_pixel(draw_x6, draw_y6, color);
        }
        if ( (draw_x7 >= 0) && (draw_y7 <= LCD_ILI9341_TFT_height()) ) {
        	LCD_ILI9341_TFT_pixel(draw_x7, draw_y7, color);
        }
    }
    return;
}

void LCD_ILI9341_TFT_fillcircle(int x, int y, int r, uint16_t color) {
    int i;
    for (i = 0; i <= r; i++)
    	LCD_ILI9341_TFT_circle(x,y,i,color);
}



void LCD_ILI9341_TFT_hline(int x0, int x1, int y, uint16_t color) {
    int x;
	int w;
    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    w = x1 - x0 + 1;

    LCD_ILI9341_TFT_window(x0,y,w,1);


    LCD_ILI9341_TFT_wr_cmd(0x2C);

    LCD_ILI9341_TFT_wr_dat_start();

    for (x=0; x<w; x++) {

    	LCD_SPI_writebyte(color_h);
    	LCD_SPI_writebyte(color_l);
    }

    LCD_ILI9341_TFT_wr_dat_stop();

    return;
}



void LCD_ILI9341_TFT_vline(int x, int y0, int y1, uint16_t color) {
    int y;
	int h;

    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);

    h = y1 - y0 + 1;
    LCD_ILI9341_TFT_window(x,y0,1,h);

    LCD_ILI9341_TFT_wr_cmd(0x2C);

    LCD_ILI9341_TFT_wr_dat_start();

    for (y=0; y<h; y++) {

    	LCD_SPI_writebyte(color_h);
    	LCD_SPI_writebyte(color_l);
    }

    LCD_ILI9341_TFT_wr_dat_stop();

    return;
}



void LCD_ILI9341_TFT_line(int x0, int y0, int x1, int y1, uint16_t color) {


    LCD_ILI9341_TFT_WindowMax();

    int   dx = 0, dy = 0;
    int   dx_sym = 0, dy_sym = 0;
    int   dx_x2 = 0, dy_x2 = 0;
    int   di = 0;

    dx = x1-x0;
    dy = y1-y0;

    if (dx == 0) {        /* vertical line */
        if (y1 > y0) LCD_ILI9341_TFT_vline(x0,y0,y1,color);
        else LCD_ILI9341_TFT_vline(x0,y1,y0,color);
        return;
    }

    if (dx > 0) {
        dx_sym = 1;
    } else {
        dx_sym = -1;
    }
    if (dy == 0) {        /* horizontal line */
        if (x1 > x0) LCD_ILI9341_TFT_hline(x0,x1,y0,color);
        else  LCD_ILI9341_TFT_hline(x1,x0,y0,color);
        return;
    }

    if (dy > 0) {
        dy_sym = 1;
    } else {
        dy_sym = -1;
    }

    dx = dx_sym*dx;
    dy = dy_sym*dy;

    dx_x2 = dx*2;
    dy_x2 = dy*2;

    if (dx >= dy) {
        di = dy_x2 - dx;
        while (x0 != x1) {

        	LCD_ILI9341_TFT_pixel(x0, y0, color);
            x0 += dx_sym;
            if (di<0) {
                di += dy_x2;
            } else {
                di += dy_x2 - dx_x2;
                y0 += dy_sym;
            }
        }
        LCD_ILI9341_TFT_pixel(x0, y0, color);
    } else {
        di = dx_x2 - dy;
        while (y0 != y1) {
        	LCD_ILI9341_TFT_pixel(x0, y0, color);
            y0 += dy_sym;
            if (di < 0) {
                di += dx_x2;
            } else {
                di += dx_x2 - dy_x2;
                x0 += dx_sym;
            }
        }
        LCD_ILI9341_TFT_pixel(x0, y0, color);
    }
    return;
}




void LCD_ILI9341_TFT_rect(int x0, int y0, int x1, int y1, uint16_t color) {

    if (x1 > x0) LCD_ILI9341_TFT_hline(x0,x1,y0,color);
    else  LCD_ILI9341_TFT_hline(x1,x0,y0,color);

    if (y1 > y0) LCD_ILI9341_TFT_vline(x0,y0,y1,color);
    else LCD_ILI9341_TFT_vline(x0,y1,y0,color);

    if (x1 > x0) LCD_ILI9341_TFT_hline(x0,x1,y1,color);
    else  LCD_ILI9341_TFT_hline(x1,x0,y1,color);

    if (y1 > y0) LCD_ILI9341_TFT_vline(x1,y0,y1,color);
    else LCD_ILI9341_TFT_vline(x1,y1,y0,color);

    return;
}



void LCD_ILI9341_TFT_fillrect(int x0, int y0, int x1, int y1, uint16_t color) {

	int p;
    unsigned char color_h;
    unsigned char color_l;

    color_h = (color >> 0x08);
    color_l = (color & 0xff);


    int h = y1 - y0 + 1;
    int w = x1 - x0 + 1;
    int pixel = h * w;

    LCD_ILI9341_TFT_window(x0,y0,w,h);

    LCD_ILI9341_TFT_wr_cmd(0x2C);

    LCD_ILI9341_TFT_wr_dat_start();

    for (p=0; p<pixel; p++) {

    	LCD_SPI_writebyte(color_h);
    	LCD_SPI_writebyte(color_l);
    }

    LCD_ILI9341_TFT_wr_dat_stop();

    return;
}

void LCD_ILI9341_TFT_locate(int x, int y) {
    char_x = x;
    char_y = y;
}

void LCD_ILI9341_TFT_foreground(uint16_t colour) {
    _foreground = colour;
}

void LCD_ILI9341_TFT_background(uint16_t colour) {
    _background = colour;
}



void LCD_TFT_ShowChar(uint16_t line,uint16_t column,uint8_t AscNum,uint8_t Font,uint16_t pointColor,uint16_t backColor) //��ʾһ���ַ�( ָ������,�����С,��ɫ)-"����LCD_TFT"
{
	uint8_t  AscChar = 0;
    uint8_t  i = 0;
    uint8_t  j = 0;

	uint8_t  VerticalNum = 0;
	uint8_t  horizontalNum = 0;

	AscNum = AscNum - M_Space_ASCII;//�õ�ƫ�ƺ��ֵ//0x20Ϊ�ո�ֵ


	uint16_t xBuf = 0;
	xBuf = column;  //����x ����ֵ

	switch(Font)
	{
		case FONT1206:	//����12*6
			VerticalNum = 12;
			horizontalNum = 6;
			break;

		case FONT1608:	//����16*8
			VerticalNum = 16;
			horizontalNum = 8;
			break;
		default:   break;
	}

	/////////////////////////////////////////////////////////////////////////////
	for(i = 0; i < VerticalNum; i++)
	{
		switch(Font)
		{
			case FONT1206:	//����12*6
				AscChar = asc2_1206[AscNum][i];//����1206����
				break;

			case FONT1608:	//����16*8
				AscChar = asc2_1608[AscNum][i];//����1608����
				break;
			default:   break;
		}

		/////////////////////////////////////////////////////////////////////////////
		for(j = 0; j < horizontalNum; j++)
		{
			if(AscChar & 0x01) LCD_ILI9341_TFT_pixel(column, line, pointColor);  // ���ַ����ĵ�
			else  LCD_ILI9341_TFT_pixel(column, line, backColor);  //���հ״��ĵ�

			AscChar >>= 1;
			column ++;
		}

		column = xBuf;
		line ++;
	}

}


void LCD_TFT_ShowString(uint16_t column,uint16_t line,uint8_t *ArrayPoint,uint8_t Font,uint16_t pointColor,uint16_t backColor) //��ʾ�ַ���( ָ������,�����С,��ɫ)-"����LCD_TFT"
{
	uint16_t x = 0;
	uint16_t y = 0;

	uint8_t  VerticalNum = 0;
	uint8_t  horizontalNum = 0;

	uint16_t max_x = 0;
	uint16_t max_y = 0;

	x = column;
	y = line;
	switch(Font)
	{
		case FONT1206:	//����12*6
			VerticalNum = 12;
			horizontalNum = 6;
			max_x = 320;
			max_y = 240;
			break;

		case FONT1608:  //����16*8
			VerticalNum = 16;
			horizontalNum = 8;
			max_x = 320;
			max_y = 240;
			break;
		default:   break;
	}

	//////////////////////////////////////////////////////////////////////
	while(*ArrayPoint != '\0')
	{
        if(x > max_x) {x = 0; y += VerticalNum;}
        if(y > max_y) {x = y = 0; LCD_ILI9341_TFT_cls(backColor);}

		LCD_TFT_ShowChar(y,x,*ArrayPoint,Font,pointColor,backColor);

		x += horizontalNum;    //�����ַ�ۼӣ�ָ����һ���ַ�����ʾλ��
        ArrayPoint ++;  //����ָ���ۼӣ�ָ����һ���ַ�
    }


}



/****************************************************************************
*������-Function:	void LCD_Show_ChFont2424(void)
*����- Description:		��ʾ1��24*24�ĺ���-"����LCD_ILI9325"
*�������-Input:	(x,y):������ʾ��λ��    index:tfont24��������ĵڼ�������
						pointColor: ���ֻ�����ɫ;     backColor: ������ɫ;
*�������-output:	None
*ע������-Note��
****************************************************************************/
void LCD_Show_ChFont2424(uint16_t x,uint16_t y,uint8_t index,uint16_t pointColor,uint16_t backColor) //��ʾ1��24*24�ĺ���-"����LCD_ILI9325"
{
	uint8_t byteNum = 0;
	uint8_t i = 0;
	uint8_t temp;

	uint16_t x0=x;

	for(byteNum = 0; byteNum < 72; byteNum++)//ÿ��24*24�ĺ��ֵ��� ��72���ֽ�
	{
		//��ά����ÿһά���24�ֽ�
		if(byteNum < 24) temp = tfont24[index*3][byteNum];             //ǰ24���ֽ�
		else if(byteNum < 48) temp=tfont24[index*3 + 1][byteNum - 24]; //��24���ֽ�
		else temp = tfont24[index*3 + 2][byteNum - 48];                //��24���ֽ�

		for(i = 0; i < 8; i++)	 //ÿ���ֽ�8λ����
		{
			if(temp & 0x80) LCD_ILI9341_TFT_pixel(y,x,pointColor); //�����ֵĵ�
			else LCD_ILI9341_TFT_pixel(y,x,backColor);   //���հ׵㣨ʹ�ñ���ɫ��
			temp <<= 1;
			x ++;
			if((x-x0) == 24)	  //�ﵽ��������ֵ��ʼ�µ�һ��
			{
				x=x0;
				y++;
				break;
			}
		}
	}
}

/****************************************************************************
*������-Function:	void LCD_Show_StandbyPage(void)
*����- Description:		��ʾ"����ҳ��"-"����LCD_ILI9325"
*�������-Input:	None
*�������-output:	None
*ע������-Note��
****************************************************************************/
void LCD_Show_StandbyPage(void) //��ʾ"����ҳ��"-"����LCD_ILI9325"
{
	uint8_t  ChNum = 0;
	uint16_t yStart = 0;


	LCD_ILI9341_TFT_cls(Red);  //ȫ������-"����LCD_ILI9325"

	LCD_ILI9341_TFT_cls(Green);  //ȫ������-"����LCD_ILI9325"

	LCD_ILI9341_TFT_cls(Blue);  //ȫ������-"����LCD_ILI9325"

	LCD_ILI9341_TFT_fillrect(0, 0, 319,39,Red);
	LCD_ILI9341_TFT_fillrect(0, 40,319,79,Black);
	LCD_ILI9341_TFT_fillrect(0, 80,319,119,Blue);
	LCD_ILI9341_TFT_fillrect(0, 120,319,159,Green);
	LCD_ILI9341_TFT_fillrect(0, 160,319,199,GreenYellow);
	LCD_ILI9341_TFT_fillrect(0, 200,319,239,Yellow);

	LCD_TFT_ShowString( 90,8,"TEXAS INSTRUMENTS",FONT1608,White,Red);
	LCD_TFT_ShowString(74,48,"TI University Program",FONT1608,GreenYellow,Black);

	yStart = 88;
	for(ChNum = 12;ChNum < 17; ChNum++)//MSP430_FFTB
	{
		LCD_Show_ChFont2424(88,yStart,ChNum,GreenYellow,Blue);  //LIGHTGREEN
		yStart += 24;
	}

	yStart = 76;
	for(ChNum = 0;ChNum < 6; ChNum++) //���е��ӿƼ�
	{
		LCD_Show_ChFont2424(128,yStart,ChNum,Red,Green);
		yStart += 26;
	}

	LCD_TFT_ShowString(40,168,"Deyan Electronic Technology",FONT1608,Red,GreenYellow);

	LCD_TFT_ShowString(95,208,"www.gototi.com",FONT1608,Blue,Yellow);

}




