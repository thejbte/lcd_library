
#include <stdio.h>
#include <stdint.h>

#include "src/lcd.h"
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
    unsigned char Pattern3[]= {0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00};
    //unsigned char Pattern4[]= {0x00,0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00};

    lcdCreateCustomCharacter (&objLcd, Pattern1, 1);
    lcdCreateCustomCharacter (&objLcd, Pattern3, 3);
    //while(1)
    {
        lcdSetPosition(&objLcd, 0);
        lcdPuts(&objLcd, "1%°*789012345abcdefghijklmno");
        lcdPutsInLine1(&objLcd, "a", 0);
        
        lcdSetPosition(&objLcd, 0);
        lcdPutch(&objLcd, 0x7e);
        lcdPutsInLine1(&objLcd, "Julian",1);

        lcdSetPosition(&objLcd, 6);
        lcdPutch(&objLcd, 1);
        lcdSetPosition(&objLcd, 8);
        lcdPutch(&objLcd, 3);
   }
}