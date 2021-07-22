/*
 * File:   ADC.c
 * Author: Guille Lam
 *
 * Created on 20 de julio de 2021, 02:34 PM
 */


#define _XTAL_FREQ 250000
#include <xc.h>
#include <stdint.h>
#include "ADC.h"

void ADCnum(char num) { 
    switch(num){
        case 0: 
            ADCON0bits.CHS = 0;     
            __delay_us(100);
            ADCON0bits.ADCS0 = 0;   
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
        case 1: 
            ADCON0bits.CHS = 0;     
            __delay_us(100);
            ADCON0bits.ADCS0 = 1;   
            ADCON0bits.ADCS1 = 0;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
        case 2: //FOSC/32
            ADCON0bits.CHS = 0;     
            __delay_us(100);
            ADCON0bits.ADCS0 = 0;   
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
        case 3: 
            ADCON0bits.CHS = 0;     
            __delay_us(100);
            ADCON0bits.ADCS0 = 1;   
            ADCON0bits.ADCS1 = 1;   
            ADCON0bits.ADON = 1;    
            ADCON1bits.ADFM = 0;    
            ADCON1bits.VCFG1 = 0;   
            ADCON1bits.VCFG0 = 0;   
            break;
    }
    return;
}
