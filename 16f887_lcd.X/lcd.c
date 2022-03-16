#include "lcd.h"
#include "stdbool.h"
#include "string.h"
#define LCD_MAX_NUM_CHARACTER       (32)

static void lcdSet(lcdData_t const * const obj);
static void lcdWriteData( lcdData_t const * const obj, unsigned char c);
static void lcdConfig(lcdData_t const * const obj);

/*ctor*/
void lcdInit(lcdData_t * const obj, pFcnGpio E, pFcnGpio RS,
             pFcnGpio data, pFcnWait wait){  /*Control_Data es de tipo OutFcn_t*/
	obj->ctrlEnable = E; /*Asigno  wrapper de función al driver */
	obj->ctrlRS = RS;
	obj->ctrlData = data;
	obj->ctrlWait = wait;

    lcdConfig(obj);
    lcdSetPosition(obj, 0);
}

static void lcdSet(lcdData_t const * const obj){
	obj->ctrlRS(false);
	obj->ctrlEnable(true);
	obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);
}


static void lcdConfig(lcdData_t const * const obj){
    obj->ctrlData(LCD_REG_CLEAR);
	lcdSet(obj);
	obj->ctrlData(LCD_REG_RETURN_HOME);
	lcdSet(obj);
    obj->ctrlData(LCD_REG_FUNCTION_SET);
    lcdSet(obj);
    obj->ctrlData(LCD_REG_DISPLAY_ON_OFF);	
	lcdSet(obj);
    obj->ctrlData(LCD_REG_ENTRY_MODE_SET);
    lcdSet(obj);

}

void lcdWriteRegister(lcdData_t const * const obj, uint8_t regAddr){
    obj->ctrlData(regAddr);
	lcdSet(obj);
}
void lcdSetPosition(lcdData_t const * const obj, unsigned char position){
	obj->ctrlRS(false);
	lcdWriteData(obj, LCD_ADDR_FISRT_LINE + position);
}
static void lcdWriteData( lcdData_t const * const obj, unsigned char c){
	obj->ctrlEnable(true);
	obj->ctrlData(c) ;
	obj->ctrlWait(LCD_DELAY_MS); 
	obj->ctrlEnable(false);
}

void lcdPutsInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos){
    lcdSetPosition(obj, 0 + initPos);
    int i = 0;
    while(s[i]){
        obj->ctrlRS(true);
        lcdWriteData(obj, s[i++]);
    }
}

void lcdPutsInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos){
    lcdSetPosition(obj, LCD_ADDR_SECOND_LINE + initPos);
    int i = 0;
    while(s[i]){
        obj->ctrlRS(true);
        lcdWriteData(obj, s[i++]);
    }
}

/*set position before use*/
void lcdPuts(lcdData_t const * const obj, const char *s){
    	
    	uint8_t i = 0;
        
        char line1[LCD_MAX_NUMBER_CHAR_BY_LINE] = {0};
        char line2[LCD_MAX_NUMBER_CHAR_BY_LINE] = {0};
        if(strlen(s) >= LCD_MAX_NUMBER_CHAR_BY_LINE){
            memcpy(line1,s,LCD_MAX_NUMBER_CHAR_BY_LINE);
            memcpy(line2,s+LCD_MAX_NUMBER_CHAR_BY_LINE,strlen(s) -LCD_MAX_NUMBER_CHAR_BY_LINE );
            
        }else{
            memcpy(line1,s,strlen(s)); 
        }
        lcdPutsInLine1(obj, line1, 0);//
        lcdPutsInLine2(obj, line2, 0);//
/*
        lcdSetPosition(obj, 0);
        while(line1[i] != '\0' && i < LCD_MAX_NUMBER_CHAR_BY_LINE ){
            obj->ctrlRS(true);
        	lcdWriteData(obj, line1[i++]);
        }
        lcdSetPosition(obj, LCD_ADDR_SECOND_LINE);
        i = 0;
        while(line2[i] != '\0' && i < LCD_MAX_NUMBER_CHAR_BY_LINE ){
            obj->ctrlRS(true);
        	lcdWriteData(obj, line2[i++]);
        }
*/
    }


void lcdPutch (lcdData_t const * const obj, unsigned char c){
    	obj->ctrlRS(true);
    	lcdWriteData(obj, c);
    }

	/*Wrapper */
	/*
	* void CtrlEn(uint8_t Status){
	*		RB0 = Status;
	*	}
	* void CtrlRs(uint8_t Status){
	*		RB1 = Status;
	*	}
	* void CtrlData(uint8_t Data){
	*		PORTB = Data;
	*	}
	 */

	/* lcd_Init(&lcd_Config,CtrlEn ,CtrlRs ,CtrlData , __delay_ms);
 */