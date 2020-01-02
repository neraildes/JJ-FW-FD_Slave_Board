/* 
 * File:   24C1025.h
 * Author: Nera
 *
 * Created on 22 de Maio de 2018, 09:01
 */



#ifndef EEPROM_24C1025_H
#define	EEPROM_24C1025_H

#define EE_WRITE 0
#define EE_READ  1

void EEPROM_24C1025_Write_Buffer(unsigned char chip_add, 
                                  unsigned long mem_add,
                                 unsigned char sizedata,
                                            char *data);

void EEPROM_24C1025_Read_Buffer(unsigned char chip_add, 
                                 unsigned long mem_add,
                                unsigned char sizedata,
                                           char *data);

void EEPROM_24C1025_Read_Str(unsigned char chip_add, unsigned long mem_add,char *texto);
void EEPROM_24C1025_Write_Str(unsigned char chip_add, unsigned long mem_add,char *data);


void EEPROM_24C1025_Write_Byte(unsigned char chip_add, unsigned long mem_add, char data);
unsigned char EEPROM_24C1025_Read_Byte(unsigned char chip_add, unsigned long mem_add);
void EEPROM_24C1025_Write_Int(unsigned char chip_add, unsigned long mem_add, int data);
unsigned int EEPROM_24C1025_Read_Int(unsigned char chip_add, unsigned long mem_add);

void EEPROM_24C1025_Fill_All(unsigned char chip_add, unsigned char value);





#endif	/* 24C1025_H */

