/*-----------------------------------------------------------------------------/
 * File:   ntc10k.h                                                            /
 * Author: Neraildes                                                           /
 * Revisão 1.0.0                                                               /
 * Created on 19 de Abril de 2018, 07:38                                       /
 * Descrição:                                                                  /
 *		Leitura de sensor PT100 em entradas analógicas do PIC.                 /
 -----------------------------------------------------------------------------*/



#ifndef NTC10K_H
#define	NTC10K_H

const double BALANCE_RESISTOR = 10000.0;
const double MAX_ADC = 1023.0;
const double BETA    = 3974.0;
const double ROOM_TEMP = 301.05; // room temperature in Kelvin
const double RESISTOR_ROOM_TEMP = 10000.0;

double NTC10K_Read_Temperature(unsigned char canal);

#endif	/* NTC10K_H */

