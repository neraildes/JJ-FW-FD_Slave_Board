/*-----------------------------------------------------------------------------/
 * File:   adc.h                                                               /
 * Author: Neraildes                                                           /
 * Revisão 1.0.0                                                               /
 * Created on 19 de Abril de 2018, 06:24                                       /
 * Descrição:                                                                  /
 *		Funções de Analogic to Digital converter                               /
 -----------------------------------------------------------------------------*/


#ifndef ADC_H
#define	ADC_H

#define SAMPLE_NUMBER     10
void ADC_init(void);
unsigned int captura(void);
float ADC_Read_NTC(char canal);
float ADC_Media_10bits(char canal);
unsigned int ADC_Max_10Bits(char canal);
#endif





