
#include <stdio.h>
#include <stdint.h>

#include "src/lcd.h"

#define LOCATION_PATTERN_0  (0)
#define LOCATION_PATTERN_1  (1)

void ctrlEn(uint8_t status){
    //PORTDbits.RD7 = status;
}
void ctrlRs(uint8_t status){
    //PORTDbits.RD6 = status; 
}
void ctrlData(uint8_t Data){
 //PORTC = Data;
}

 void delay_ms(uint32_t milliseconds){
   while(milliseconds > 0)
   {
       //__delay_ms(1);
      milliseconds--;
    }
 }

//TODO: pendiente agregar unity test, cmock

int main(){
    lcdData_t objLcd;
    lcdInit(&objLcd,ctrlEn, ctrlRs , ctrlData, delay_ms );
 
    unsigned char Pattern1 [ ] = { 0x0e, 0x0e, 0x04, 0x04, 0x1f, 0x04, 0x0a, 0x0a } ;
    unsigned char Pattern0[]= {0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00};
    //unsigned char Pattern4[]= {0x00,0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00};

    lcdCreateCustomCharacter (&objLcd, Pattern1, LOCATION_PATTERN_1);
    lcdCreateCustomCharacter (&objLcd, Pattern0, LOCATION_PATTERN_0);
    //while(1)
    {
        lcdSetCursorPosition(&objLcd, 0);
        lcdPrint(&objLcd, "123456789012345abcdefghijklmnopq123456");
        lcdPrintInLine1(&objLcd, "a", 0);
        
        lcdSetCursorPosition(&objLcd, 0);
        lcdPrintChar(&objLcd, 0x7e);
        lcdPrintInLine1(&objLcd, "123456789012345abcdefghijklmnopq123456",1);

        lcdSetCursorPosition(&objLcd, 6);
        lcdPrintChar(&objLcd, LOCATION_PATTERN_1);
        lcdSetCursorPosition(&objLcd, 8);
        lcdPrintChar(&objLcd, LOCATION_PATTERN_0);
   }
}