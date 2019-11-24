/* 
 * File:   pt100.h
 * Author: Nera
 *
 * Created on 2 de Maio de 2018, 10:36
 */

#ifndef PT100_H
#define	PT100_H

const double PT100_CONSTANT  =  -10.73;
const double PT100_TRANSLADA = -1.3;


double PT100_Read_Temperature(unsigned char canal);

#endif	/* PT100_H */

