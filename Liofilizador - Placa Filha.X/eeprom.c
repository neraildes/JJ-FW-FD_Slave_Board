/*------------------------------------------------------------------------------
 * Descricao..................................Rotinas de persistencia de dados |
 * Autor......................................Neraildes da Silva               |
 * Data.......................................13/10/2017                       |
------------------------------------------------------------------------------*/



#include <xc.h>
#include "global.h"
#include "eeprom.h"



extern volatile unsigned int Delay_Led_Memory; //Verde


/*------------------------------------------------------------------------------
                                  B Y T E                                      |
------------------------------------------------------------------------------*/
void EEPROM_Write_Byte(unsigned char addr, unsigned char dado)
{
    unsigned char last_GIE;    
    Delay_Led_Memory=DEFAULT_LEDS;
    while(EECON1bits.WR) continue;     
        
       if(INTCONbits.GIE==1) 
          last_GIE=1;
       else
          last_GIE=0; 
        
       EEADR=addr;
       EEDATA=dado;
       EECON1bits.EEPGD=0;
       EECON1bits.WREN=1;
       INTCONbits.GIE=0;
       EECON2=0x55;
       EECON2=0xAA;
       EECON1bits.WR=1;            

       if(last_GIE==1) 
          INTCONbits.GIE=1;
       else
          INTCONbits.GIE=0;

       EECON1bits.WREN=0;        
       __delay_ms(30);
}


//------------------------------------------------------------------------------
unsigned char EEPROM_Read_Byte(unsigned char addr)
{
   Delay_Led_Memory=DEFAULT_LEDS;
   while(EECON1bits.WR)continue;

   EEADR=addr;   
   EECON1bits.EEPGD=0;
   EECON1bits.RD=1;
   __delay_ms(30);
   return EEDATA;
}


/*------------------------------------------------------------------------------
                          I  N  T  E  G  E  R                                  |
------------------------------------------------------------------------------*/
void EEPROM_Write_Integer(unsigned char addr, unsigned int dado)
{
      unsigned char *ptr;
      unsigned char i;
      unsigned char addr_memo;
      addr_memo=addr;
      ptr=(unsigned char *)&dado;
      for (i=0;i<2;i++)
          EEPROM_Write_Byte(addr++,*(ptr++));
      __delay_ms(30);
}

//------------------------------------------------------------------------------
unsigned int EEPROM_Read_Integer(unsigned char addr)
{
      unsigned int result;
      unsigned char *ptr;
      unsigned char i;
      ptr=(unsigned char *)&result;
      for (i=0;i<2;i++)
          *(ptr++)=EEPROM_Read_Byte(addr++);
      return result;
      __delay_ms(30);
}


/*------------------------------------------------------------------------------
                                    STRING                                     |
------------------------------------------------------------------------------*/
void EEPROM_Write_String(unsigned char addr,char *dado)
{
    while(*dado)
    {
     EEPROM_Write_Byte(addr,*dado);
     addr++;
     dado++;
    }
    EEPROM_Write_Byte(addr,*dado);
}

//------------------------------------------------------------------------------
void EEPROM_Read_String(unsigned char addr,char *dado)
{
    unsigned char c;    
    do{
        c=EEPROM_Read_Byte(addr++);        
        *(dado++)=c;
        //__delay_ms(5);
      }while(c);      
}
  



#ifndef NTC_BOARD
/*------------------------------------------------------------------------------
                 F  L  O  A  T      2 4        B I T S                         |
------------------------------------------------------------------------------*/
void EEPROM_Write_Float(unsigned char addr,float dado)
{
      unsigned char *ptr;
      unsigned char i;
      unsigned char addr_memo;
      addr_memo=addr;
      ptr=(unsigned char *)&dado;
      for (i=0;i<3;i++)
          EEPROM_Write_Byte(addr++,*(ptr++));
}

//------------------------------------------------------------------------------
float EEPROM_Read_Float(unsigned char addr)
{
      float result;
      unsigned char *ptr;
      unsigned char i;
      ptr=(unsigned char *)&result;
      for (i=0;i<3;i++)
          *(ptr++)=EEPROM_Read_Byte(addr++);
      return result;
}




/*------------------------------------------------------------------------------
                                D O U B L E                                    |
------------------------------------------------------------------------------*/
void EEPROM_Write_Double(unsigned char addr,double dado)
{
      unsigned char *ptr;
      unsigned char i;
      unsigned char addr_memo;
      addr_memo=addr;
      ptr=(unsigned char *)&dado;
      for (i=0;i<4;i++)
          EEPROM_Write_Byte(addr++,*(ptr++));
}


//------------------------------------------------------------------------------
float EEPROM_Read_Double(unsigned char addr)
{
      float result;
      unsigned char *ptr;
      unsigned char i;
      ptr=(unsigned char *)&result;
      for (i=0;i<4;i++)
          *(ptr++)=EEPROM_Read_Byte(addr++);
      return result;
}




/*------------------------------------------------------------------------------
                                L  O  N  G                                     |
------------------------------------------------------------------------------*/

void EEPROM_Write_Long24(unsigned char addr,long dado)
{
      unsigned char *ptr;
      unsigned char i;
      unsigned char addr_memo;
      addr_memo=addr;
      ptr=(unsigned char *)&dado;
      for (i=0;i<3;i++){
          EEPROM_Write_Byte(addr,*ptr);
          addr++;
          ptr++;
          }
}

//------------------------------------------------------------------------------
unsigned long EEPROM_Read_ULong24(unsigned char addr)
{
      unsigned long result;
      unsigned char *ptr;
      unsigned char i;
      ptr=(unsigned char *)&result;
      for (i=0;i<3;i++)
          *(ptr++)=EEPROM_Read_Byte(addr++);
      return result;
}





void EEPROM_Write_Long32(unsigned char addr,long dado)
{
      unsigned char *ptr;
      unsigned char i;
      unsigned char addr_memo;
      addr_memo=addr;
      ptr=(unsigned char *)&dado;
      for (i=0;i<4;i++)
           EEPROM_Write_Byte(addr++,*(ptr++));
}



//------------------------------------------------------------------------------
long EEPROM_Read_Long32(unsigned char addr)
{
      unsigned long result;
      unsigned char *ptr;
      unsigned char i;
      ptr=(unsigned char *)&result;
      for (i=0;i<4;i++)
          *(ptr++)=EEPROM_Read_Byte(addr++);
      return result;
}


#endif

