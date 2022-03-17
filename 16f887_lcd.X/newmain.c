/*
 * File:   newmain.c
 * Author: julia
 *
 * Created on March 11, 2022, 11:54 AM
 */



//https://www.pjrc.com/tech/8051/board5/lcd_example.html




// PIC16F887 Configuration Bit Settings
// programna2 de recepcin darwin lcd
#include <xc.h>
#include <stdio.h>
#include "lcd.h"

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_ON & IESO_ON & FCMEN_ON & LVP_OFF);
__CONFIG(BOR4V_BOR40V & WRT_OFF);


#define LCD_RS RD6   //le colocoo al pin rd6 un nombre  por q vaconectado a rs en el lcd
#define LCD_EN RD7  //le colocoo al pin  bit rd7 un nombre  por q vaconectado a EN q es el enable  en el lcd
#define LCD_DATA   PORTC // al puerto b completo  lo llamo data que es donde llega los datos

#define _XTAL_FREQ 4000000

int cont,centenas,decenas,unidades,temp;

void bin_bcd( void);


void ctrlEn(uint8_t status);
void ctrlRs(uint8_t status);
void ctrlData(uint8_t Data);

//void ctrlWait( uint32_t const ms);
void delay_ms(uint32_t milliseconds);

void main(){
    TRISD=0;   // RS ENABLE  SALIDAS
    TRISB=0;   //DATOS
    TRISC=0;   //DATOS
    PORTB=0;
    PORTD=0;
    PORTC=0;

    lcdData_t objLcd;
    lcdInit(&objLcd,ctrlEn, ctrlRs , ctrlData, delay_ms );
 char b[7]={0x10,0x10,0x16,0x19,0x11,0x11,0x1E}; 
 unsigned char Pattern1 [ ] = { 0x0e, 0x0e, 0x04, 0x04, 0x1f, 0x04, 0x0a, 0x0a } ;
unsigned char Pattern3[]= {0x00,0x00,0x0a,0x15,0x11,0x0a,0x04,0x00};
unsigned char Pattern4[]= {0x00,0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00};

        lcdCreateCustomCharacter (&objLcd, Pattern1, 1);
        lcdCreateCustomCharacter (&objLcd, Pattern3, 3);
    while(1){
        //lcdSetPosition(&objLcd, 0);
        //lcdPuts(&objLcd, "1%°*789012345abcdefghijklmno");
        //lcdPutsInLine1(&objLcd, "a", 0);
        
        lcdSetPosition(&objLcd, 0);
        lcdPutch(&objLcd, 0x7e);
        lcdPutsInLine1(&objLcd, "Adriana TE AMO",1);

        //lcdSetPosition(&objLcd, 6);
        //lcdPutch(&objLcd, 1);
        //lcdSetPosition(&objLcd, 8);
        lcdPutch(&objLcd, 3);
       // lcdWriteRegister(&objLcd, REG_CGRAM_BIT | b[0] );
  __delay_ms(500);
   }
}



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




void bin_bcd(void)
{
    temp=50;//porcentaje;
    centenas=0;
    decenas=0;
    unidades=0;

    while(temp>=100)
    {
    temp=temp-100;
    // averiguar modulo
    centenas++;}

    while(temp>=10)
    {temp=temp-10;
    decenas++;
    }

    while(temp>=1)
    {
    temp=temp-1;
    // averiguar modulo
    unidades;}

}

