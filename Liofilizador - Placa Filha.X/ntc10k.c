#include "global.h"
#include "adc.h"
#include "ntc10k.h"

double NTC10K_Read_Temperature(unsigned char canal) 
{
 // variables that live in this function
 double rThermistor = 0;            // Holds thermistor resistance value
 double tKelvin     = 0;            // Holds calculated temperature
 double tCelsius    = 0;            // Hold temperature in celsius
 double adcAverage  = 0;            // Holds the average voltage measurement

 adcAverage = 0;//ADC_Media_Filtro2_Read(canal); 

 /* Here we calculate the thermistor?s resistance using the equation 
    discussed in the article. */
 rThermistor = BALANCE_RESISTOR * ( (MAX_ADC / adcAverage) - 1);

 /* Here is where the Beta equation is used, but it is different
    from what the article describes. Don't worry! It has been rearranged
    algebraically to give a "better" looking formula. I encourage you
    to try to manipulate the equation from the article yourself to get
    better at algebra. And if not, just use what is shown here and take it
    for granted or input the formula directly from the article, exactly
    as it is shown. Either way will work! */
 
 tKelvin = (BETA * ROOM_TEMP) / 
           (BETA + (ROOM_TEMP * log(rThermistor / RESISTOR_ROOM_TEMP)));

 /* I will use the units of Celsius to indicate temperature. I did this
    just so I can see the typical room temperature, which is 25 degrees
    Celsius, when I first try the program out. I prefer Fahrenheit, but
    I leave it up to you to either change this function, or create
    another function which converts between the two units. */
 
 tCelsius = tKelvin - 273.15;  // convert kelvin to celsius 
 if(tCelsius<=20)
    tCelsius=tCelsius-8.3+(tCelsius*0.21); //Correção de Leitura     
 else
    tCelsius=tCelsius-5.0+((tCelsius*tCelsius)*0.0029); // Correção de Leitura 
    
 
 return tCelsius;    // Return the temperature in Celsius
}