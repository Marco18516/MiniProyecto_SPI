/*
 * File:   main.c
 * Author: Marco
 *
 * Created on 16 de febrero de 2021, 10:19 PM
 */
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
//******************************************************************************
// Definiciones y Librerias
//******************************************************************************
#define _XTAL_FREQ 8000000 //Libreria de Delay 
#define RS RE0
#define EN RE1
#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7

#include "Lib_LCD.h"
//#include "LIB_USART.h"
//******************************************************************************
// Funciones
//******************************************************************************
void CONFIG_IO(void);
void OSCILADOR(void);
void ADC_INTERRUPT(void);
void Mandar1(void);
void Mandar2(void);
//******************************************************************************
// Variables
//******************************************************************************
char data[16];//Variable para mostrar datos en LCD
float volt, volt2;//Variable para voltajes de POTs

void main(void) {
    CONFIG_IO();
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    while(1){
       PORTCbits.RC2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(PORTB);
       PORTD = spiRead();
       
       __delay_ms(1);
       PORTCbits.RC2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       PORTB++;
    }
    return;
}

//Configuracion de puertos de entrada y salida
void CONFIG_IO(void) { 
    TRISA = 0; //Salida.
    TRISB = 0; 
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC3 = 0;
    TRISD = 0; //Salida
    TRISE = 0; //Salida
    
    ANSEL = 0;
    ANSELH = 0;
    TRISC2 = 0;
    TRISB = 0;
    TRISD = 0;
    PORTCbits.RC2 = 1;
    
    
    // Se limpian los puertos
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;
}