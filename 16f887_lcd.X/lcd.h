#ifndef _LCD_H_
#define _LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define LCD_REG_CLEAR                   (1)
#define LCD_REG_RETURN_HOME             (2)
#define LCD_REG_ENTRY_MODE_SET          (6)
#define LCD_REG_DISPLAY_ON_OFF          (12)
#define LCD_REG_CURSOR_DISPLAY_SHIFT    (28)
#define LCD_REG_FUNCTION_SET            (56)
//#define LCD_REG_FUNCTION_SET_4BITS      (40)

#define LCD_DELAY_MS                    (2)
#define LCD_ADDR_FISRT_LINE             (0x80U)
#define LCD_ADDR_SECOND_LINE            (0x40U)  /*0x80+ 0x40 = 0xc0*/
#define LCD_MAX_NUMBER_CHAR_BY_LINE     (16)

typedef void (*pFcnGpio)(uint8_t);   /*E*/
typedef void (*pFcnWait)(uint32_t);

typedef struct {
    pFcnGpio ctrlEnable;
	pFcnGpio ctrlRS ;
	pFcnGpio ctrlData ;
	pFcnWait ctrlWait ;
}lcdData_t;


/*ctor*/
void lcdInit(lcdData_t * const obj, pFcnGpio E, pFcnGpio RS,
             pFcnGpio data, pFcnWait wait);
void lcdSetPosition(lcdData_t const * const obj, unsigned char position);
void lcdPuts (lcdData_t const * const obj, const char *s);
void lcdPutch (lcdData_t const * const obj, unsigned char c);
void lcdWriteRegister(lcdData_t const * const obj, uint8_t regAddr);
void lcdPutsPos(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdPutsInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdPutsInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos);
#ifdef __cplusplus
}
#endif

#endif /*_LCD_H_*/

/*

1. Display clear
2. Function set:
DL = 1; 8-bit interface data
N = 0; 1-line display
F = 0; 5 × 8 dot character font
3. Display on/off control:
D = 0; Display off
C = 0; Cursor off
B = 0; Blinking off
4. Entry mode set:
I/D = 1; Increment by 1
S = 0; No shift

*/