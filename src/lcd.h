/**
 * @file lcd.h
 * @brief interface for lcd 2x16
 * @author Julian Bustamante N
 * @date 17-march-2022
*/

#ifndef _LCD_H_
#define _LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

#define LCD_ENABLE_8BITS                    (0)     /*1-> 8 bits ; 0 -> 4 bits (DB4-DB7 reg)*/
    
#if (LCD_ENABLE_8BITS == 0)
    #define LCD_ENABLE_4BITS                (1)
#else
    #define LCD_ENABLE_4BITS                (0)
#endif

#define LCD_VERSION                         "1.0.0"
    
#define LCD_DELAY_MS                        (2)    
    
//exported macro
#define LCD_SET_BIT(REG, BIT)               ((REG) |= (BIT))
#define LCD_CLEAR_BIT(REG, BIT)             ((REG) &= ~(BIT))

    
//macro config
#define LCD_REG_CLEAR                       (1)
#define LCD_REG_RETURN_HOME                 (2)

//#define LCD_REG_ENTRY_MODE_SET            (6)  
#define REG_ENTRYMODE_S_BIT_POS             (0U)  
#define REG_ENTRYMODE_S_BIT_MASK            (0x1U << REG_ENTRYMODE_S_BIT_POS)
#define REG_ENTRYMODE_S_BIT                 (REG_ENTRYMODE_S_BIT_MASK & 0xff)

#define REG_ENTRYMODE_ID_BIT_POS            (1U)  
#define REG_ENTRYMODE_ID_BIT_MASK           (0x1U << REG_ENTRYMODE_ID_BIT_POS)
#define REG_ENTRYMODE_ID_BIT                (REG_ENTRYMODE_ID_BIT_MASK & 0xff)

#define REG_ENTRYMODE_BIT_POS               (2U)  
#define REG_ENTRYMODE_BIT_MASK              (0x1U << REG_ENTRYMODE_BIT_POS)
#define REG_ENTRYMODE_BIT                   (REG_ENTRYMODE_BIT_MASK & 0xff)

    
//#define LCD_REG_DISPLAY_ON_OFF          (12)  
#define REG_DISPLAY_ON_OFF_B_BIT_POS         (0U)  
#define REG_DISPLAY_ON_OFF_B_BIT_MASK        (0x1U << REG_DISPLAY_ON_OFF_B_BIT_POS)
#define REG_DISPLAY_ON_OFF_B_BIT             (REG_DISPLAY_ON_OFF_B_BIT_MASK & 0xff)
#define REG_DISPLAY_ON_OFF_C_BIT_POS         (1U)  
#define REG_DISPLAY_ON_OFF_C_BIT_MASK        (0x1U << REG_DISPLAY_ON_OFF_C_BIT_POS)
#define REG_DISPLAY_ON_OFF_C_BIT             (REG_DISPLAY_ON_OFF_C_BIT_MASK & 0xff)
#define REG_DISPLAY_ON_OFF_D_BIT_POS         (2U)  
#define REG_DISPLAY_ON_OFF_D_BIT_MASK        (0x1U << REG_DISPLAY_ON_OFF_D_BIT_POS)
#define REG_DISPLAY_ON_OFF_D_BIT             (REG_DISPLAY_ON_OFF_D_BIT_MASK & 0xff)
#define REG_DISPLAY_ON_OFF_BIT_POS           (3U)  
#define REG_DISPLAY_ON_OFF_BIT_MASK          (0x1U << REG_DISPLAY_ON_OFF_BIT_POS)
#define REG_DISPLAY_ON_OFF_BIT               (REG_DISPLAY_ON_OFF_BIT_MASK & 0xff)

#define LCD_REG_CURSOR_DISPLAY_SHIFT         (28)
    
//#define LCD_REG_FUNCTION_SET               (56)
#define REG_FUNCTION_SET_F_BIT_POS           (2U)  
#define REG_FUNCTION_SET_F_BIT_MASK          (0x1U << REG_FUNCTION_SET_F_BIT_POS)
#define REG_FUNCTION_SET_F_BIT               (REG_FUNCTION_SET_F_BIT_MASK & 0xff)
#define REG_FUNCTION_SET_N_BIT_POS           (3U)  
#define REG_FUNCTION_SET_N_BIT_MASK          (0x1U << REG_FUNCTION_SET_N_BIT_POS)
#define REG_FUNCTION_SET_N_BIT               (REG_FUNCTION_SET_N_BIT_MASK & 0xff)
#define REG_FUNCTION_SET_DL_BIT_POS          (4U)  
#define REG_FUNCTION_SET_DL_BIT_MASK         (0x1U << REG_FUNCTION_SET_DL_BIT_POS)
#define REG_FUNCTION_SET_DL_BIT              (REG_FUNCTION_SET_DL_BIT_MASK & 0xff)
#define REG_FUNCTION_SET_BIT_POS             (5U)  
#define REG_FUNCTION_SET_BIT_MASK            (0x1U << REG_FUNCTION_SET_BIT_POS)
#define REG_FUNCTION_SET_BIT                 (REG_FUNCTION_SET_BIT_MASK & 0xff)


#define LCD_ADDR_FISRT_LINE                 (0x80U)
#define LCD_ADDR_SECOND_LINE                (0x40U)  
#define LCD_MAX_NUMBER_CHAR_BY_LINE         (16)

    

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
void lcdSetCursorPosition(lcdData_t const * const obj, uint8_t position);
void lcdPrint(lcdData_t const * const obj, const char *s);
void lcdPrintChar(lcdData_t const * const obj, unsigned char character);
void lcdSendRegister(lcdData_t const * const obj, uint8_t regAddr);
void lcdPrintInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdPrintInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdCreateCustomCharacter (lcdData_t const * const obj, unsigned char *Pattern, const char Location);
void lcdCls(lcdData_t const * const obj);

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