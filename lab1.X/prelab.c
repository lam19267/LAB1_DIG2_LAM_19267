/*
 * File:   prelab.c
 * Author: Guille Lam
 *
 * Created on 20 de julio de 2021, 11:12 AM
 */


#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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


#define _XTAL_FREQ 250000
#include <xc.h>
#include <stdint.h>
#include <stdio.h>  
#include "ADC.h"



//Variables 
char sietes[16] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
    0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, 0b01110111, 
    0b01111100, 0b00111001, 0b01011110, 0b01111001, 0b01110001};
char ADC;
int bandera;
char display;
uint8_t nib2;
uint8_t nib1;

//Prototipos 
void setup(void);
char nibbles (char display);
void alarma(void);

//Interrupciones
void __interrupt() isr(void)
{
    if(PIR1bits.ADIF == 1)// Revisa bandera 
       {
        if(ADCON0bits.CHS == 0) {
            ADC = ADRESH;
        }
        PIR1bits.ADIF = 0;//Kimpieza de bandera
    }
       
    if(T0IF == 1)// Verifica bandera
    {   
        RA5 = 0;//se ponen los puertos en 0
        RA6 = 0;
       if (bandera == 1){//se realiza la separación de nibbles
            PORTAbits.RA5 = 1; 
            PORTD = sietes[nib2];
            bandera = 0;
        }
        else {
            PORTAbits.RA6 = 1;
            PORTD = sietes[nib1];
            bandera = 1;
        }
       
        INTCONbits.T0IF = 0;//Limpieza de bandera
        TMR0 = 255;//Valor de reinicio de bandera
        
    }
    if (RBIF == 1)  // Verifica bandera 
    {
        if (PORTBbits.RB0 == 0)//Revisa el boton 1
        {
            PORTC = PORTC + 1;//suma 1 al puerto C
        }
        if  (PORTBbits.RB1 == 0)//Revisa el boton 2
        {
            PORTC = PORTC - 1;//resta 1 al puerto C
        }
        INTCONbits.RBIF = 0;//Limpieza de bandera 
    }
}

//Main
void main(void) {
    setup();//se llama a la configuracion general    
    while(1)//loop
    {
        nibbles(ADC);
        if (GO == 0){
            __delay_us(100);
            GO = 1;
        }
        alarma();//se llama a la funcion de alarma
    }
}

//Funcionea
void setup(void){
    //Configuracion de puertos digitales
    ANSEL = 0b00000001;
    ANSELH = 0x00;
    
    //Configuracion bits de salida o entradas
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA4 = 0;
    TRISAbits.TRISA5 = 0;
    TRISAbits.TRISA6 = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0x00;
    TRISD = 0x00;
    
    //Limpieza de puertos
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    
    //Configuracion del oscilador
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;   // Se configura a 250kHz
    OSCCONbits.SCS = 1;
    
    //Configuracion Timer0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    
    // Configuracion PORTB Pull up
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    //Configuacion de interrupciones
    INTCONbits.GIE = 1;
    INTCONbits.RBIF = 1;    // Para que el boton funcione a la primera
    INTCONbits.RBIE = 1;    // Interrupcion Puerto B
    INTCONbits.PEIE = 1;    // Periferical interrupt
    INTCONbits.T0IF = 0; 
    PIE1bits.ADIE = 1;      // Activar la interrupcion del ADC
    PIR1bits.ADIF = 0;      // Bandera del ADC
    
    //Llamada de la libreria
    ADCnum(2);
}

char nibbles (char display){    
    nib1 = display & 0x0F;//Bits menos significativos
    nib2 = (display>>4) & 0x0F;//Bits mas significativos
    return nib1;
    return nib2;
}

void alarma (void){//funcion de alarma
    if (ADC < PORTC){
        RA4 = 1;//se prende led
    }
    else {
        RA4 = 0;//se apaga led
    }
}
