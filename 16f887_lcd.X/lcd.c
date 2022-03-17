#include "lcd.h"
#include "stdbool.h"
#include "string.h"
#include <xc.h>//borrar, solo test

#define LCD_MAX_NUM_CHARACTER       (32)

static void lcdWriteData( lcdData_t const * const obj, unsigned char data);
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


static void lcdConfig(lcdData_t const * const obj){  
    lcdWriteRegister(obj, LCD_REG_CLEAR);
	lcdWriteRegister(obj, LCD_REG_RETURN_HOME);
    #if (LCD_ENABLE_8BITS == 1)
    lcdWriteRegister(obj, REG_FUNCTION_SET_BIT | REG_FUNCTION_SET_DL_BIT | REG_FUNCTION_SET_N_BIT);
    #else
        lcdWriteRegister(obj, REG_FUNCTION_SET_BIT | REG_FUNCTION_SET_N_BIT);
    #endif
    lcdWriteRegister(obj, REG_DISPLAY_ON_OFF_BIT | REG_DISPLAY_ON_OFF_D_BIT);	
    lcdWriteRegister(obj, REG_ENTRYMODE_BIT | REG_ENTRYMODE_ID_BIT);   
}
/*
 REG_ENTRYMODE_BIT
 */
void lcdWriteRegister(lcdData_t const * const obj, uint8_t regAddr){
#if (LCD_ENABLE_8BITS == 1)
    obj->ctrlRS(false);
	obj->ctrlEnable(true);    
    obj->ctrlData(regAddr);
	obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);    
#else
    obj->ctrlRS(false);
	obj->ctrlEnable(true); 
    obj->ctrlData( 0x0F );  //LCD_DATA=LCD_DATA&0X0F;
    obj->ctrlData( regAddr & 0xF0 ); //aux=dato&0XF0; LCD_DATA=aux|LCD_DATA;
    //PORTC &=0x0F;  
	//PORTC |=regAddr & 0xF0; 
 
    obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(true);
    obj->ctrlData( 0x0F );  //LCD_DATA=LCD_DATA&0X0F;
    obj->ctrlData( (regAddr<<4) & 0xF0 ); //dato=dato<<4;aux=dato; aux=aux&0XF0; LCD_DATA=aux|LCD_DATA;
    //PORTC &=0x0F;  
	//PORTC |=(regAddr<<4) & 0xF0; 
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
#endif 

}

void lcdSetPosition(lcdData_t const * const obj, unsigned char position){
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
    obj->ctrlData( 0x0F );  //LCD_DATA=LCD_DATA&0X0F;
    obj->ctrlData( data & 0xF0 ); //aux=dato&0XF0; LCD_DATA=aux|LCD_DATA;
  	//PORTC &=0x0F;  
	//PORTC |=data & 0xF0;
    obj->ctrlWait(LCD_DELAY_MS);
	obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(true);
    obj->ctrlData( 0x0F );  //LCD_DATA=LCD_DATA&0X0F;
    obj->ctrlData( (data<<4) & 0xF0 ); //dato=dato<<4;aux=dato; aux=aux&0XF0; LCD_DATA=aux|LCD_DATA;
    //PORTC &=0x0F;  
	//PORTC |=(data<<4) & 0xF0;
    obj->ctrlWait(LCD_DELAY_MS);
    obj->ctrlEnable(false);
    obj->ctrlWait(LCD_DELAY_MS);
#endif 
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