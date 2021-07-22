/* 
 * File:   ADC.h
 * Author: Guille Lam
 *
 * Created on 20 de julio de 2021, 02:31 PM
 */

#ifndef ADC_H
#define	ADC_H
#pragma config FOSC = INTRC_NOCLKOUT
#include <xc.h>

void ADCnum(char num);

char translate_hex(char var);

#endif 



