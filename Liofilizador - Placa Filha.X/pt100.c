#include "global.h"
#include "pt100.h"
#include "adc.h"

#ifndef NTC_BOARD
double PT100_Read_Temperature(unsigned char canal){
       //
       // ADC_Read(canal);
       //((5.0/1023)*(1023.0-ADC_Read(canal))*(PT100_CONSTANT))+(PT100_TRANSLADA);
       return 0;
}
#endif