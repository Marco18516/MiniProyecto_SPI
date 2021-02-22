/*
 * File:   POT_ADC.c
 * Author: Marco
 *
 * Created on 18 de febrero de 2021, 11:03 AM
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

//******************************************************************************
// Definiciones y Librerias
//******************************************************************************
#define _XTAL_FREQ 8000000 //Libreria de Delay 
#include "Lib_ADC.h"
#include "SPI.h"
//******************************************************************************
// Funciones
//******************************************************************************
void CONFIG_IO(void);
void OSCILADOR(void);
void ADC_INTERRUPT(void);
void ADC(void);//
void INTER(void);
//void Mandar1(void);
//******************************************************************************
// Variables
//******************************************************************************
double volt;//Variable para voltajes de POTs
//unsigned int result_adc;

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(SSPIF == 1){
        PORTD = spiRead();
        spiWrite(PORTB);
        SSPIF = 0;
    }
}
//*****************************************************************************
// Código Principal
//*****************************************************************************
void main(void) {
    CONFIG_IO();
    OSCILADOR();
    ADC_INIT();
    ADC_INTERRUPT();
    INTER();
    
    while (1) {
        //Conversion de POT1
        ADC(); 
        //Mandar1();
        __delay_ms(1);
        
        PORTB--;
       __delay_ms(250);

    }

    return;
}

//Configuracion de Puertos
    void CONFIG_IO(void) { 
    ANSEL = 0b00000001;//Se habilitan AN0 y AN1 para los canales del ADC
    ANSELH = 0;
    TRISA = 0b00000001;//RA0 como entrada
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    //Se limpian los puertos
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}

void OSCILADOR(void) {
    OSCCON = 0b01110001; //8MHz
}

//Se deshabilitan las interrupciones del ADC
void ADC_INTERRUPT() { 
    PIE1bits.ADIE = 0;
    PIR1bits.ADIF = 0;
    OPTION_REG = 0b00000000;
    INTCON = 0b00000000;
}

void ADC(void) {
    ADC_CHANNEL(0); //canal 0
    //Cinfiguracion bits ADCON0
    ADCON0bits.ADCS0 = 1;//Clock ADC conversion
    ADCON0bits.ADCS1 = 0;//Fosc/2
    ADCON0bits.ADON = 1;//Se habilita el ADC
    __delay_ms(0.25);//Para conversion
    ADCON0bits.GO = 1;  //Inicia la conversión
    while (ADCON0bits.GO == 1) {
        //result_adc = ADRESH; //Desplegar ADRESH en result_adc
        //PORTC = result_adc;
        volt = ((ADRESH * 5.0) / 255); //CONVERSION DE  0-5V      
    }

}

void INTER(void){
    INTCONbits.GIE = 1;         // Habilitamos interrupciones
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones PEIE
    PIR1bits.SSPIF = 0;         // Borramos bandera interrupción MSSP
    PIE1bits.SSPIE = 1;         // Habilitamos interrupción MSSP
    TRISAbits.TRISA5 = 1;       // Slave Select
}