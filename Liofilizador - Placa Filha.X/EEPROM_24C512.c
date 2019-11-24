#include "global.h"
#include "I2C.h"
#include "EEPROM_24C512.h"


void EEPROM_24C512_Write_Byte(unsigned char chip_add, unsigned int mem_add, unsigned char data){
     I2C_Master_Start();                   // start no barramento i2c
     I2C_Master_Write(0xA0|(chip_add<<1));
     I2C_Master_Write(Hi(mem_add));   // endereço da eerprom onde será salvo o dado High
     I2C_Master_Write(Lo(mem_add));   // endereço da eerprom onde será salvo o dado Low     
     I2C_Master_Write(data);    // grava a variável A no endereço 00 da memória serial     
     I2C_Master_Stop();         // condição de stop na comunicação i2c      
     __delay_ms(5);
}


unsigned char EEPROM_24C512_Read_Byte(unsigned char chip_add, unsigned int mem_add){
 unsigned char retorno;
 I2C_Master_Start();                   // start no barramento i2c 
 I2C_Master_Write(0xA0|(chip_add<<1));
 I2C_Master_Write(Hi(mem_add));        // endereço da eeprom onde será lido o byte h
 I2C_Master_Write(Lo(mem_add));        // endereço da eeprom onde será lido o byte l 
 I2C_Master_RepeatedStart();       // re-start no barramento  
 I2C_Master_Write(0xA1|(chip_add<<1));
 retorno = I2C_Master_Read(0);     // leitura do endereço 00h na variável_A 
 I2C_Master_Stop();                // finaliza a comunicação i2c
 return retorno;
}


void EEPROM_24C512_Write_Int(unsigned char chip_add, unsigned int mem_add, unsigned int data){
     I2C_Master_Start();              // start no barramento i2c
     I2C_Master_Write(0xA0|(chip_add<<1));
     I2C_Master_Write(Hi(mem_add));   // endereço da eerprom onde será salvo o dado High
     I2C_Master_Write(Lo(mem_add));   // endereço da eerprom onde será salvo o dado Low     
     I2C_Master_Write(Hi(data));      // grava a variável A no endereço 00 da memória serial     
     I2C_Master_Write(Lo(data));      // grava a variável A no endereço 00 da memória serial          
     I2C_Master_Stop();         // condição de stop na comunicação i2c      
     __delay_ms(5);     
}

unsigned int EEPROM_24C512_Read_Int(unsigned char chip_add, unsigned int mem_add){
     unsigned int retorno=0;     
     I2C_Master_Start();                   // start no barramento i2c 
     I2C_Master_Write(0xA0|(chip_add<<1));
     I2C_Master_Write(Hi(mem_add));        // endereço da eeprom onde será lido o byte h
     I2C_Master_Write(Lo(mem_add));        // endereço da eeprom onde será lido o byte l 
     I2C_Master_RepeatedStart();           // re-start no barramento  
     I2C_Master_Write(0xA1|(chip_add<<1));
     retorno =(I2C_Master_Read(1)<<8);   // leitura do endereço 00h na variável_A 
     retorno+= I2C_Master_Read(0);       // leitura do endereço 00h na variável_A 
     I2C_Master_Stop();                  // finaliza a comunicação i2c                 
     return retorno ;
}



void EEPROM_24C512_Write_Str(unsigned char chip_add, unsigned int mem_add,char *data){
     I2C_Master_Start();              // start no barramento i2c
     I2C_Master_Write(0xA0|(chip_add<<1));
     I2C_Master_Write(Hi(mem_add));   // endereço da eerprom onde será salvo o dado High
     I2C_Master_Write(Lo(mem_add));   // endereço da eerprom onde será salvo o dado Low     
     while(*data){
          I2C_Master_Write(*data); //Grava String na EEPROM
          data++;
          }
     I2C_Master_Write(0);
     I2C_Master_Stop();                 // condição de stop na comunicação i2c           
     __delay_ms(5);     
}



void EEPROM_24C512_Read_Str(unsigned char chip_add, unsigned int mem_add,char *texto){
 I2C_Master_Start();                   // start no barramento i2c 
 I2C_Master_Write(0xA0|(chip_add<<1));
 I2C_Master_Write(Hi(mem_add));        // endereço da eeprom onde será lido o byte h
 I2C_Master_Write(Lo(mem_add));        // endereço da eeprom onde será lido o byte l 
 I2C_Master_RepeatedStart();           // re-start no barramento  
 I2C_Master_Write(0xA1|(chip_add<<1));
 while((*texto)!=0)
      {
      *texto=I2C_Master_Read(1);
      texto++;
      }
 (*texto)=I2C_Master_Read(0);
 I2C_Master_Stop();                // finaliza a comunicação i2c
 __delay_ms(5); 
}
