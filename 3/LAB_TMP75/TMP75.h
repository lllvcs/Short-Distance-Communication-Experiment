#ifndef _TMP75_H_
#define _TMP75_H_


        #define SHUTDOWN_MODE_OFF  0x00
        #define SHUTDOWN_MODE_ON   0x01
        #define COMPARATOR_MODE    0x00
        #define INTERRUPT_MODE     0x02
        #define POLARITY_0         0x00
        #define POLARITY_1         0x04
        #define FAULT_QUEUE_1      0x00
        #define FAULT_QUEUE_2      0x08
        #define FAULT_QUEUE_4      0x10
        #define FAULT_QUEUE_6      0x18
        #define RESOLUTION_9       0x00
        #define RESOLUTION_10      0x20
        #define RESOLUTION_11      0x40
        #define RESOLUTION_12      0x60
        #define ONE_SHOT           0x80

//        TMP175(PinName PIN_SDA, PinName PIN_SCL, PinName PIN_ALERT=NC);
        void vSetConfigurationTMP175(unsigned char Config);
        void vStartSingleConversionTMP175(void);
        float fReadTemperatureTMP175(void);
        void vSetTemperatureLowTMP175(float Value);
        void vSetTemperatureHighTMP175(float Value);
        float fReadTemperatureHighTMP175(void);
        float fReadTemperatureLowTMP175(void);
        void TMP75int(void);
        float readtemp(void);
//        bool bReadPinAlertTMP175(void);
//        #ifdef MBED_OPERATORS
//        operator float();
//        #endif


#endif /* _TMP75_H_ */
