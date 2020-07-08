
#ifndef LAB_LCDKEY_DY_IOT_PB_H_
#define LAB_LCDKEY_DY_IOT_PB_H_

//CC3200LP-PIN    CC_GPIO          function             DY-IOT-PB
//J1
//VCC_BRD             3V            PWR
//P58             CC_GPIO_03        ANA_IN              GPIO03/AIN
//P04             CC_GPIO_13        RX                  GPIO13/RXD
//P03             CC_GPIO_12        TX                  GPIO12/TXD
//P61             CC_GPIO_06        GPIO                GPIO06
//P59             CC_GPIO_04        ANA_IN              GPIO04/AIN
//P05             CC_GPIO_14        SPI_CLK             GPIO14/SCLK
//P62             CC_GPIO_07        GPIO                GPIO07
//P01             CC_GPIO_10        SCL                 GPIO10/SCL
//P02             CC_GPIO_11        SDA                 GPIO11/ASDA
//J3
//VCC_MCU_5V          5V            PWR
//GND                 GND           GND
//P57             LP_GPIO_02       ANA_IN               GPIO02/AIN
//P60             CC_GPIO_05       ANA_IN               GPIO05/AIN
//P58             CC_GPIO_03        DNP
//P59             CC_GPIO_04        DNP
//P63             CC_GPIO_08       I2S_FSYNC            GPIO08/FSYNC
//P53             CC_GPIO_30       I2S_CLK              GPIO30/BCLK
//P64             CC_GPIO_09       I2S_DOUT             GPIO09/DOUT
//P50             CC_GPIO_00       I2S_DIN              GPIO00/DIN
//J4
//P29             CC_GPIO_26       PWM/GPIO             GPIO26
//P30             CC_GPIO_27       PWM/GPIO             GPIO27
//P17             CC_GPIO_24       PWM/GPIO-DNP
//P64             CC_GPIO_09       PWM/GPIO-DNP
//P21             CC_GPIO_25       CCP/GPIO-DNP
//P18             CC_GPIO_28       CCP/GPIO-DNP
//P62             CC_GPIO_07       GPIO/NWP_LOG-DNP
//P60             CC_GPIO_05       GPIO/WLAN_LOG-DNP
//P16             CC_GPIO_23       GPIO/RS232_RX        GPIO23
//P17             CC_GPIO_24       GPIO/RS232_TX        GPIO24
//J2
//GND             GND              GND
//P18             CC_GPIO_28       GPIO                 GPIO28
//P08             CC_GPIO_17       SPI_CS               GPIO17/SPI_CS
//P45             CC_GPIO_31       TEST                 GPIO31
//RST             LP_RESET_OUT     RESET                RESET
//P07             CC_GPIO_16       SPI_MOSI             GPIO16/MOSI
//P06             CC_GPIO_15       SPI_MISO             GPIO15/MISO
//P21             CC_GPIO_25       GPIO                 GPIO23/PWM
//P55             CC_GPIO_01       GPIO                 GPIO01
//P15             CC_GPIO_22       GPIO                 GPIO22
//
//CC3200LP
//SW3---P04---CC_GPIO_13
//SW2---P15---CC_GPIO_22
//D5----P02---CC_GPIO_11---GREEN
//D6----P01---CC_GPIO_10---YELLOW
//D7----P64---CC_GPIO_09---RED
//J2----SCL---CC_GPIO_10
//J3----SDA---CC_GPIO_11
//J4----INT---CC_GPIO_13
//
//DY-IOT-PB
//WHEEL   GPIO_03/AIN
//
//RGB_LED GPIO_28 GPIO_27 GPIO_26
//
//Key1    GPIO_01
//Key2    GPIO_22
//Key3    GPIO_23
//Key4    GPIO_24
//
//BEEP  GPIO_25/PWM
//
//APDS9930 GPIO_11/SDA GPIO_10/SCL
//SHT20 GPIO_11/SDA GPIO_10/SCL
//
//LCD_SCL  GPIO_14/SCLK
//LCD_SDI  GPIO_16/MOSI
//LCD_SDO  GPIO_15/MISO
//LCD_CS   GPIO_07
//LCD_CD   GPIO_06
//
//FONT_SCLK GPIO_14/SCLK
//FONT_SI   GPIO_16/MOSI
//FONT_SO   GPIO_15/MISO
//FONT_CS   GPIO_31
//
//SD_DAT3  GPIO_08/FSYNC
//SD_CMD   GPIO_16/MOSI
//SD_CLK   GPIO_14/SCLK
//SD-DAT0  GPIO_15/MISO
//
//MODULE
//






#endif /* LAB_LCDKEY_DY_IOT_PB_H_ */
