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
int  porcentaje,n;
int cont,centenas,decenas,unidades,temp;
float  x;
void lcd_init (void);   // declaro funciones configuro display

void set_lcd (void);

void lcd_puts (const char * s);  // para enviar un msj cualesquiera // char* es un string

void lcd_putch (char c );

void lcd_goto (unsigned char  pos );

void lcd_write (unsigned char c );
void bin_bcd( void);
void mostrar_en_lcd(void);

void ctrlEn(uint8_t status);
void ctrlRs(uint8_t status);
void ctrlData(uint8_t Data);

//void ctrlWait( uint32_t const ms);
void delay_ms(uint32_t milliseconds);
void fcn(void){

}
void main()
{   //RCSTA=0X90;	// HABILITA RECEPCION

//TXSTA=0X20;
//SPBRG=25;
//TRISC=0XFF;   // RECOMENDACION DEL DATASHEET

  //  PR2=255;
    //CCP2CON=0X0C; // --0011xx los dos ceros son bits menos significativos de ccp2con los otros 8 son de ccpr2l son los mas significativos tomando
                         // la modulacion a 10 bits, los 11xx los dos 11 son el modo pwm  y no comparcion o captura
     //CCPR2L=0;   // NO COLOCO EN UN % FIJO SINO EN CERO PA VARIARLO 10% A10%

    //TMR2IF=0;       	// bandera tmr2if  limpia no se a desbordado en tmr2
   // T2CON=0X02;     	// coloca el prestacalar en 1:16 cambia con caa 16 ciclos de instruccion  , 1x, deshabilito arrancar el ancho de pulso  que empiese a crecer tmr2
    // tmr2on arranca a subir la rampa yo la modifico abajo =1
    //TRISC=0X80;  // COMO SALIDA POR Q ES UNA SALIDA  ERA RECOMENDACION DEL DATASHEET

    TRISD=0;   // RS ENABLE  SALIDAS
    TRISB=0;   //DATOS
     TRISC=0;   //DATOS
    PORTB=0;
    PORTD=0;
    PORTC=0;

     //lcd_init();  // funcion configurar  lcd
       lcdData_t objLcd;
    lcdInit(&objLcd,ctrlEn, ctrlRs , ctrlData, delay_ms );
    //lcdPuts(&objLcd, "lalal");
    while(1){

        n=0;//RCREG;

              porcentaje=(x *100)/255;
              // mostrar_en_lcd();
           lcdSetPosition(&objLcd, 0);
           lcdPuts(&objLcd, "123456789012345abcdefghijklmno");
           lcdPutsInLine1(&objLcd, "JJulian", 0);
            
          
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




void interrupt isr(void)  	//GENERO LA INTERRUPCION PARA QUE CUANDO VARIE RCREG DATORECIBIDO ME VARIE E PANTALLA SI NO COLOCO INTERRUPCION NODA
{
    if(RCIF==1)
   {

    n=RCREG;


    __delay_ms(10);

   }
}
void mostrar_en_lcd(void)
{    	lcd_goto (0);  //selecciono primera linea   empieza en la posicion 17 del display
        lcd_puts("porcentaje "); // seleciono cualquier mensaje  comop esta en comillas ya esta en ascii asi lo lee el lcd HASTA 16 CARACTER

        lcd_goto(0x40); // seleciono segunda linea
        lcd_puts("igual a: 	%   	");// CAPTURA CADA CARACTER ASI SEAN ESPACIOS Y LOS MANDA
                      //c9ca cb



      for(int i=15;i>0;i--)
        {
            bin_bcd();

        lcd_goto(0x49); // 0x49 porque 49 +80 es 129  c9 la posicion del lcd // 48 es el 0
        lcd_putch(centenas+48); // muestrop un caracter  solo el que va en cb

         lcd_goto(0x4a); // 0x4a porque 4b +80 es 12a  ca la posicion del lcd
        lcd_putch(decenas+48); // muestrop un caracter  solo el que va en cb

         lcd_goto(0x4b); //  0x4c porque 4b +80 es 12b  cb la posicion del lcd
        lcd_putch(unidades+48); // muestrop un caracter  solo el que va en cb

        }
}

void putch(unsigned char byte)
{
    while(!TRMT)                              	// trmt BIT DE TRANSMISION si esta en 1 esta vacio en 0 lleno  para transmitr
        continue;
        TXREG=byte;
}
// funciones
void lcd_init(void)
{

LCD_DATA=1;// limpiar lcd
set_lcd();

LCD_DATA=2; //retornar
set_lcd();

LCD_DATA=6;//  en 6 no semueve  i/d incremento automaticvo de posicion  1 i/d  s 110 s = informacion visualizada  se desplaza al escribir un nuevo caracter
// en 6 no mueve el cursor en 7 si
set_lcd();

LCD_DATA=12;// configurar   d c b  diplay on   cursro off,   parpadeo off 1100 1dcb
set_lcd();

//LCD_DATA=28; // esta no estaba es para mover display  si quiero la utilizo para moverlo
//set_lcd();

LCD_DATA=56; // d/l n f x x   comuniicacion  a 8bit o 4 bit   8bit de d7-d0 y 4 bit de d4-d7
set_lcd();






}
void set_lcd(void)   //   ESTO ES LA FUNCION DE
{
    LCD_RS=0; // CONFIGURO  Y NO MUESTRO
    LCD_EN=1;  // HABILITO CONFIGURACON DEJA Q LO MANDE
    __delay_ms(2); // PARA Q PUEDA MANDAR LOS DATOS Q SE DEMORA 1.4MS

    LCD_EN=0; //DESAHABILITA
    }

void lcd_goto (unsigned char pos )
{
    LCD_RS=0;// QUE SE PREPAS PARA CONFIGURAR
    lcd_write (0x80+pos); // llameme la funcio WRITEt y llevele  8 0 Y POS ES PARA VARIAR hasta c0 por q le sumo40 en lcd_goto
}

void lcd_write(unsigned char  c)
{
    LCD_EN=1;
    LCD_DATA= (c); // LLEVA EL DATO 8 0 A  EL PUERTO B 80 ES LA DIRECCION DEL PRIMER CUADRO
    __delay_ms(2);
    LCD_EN=0;
}
void lcd_puts(const char *s)
{
    LCD_RS=1;// muestre  lo que hay en la funcion puts " temperatura : "
    while(*s) // 8 0 + 40 es  120 por que es igual a c0 de la segunda linea
        lcd_write(*s++);// * s es  mientras exista un caracter en la cadena y entonces tengo de que  *s ++ es
     //para Variar de posicion la cadena de caracteres
}

void lcd_putch(char c)
{
    LCD_RS=1;
       lcd_write(c);//leer caracteres
       __delay_ms(5);  // para que no parpadeee  los numeros
}

void bin_bcd(void)
{temp=porcentaje;
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

