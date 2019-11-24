/*-----------------------------------------------------------------------------/
 * File:   eeprom.h
 * Author: Neraildes
 * Revisão 1.0.0
 * Created on 13 de Outubro de 2017, 06:25
 * Descrição:
 *		Leitura e gravação de EEEPROM interna do microcontrolador PIC.
 -----------------------------------------------------------------------------*/


#ifndef EEPROM_H
#define EEPROM_H

void          EEPROM_Write_Byte(unsigned char addr, unsigned char dado);
unsigned char EEPROM_Read_Byte(unsigned char addr);
void          EEPROM_Write_Integer(unsigned char addr, unsigned int dado);
unsigned int  EEPROM_Read_Integer(unsigned char addr);
void          EEPROM_Write_Float(unsigned char addr,float dado);
float         EEPROM_Read_Float(unsigned char addr);
void          EEPROM_Write_Double(unsigned char addr,double dado);
float         EEPROM_Read_Double(unsigned char addr);

void          EEPROM_Write_Long24(unsigned char addr,long dado);
unsigned long EEPROM_Read_Long24(unsigned char addr);
void          EEPROM_Write_Long32(unsigned char addr,long dado);
long          EEPROM_Read_Long32(unsigned char addr);

void          EEPROM_Write_String(unsigned char addr,char *dado);
void          EEPROM_Read_String(unsigned char addr,char *dado);

#endif
