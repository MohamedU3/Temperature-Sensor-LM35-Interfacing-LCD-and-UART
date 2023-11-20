//---------------------------------LCD-----------------------------
#include <xc.h>
#define _XTAL_FREQ 4000000
#define LCD_DATA_PORT_D TRISB
#define LCD_RS_D TRISB5
#define LCD_EN_D TRISB4
#define LCD_EN_DELAY 250 //data sheet says minimum needed is 230 NANO SECOND so i set it to 1 MICRO SECOND = 1000 NANO
#define D4 RB0
#define D5 RB1
#define D6 RB2
#define D7 RB3
#define RS RB4 //1: DS , 0: IR
#define EN RB5
//FUNCTIONS OF THE LCD.h
//LCD_INIT(),LCD_WRITE_CHAR(char data),LCD_WRITE_STRING(char* str),LCD_CLEAR(),RETURN_HOME()
//LCD_DISPLAY(unsigned char state) 1:Display ON , 0:Display off
//LCD_SHOW_CURSOR(unsigned char state) 1:cursor ON , 0:cursor off
//SHIFTING SCREEN OR CURSOR: LCD_SR(),LCD_SL(),LCD_CR(),LCD_CL()
//SET CURSOR POSITION: LCD_SET_CURSOR(unsigned char r,unsigned char c)
void LCD_DATA_4BIT(unsigned char);
void LCD_CMD(unsigned char);
void LCD_INIT();
void LCD_WRITE_CHAR(char);
void LCD_WRITE_STRING(char*);
void LCD_CLEAR();
void LCD_DISPLAY(unsigned char);
void LCD_SHOW_CURSOR(unsigned char);
void RETURN_HOME();
void LCD_SR();
void LCD_SL();
void LCD_CR();
void LCD_CL();
void LCD_SET_CURSOR(unsigned char,unsigned char);
