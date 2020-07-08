
#ifndef LAB_LCDKEY_FONT_LIB_H_
#define LAB_LCDKEY_FONT_LIB_H_

#define GT20_READ           0x03
#define GT20_FAST_READ      0x11



#define FONT_CS_PORT         GPIOA3_BASE
#define FONT_CS_PIN          GPIO_PIN_7


// Definition of USCI base address to be used for SPI communication


typedef enum __FontType_
{
    GB2312_15x16,
    ASCII_7x8,
    GB2312_8x16,
    ASCII_8x16,
    ASCII_5x7,
    ASCII_Arial,
    ASCII_8x16_Bold,
    ASCII_TimesNewRoman
} FontType;

typedef struct __FontSize_
{
    FontType name;
    int font_column;
    int font_row;
} FontSize;

extern void GT20Init(void);
extern void GT20Read(FontType , uint16_t , uint8_t *, uint8_t *, uint16_t *);


#endif /* LAB_LCDKEY_FONT_LIB_H_ */
