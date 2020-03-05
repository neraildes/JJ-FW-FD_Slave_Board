/* 
 * File:   24C512.h
 * Author: Nera
 *
 * Created on 22 de Maio de 2018, 09:01
 */

#define EE_WRITE 0
#define EE_READ  1

#ifndef EEPROM_24C512_H
#define	EEPROM_24C512_H

void EEPROM_24C512_Write_Byte(unsigned char dev_add, unsigned int mem_add, unsigned char data);
unsigned char EEPROM_24C512_Read_Byte(unsigned char dev_add, unsigned int mem_add);

void EEPROM_24C512_Write_Int(unsigned char dev_add, unsigned int mem_add, unsigned int data);
unsigned int EEPROM_24C512_Read_Int(unsigned char dev_add, unsigned int mem_add);

void EEPROM_24C512_Write_Str(unsigned char dev_add, unsigned int mem_add,char *data);
void EEPROM_24C512_Read_Str(unsigned char dev_add, unsigned int mem_add,char *buffer);

#endif	/* 24C512_H */

