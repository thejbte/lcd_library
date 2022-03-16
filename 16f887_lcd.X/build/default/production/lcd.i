
# 1 "lcd.c"

# 13 "C:\Program Files\Microchip\xc8\v2.35\pic\include\c90\stdint.h"
typedef signed char int8_t;

# 20
typedef signed int int16_t;

# 28
typedef __int24 int24_t;

# 36
typedef signed long int int32_t;

# 52
typedef unsigned char uint8_t;

# 58
typedef unsigned int uint16_t;

# 65
typedef __uint24 uint24_t;

# 72
typedef unsigned long int uint32_t;

# 88
typedef signed char int_least8_t;

# 96
typedef signed int int_least16_t;

# 109
typedef __int24 int_least24_t;

# 118
typedef signed long int int_least32_t;

# 136
typedef unsigned char uint_least8_t;

# 143
typedef unsigned int uint_least16_t;

# 154
typedef __uint24 uint_least24_t;

# 162
typedef unsigned long int uint_least32_t;

# 181
typedef signed char int_fast8_t;

# 188
typedef signed int int_fast16_t;

# 200
typedef __int24 int_fast24_t;

# 208
typedef signed long int int_fast32_t;

# 224
typedef unsigned char uint_fast8_t;

# 230
typedef unsigned int uint_fast16_t;

# 240
typedef __uint24 uint_fast24_t;

# 247
typedef unsigned long int uint_fast32_t;

# 268
typedef int32_t intmax_t;

# 282
typedef uint32_t uintmax_t;

# 289
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;

# 23 "lcd.h"
typedef void (*pFcnGpio)(uint8_t);
typedef void (*pFcnWait)(uint32_t);

typedef struct {
pFcnGpio ctrlEnable;
pFcnGpio ctrlRS ;
pFcnGpio ctrlData ;
pFcnWait ctrlWait ;
}lcdData_t;



void lcdInit(lcdData_t * const obj, pFcnGpio E, pFcnGpio RS,
pFcnGpio data, pFcnWait wait);
void lcdSetPosition(lcdData_t const * const obj, unsigned char position);
void lcdPuts (lcdData_t const * const obj, const char *s);
void lcdPutch (lcdData_t const * const obj, unsigned char c);
void lcdWriteRegister(lcdData_t const * const obj, uint8_t regAddr);
void lcdPutsPos(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdPutsInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos);
void lcdPutsInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos);

# 15 "C:\Program Files\Microchip\xc8\v2.35\pic\include\c90\stdbool.h"
typedef unsigned char bool;

# 4 "C:/Program Files/Microchip/MPLABX/v6.00/packs/Microchip/PIC16Fxxx_DFP/1.3.42/xc8\pic\include\__size_t.h"
typedef unsigned size_t;

# 14 "C:\Program Files\Microchip\xc8\v2.35\pic\include\c90\string.h"
extern void * memcpy(void *, const void *, size_t);
extern void * memmove(void *, const void *, size_t);
extern void * memset(void *, int, size_t);

# 36
extern char * strcat(char *, const char *);
extern char * strcpy(char *, const char *);
extern char * strncat(char *, const char *, size_t);
extern char * strncpy(char *, const char *, size_t);
extern char * strdup(const char *);
extern char * strtok(char *, const char *);


extern int memcmp(const void *, const void *, size_t);
extern int strcmp(const char *, const char *);
extern int stricmp(const char *, const char *);
extern int strncmp(const char *, const char *, size_t);
extern int strnicmp(const char *, const char *, size_t);
extern void * memchr(const void *, int, size_t);
extern size_t strcspn(const char *, const char *);
extern char * strpbrk(const char *, const char *);
extern size_t strspn(const char *, const char *);
extern char * strstr(const char *, const char *);
extern char * stristr(const char *, const char *);
extern char * strerror(int);
extern size_t strlen(const char *);
extern char * strchr(const char *, int);
extern char * strichr(const char *, int);
extern char * strrchr(const char *, int);
extern char * strrichr(const char *, int);

# 6 "lcd.c"
static void lcdSet(lcdData_t const * const obj);
static void lcdWriteData( lcdData_t const * const obj, unsigned char c);
static void lcdConfig(lcdData_t const * const obj);


void lcdInit(lcdData_t * const obj, pFcnGpio E, pFcnGpio RS,
pFcnGpio data, pFcnWait wait){
obj->ctrlEnable = E;
obj->ctrlRS = RS;
obj->ctrlData = data;
obj->ctrlWait = wait;

lcdConfig(obj);
lcdSetPosition(obj, 0);
}

static void lcdSet(lcdData_t const * const obj){
obj->ctrlRS(0);
obj->ctrlEnable(1);
obj->ctrlWait((2));
obj->ctrlEnable(0);
}


static void lcdConfig(lcdData_t const * const obj){
obj->ctrlData((1));
lcdSet(obj);
obj->ctrlData((2));
lcdSet(obj);
obj->ctrlData((56));
lcdSet(obj);
obj->ctrlData((12));
lcdSet(obj);
obj->ctrlData((6));
lcdSet(obj);

}

void lcdWriteRegister(lcdData_t const * const obj, uint8_t regAddr){
obj->ctrlData(regAddr);
lcdSet(obj);
}
void lcdSetPosition(lcdData_t const * const obj, unsigned char position){
obj->ctrlRS(0);
lcdWriteData(obj, (0x80U) + position);
}
static void lcdWriteData( lcdData_t const * const obj, unsigned char c){
obj->ctrlEnable(1);
obj->ctrlData(c) ;
obj->ctrlWait((2));
obj->ctrlEnable(0);
}

void lcdPutsInLine1(lcdData_t const * const obj, const char *s, uint8_t initPos){
lcdSetPosition(obj, 0 + initPos);
int i = 0;
while(s[i]){
obj->ctrlRS(1);
lcdWriteData(obj, s[i++]);
}
}

void lcdPutsInLine2(lcdData_t const * const obj, const char *s, uint8_t initPos){
lcdSetPosition(obj, (0x40U) + initPos);
int i = 0;
while(s[i]){
obj->ctrlRS(1);
lcdWriteData(obj, s[i++]);
}
}


void lcdPuts(lcdData_t const * const obj, const char *s){

uint8_t i = 0;

char line1[(16)] = {0};
char line2[(16)] = {0};
if(strlen(s) >= (16)){
memcpy(line1,s,(16));
memcpy(line2,s+(16),strlen(s) -(16) );

}else{
memcpy(line1,s,strlen(s));
}
lcdPutsInLine1(obj, line1, 0);
lcdPutsInLine2(obj, line2, 0);

# 106
}


void lcdPutch (lcdData_t const * const obj, unsigned char c){
obj->ctrlRS(1);
lcdWriteData(obj, c);
}

