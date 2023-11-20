#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "config.h"
#include "LCD.h"
#define _XTAL_FREQ 4000000
//FUNCTIONS OF THE LCD.h
//LCD_INIT(),LCD_WRITE_CHAR(char data),LCD_WRITE_STRING(char* str),LCD_CLEAR(),RETURN_HOME()
//LCD_DISPLAY(unsigned char state) 1:Display ON , 0:Display off
//LCD_SHOW_CURSOR(unsigned char state) 1:cursor ON , 0:cursor off
//SHIFTING SCREEN OR CURSOR: LCD_SR(),LCD_SL(),LCD_CR(),LCD_CL()
//SET CURSOR POSITION: LCD_SET_CURSOR(unsigned char r,unsigned char c)

void __interrupt()ISR();
void ADC_init();
void ADC_Read(uint8_t);
char* buffer[10];
void main(void) {
     //choose AN0 channel
    ADC_init();
    LCD_INIT();
    LCD_CLEAR();
    while(1)
    {
        ADC_Read(0);
        __delay_ms(10);
    }
    return;
}

void ADC_init(){
    //ANO channel is selected by default (CONVERSION DOESN'T START IN THE INITIALIZATION)
    ADCON0 = 0x41; 
    //0x80 value -> right justified with all channels defined as analog
    ADCON1 = 0x80;
    //interrupt
    ADIF = 0; ADIE = 1; PEIE = 1; GIE = 1;
}

void ADC_Read(uint8_t channel_num){
    if(channel_num >= 0 && channel_num < 8)
    {
        //channels from AN0 ~ AN7 , in all these hex values the GO_DONE is pick as 0
        //because you MUST have a delay (ACQUISITION TIME) before starting the conversion 
        uint8_t channel[] = {0x41,0x49,0x51,0x59,0x61,0x69,0x71,0x79};
        //choose channel number
        ADCON0 = channel[channel_num];
        //ACQUISITION TIME (the time the capacitor needs to charge for the sample and hold circuit) MINIMUM 20us
        __delay_us(30); 

        /*GO/DONE: A/D Conversion Status bit
        When ADON = 1:
        1 = A/D conversion in progress (setting this bit starts the A/D conversion which is automatically
        cleared by hardware when the A/D conversion is complete)
        0 = A/D conversion not in progress*/
        GO_DONE = 1;
    }
}

void __interrupt()ISR(){
    if(ADIF){
        //read here
        uint16_t AN0_RES = (ADRESH << 8) + ADRESL; //right justified 
        float temperature = AN0_RES * 0.488;
        sprintf(buffer,"%.3f",temperature);
        LCD_SET_CURSOR(1,5);
        LCD_WRITE_STRING("Temperature:");
        LCD_SET_CURSOR(2,5);
        LCD_WRITE_STRING(buffer);
        ADIF = 0;
    }
}