#include "lcd.h"
#include "stdbool.h"
#include "string.h"

#define LCD_MAX_NUM_CHARACTER                       (32)
#define LCD_ADDR_DDRAM_CUSTOM_CHAR                  (0x40U)  /*0x40 0x48 de 8 en 8*/
#define LCD_ADDR_OFFSET_DDRAM_CUSTOM_CHAR           (8U)  /*0x40 0x48 de 8 en 8*/

static void lcdWriteData( lcdData_t const * const obj, unsigned char data);
static void lcdConfig(lcdData_t const * const obj);
static void lcdPuts(lcdData_t const * const obj, const char *s);

/*constructor*/
void lcdInit(lcdData_t * const obj, pFcnGpio E, pFcnGpio RS,
             pFcnGpio data, pFcnWait wait){
	obj->ctrlEnable = E; /*Asigno  wrapper de función al driver */
	obj->ctrlRS = RS;
	obj->ctrlData = data;
	obj->ctrlWait = wait;

    lcdConfig(obj);
    lcdSetCursorPosition(obj, 0);
}


static void lcdConfig(lcdData_t const * const obj){  
    lcdSendRegister(obj, LCD_REG_CLEAR);
	lcdSendRegister(obj, LCD_REG_RETURN_HOME);
    #if (LCD_ENABLE_8BITS == 1)
    lcdSendRegister(obj, REG_FUNCTION_SET_BIT | REG_FUNCTION_SET_DL_BIT | REG_FUNCTION_SET_N_BIT);
    #else
        lcdSendRegister(obj, REG_FUNCTION_SET_BIT | REG_FUNCTION_SET_N_BIT);
    #endif
    lcdSendRegister(obj, REG_DISPLAY_ON_OFF_BIT | REG_DISPLAY_ON_OFF_D_BIT);	
    lcdSendRegister(obj, REG_ENTRYMODE_BIT | REG_ENTRYMODE_ID_BIT);   
}
/*
 REG_ENTRYMODE_BIT
 */
void lcdSendRegister(lcdData_t const * const obj, uint8_t regAddr){
#if (LCD_ENABLE_8BITS == 1)
    obj->ctrlRS(false);
	obj->ctrlEnable(true);    
    obj->ctrlData(regAddr);
	obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);    
#else
    obj->ctrlRS(false);
	obj->ctrlEnable(true); 
    obj->ctrlData( 0x0F );  //clear nibble port
    obj->ctrlData( regAddr & 0xF0 ); 
    obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(true);
    obj->ctrlData( 0x0F ); 
    obj->ctrlData( (regAddr<<4) & 0xF0 );
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
#endif 

}

void lcdSetCursorPosition(lcdData_t const * const obj, uint8_t position){
	obj->ctrlRS(false);
	lcdWriteData(obj, LCD_ADDR_FISRT_LINE + position);
}
static void lcdWriteData( lcdData_t const * const obj, unsigned char data){
   
#if (LCD_ENABLE_8BITS == 1)
	obj->ctrlEnable(true);    
    obj->ctrlData(data);
	obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);    
#else
	obj->ctrlEnable(true); 
    obj->ctrlData( 0x0F );
    obj->ctrlData( data & 0xF0 );
    obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(true);
    obj->ctrlData( 0x0F );  //LCD_DATA=LCD_DATA&0X0F;
    obj->ctrlData( (data<<4) & 0xF0 );
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
#endif 
}

static void lcdPuts(lcdData_t const * const obj, const char *s){
    int i = 0;
    while(s[i]){
        obj->ctrlRS(true);
        lcdWriteData(obj, s[i++]);
    }
}

void lcdPrintInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos){
    lcdSetCursorPosition(obj, 0U + initPos);
    lcdPuts(obj, s);
}

void lcdPrintInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos){
    lcdSetCursorPosition(obj, LCD_ADDR_SECOND_LINE + initPos);
    lcdPuts(obj, s);
}

/*set position before use*/
void lcdPrint(lcdData_t const * const obj, const char *s){
    	
    	uint8_t i = 0;
        uint8_t lenStr = strlen(s);
        char line1[LCD_MAX_NUMBER_CHAR_BY_LINE+1] = {0};
        char line2[LCD_MAX_NUMBER_CHAR_BY_LINE+1] = {0};
        if( lenStr >= LCD_MAX_NUMBER_CHAR_BY_LINE){
            memcpy(line1, s,LCD_MAX_NUMBER_CHAR_BY_LINE);
            memcpy(line2, s+LCD_MAX_NUMBER_CHAR_BY_LINE, (lenStr -LCD_MAX_NUMBER_CHAR_BY_LINE) > LCD_MAX_NUMBER_CHAR_BY_LINE? 
                    LCD_MAX_NUMBER_CHAR_BY_LINE : (lenStr -LCD_MAX_NUMBER_CHAR_BY_LINE) );
        }else{
            memcpy(line1,s,lenStr); 
        }
        lcdPrintInLine1(obj, line1, 0);//
        lcdPrintInLine2(obj, line2, 0);//
    }


void lcdPrintChar(lcdData_t const * const obj, unsigned char character){
    	obj->ctrlRS(true);
    	lcdWriteData(obj, character);
    }
void lcdCreateCustomCharacter(lcdData_t const * const obj, unsigned char *Pattern, const char Location)
{ 
    int i=0; 
    lcdSendRegister(obj, LCD_ADDR_DDRAM_CUSTOM_CHAR + (Location*LCD_ADDR_OFFSET_DDRAM_CUSTOM_CHAR) );     //Send the Address of CGRAM
    for (i=0; i<8; i++)
    lcdPrintChar(obj, Pattern [ i ] );         //Pass the bytes of pattern on LCD 
}

void lcdCls(lcdData_t const * const obj){
    lcdSendRegister(obj, LCD_REG_CLEAR);
}


//USE


    /*
     lcdData_t objLcd;
     lcdInit(&objLcd,ctrlEn, ctrlRs , ctrlData, delay_ms );
     */

    /*
      //lcdSetCursorPosition(&objLcd, 0);
      lcdPrint(&objLcd, "1%�*789012345abcdefghijklmno"); // run in pos 0
     * 
      lcdPrintInLine1(&objLcd, "alal", 0);
     */

    /* unsigned char Pattern1 [ ] = { 0x0e, 0x0e, 0x04, 0x04, 0x1f, 0x04, 0x0a, 0x0a } ;
        lcdCreateCustomCharacter (&objLcd, Pattern1, 1); //save in index 1
        lcdSetCursorPosition(&objLcd, 6); // set cursor
        lcdPrintChar(&objLcd, 1);  // show character saved in index 1
     */


	/*Wrapper */

/*
    void ctrlEn(uint8_t status){
        PORTDbits.RD7 = status;
    }
    void ctrlRs(uint8_t status){
        PORTDbits.RD6 = status; 
    }
    void ctrlData(uint8_t Data){
     PORTC = Data;
    }

     void delay_ms(uint32_t milliseconds){
       while(milliseconds > 0)
       {
           __delay_ms(1);
          milliseconds--;
        }
     }
 */