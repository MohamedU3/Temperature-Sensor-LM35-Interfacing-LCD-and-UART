#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include "config.h"
#define _XTAL_FREQ 4000000


void __interrupt()ISR();
void ADC_init();
void ADC_Read(uint8_t);
void UART_TX_INIT(void);
void UART_Write(uint8_t);
void UART_Write_String(char*);
char* tempStr[25];
void main(void) {
     //choose AN0 channel
    ADC_init();
    UART_TX_INIT();
    while(1)
    {
        ADC_Read(0);
        __delay_ms(30); 
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
        //channels from AN0 ~ AN7 , in all these hex values the GO_DONE bit 0 because
        //because you MUST have a delay (ACQUISITION TIME) before starting the conversion and bit G0_DONE starts the convesions 
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
        sprintf(tempStr,"Temperature is : %.3f C\r\n",temperature);
        UART_Write_String(tempStr);
        ADIF = 0;
    }
}

void UART_TX_INIT(void){
    //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
    TRISC6 = 1; // As stated in the datasheet
    TRISC7 = 1; // As stated in the datasheet
    // Set For High-Speed Baud Rate
    BRGH = 1; 
    SPBRG = 25;  // Set The Baud Rate To Be 9600 bps
    //--[ Enable The Ascynchronous Serial Port ]--
    SYNC = 0;
    SPEN = 1;
    // Enable UART Transmission
    TXEN = 1;
}

void UART_Write(uint8_t data){
    //TRMT: Transmit Shift Register Status bit
    //1 = TSR empty
    //0 = TSR full
    while(!TRMT);
    TXREG = data;
}

void UART_Write_String(char* text)
{
    for(uint16_t i = 0; text[i] != '\0' ; i++) UART_Write(text[i]);
}