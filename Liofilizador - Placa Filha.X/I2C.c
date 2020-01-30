#include <xc.h>
#include "global.h"
#include "I2C.h"
#include "util.h"


extern unsigned int Delay_Led_Memory;

void I2C_Master_Init(const unsigned long c)
{
  #if ((defined __18F4620) || (defined __18F4525)) 
  SSPCON1 = 0b00101000;            //SSP Module as Master
  #endif
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ /(4*c))-1; //Setting Clock Speed
  SSPSTAT = 0;
  TRISCbits.TRISC3 = 1;                   //Setting as input as given in datasheet
  TRISCbits.TRISC4 = 1;                   //Setting as input as given in datasheet
}

void I2C_Slave_Init(short address) 
{
  SSPSTAT = 0x80;    
  SSPADD = address; //Setting address
  #if ((defined __18F4620) || (defined __18F4525)) 
  SSPCON1 = 0x36;   //As a slave device         
  #endif
  SSPCON2 = 0x01;
  TRISCbits.TRISC3 = 1;       //Setting as input as given in datasheet
  TRISCbits.TRISC4 = 1;       //Setting as input as given in datasheet
  INTCONbits.GIE = 1;          //Global interrupt enable
  INTCONbits.PEIE = 1;         //Peripheral interrupt enable
  PIR1bits.SSPIF = 0;        //Clear interrupt flag
  PIE1bits.SSPIE = 1;        //Synchronous serial port interrupt enable
}






char I2C_Master_Wait(void)
{  
  char tempo;
  tempo=200;  
  while ((SSPSTAT & 0b00000100) || (SSPCON2 & 0b00011111)) //Transmit is in progress
      {
      if(tempo) 
         tempo--;
      else
         break;
      __delay_ms(1);
      }    
  return tempo ;
}

void I2C_Master_Start(void)
{  
  INTCONbits.GIE=0;
  Delay_Led_Memory=DEFAULT_LEDS;  
  if(I2C_Master_Wait())
    {  
    SSPCON2bits.SEN = 1;             //Initiate start condition
    }
}

void I2C_Master_RepeatedStart(void)
{
  if(I2C_Master_Wait())
    {  
    SSPCON2bits.RSEN = 1;           //Initiate repeated start condition
    }
}

void I2C_Master_Stop(void)
{
  if(I2C_Master_Wait())
    {  
    SSPCON2bits.PEN = 1;           //Initiate stop condition
    }
  INTCONbits.GIE=1;
}

void I2C_Master_Write(unsigned d)
{
  if(I2C_Master_Wait())
    {  
    SSPBUF = d;         //Write data to SSPBUF
    }
}

unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;  
  if(I2C_Master_Wait())SSPCON2bits.RCEN = 1;
  if(I2C_Master_Wait())temp = SSPBUF;      //Read data from SSPBUF
  if(I2C_Master_Wait())
    {  
    SSPCON2bits.ACKDT = (a)?0:1;    //Acknowledge bit
    SSPCON2bits.ACKEN = 1;          //Acknowledge sequence  
    return temp;
    }
  else
    {  
    return 0x00;  
    }
}