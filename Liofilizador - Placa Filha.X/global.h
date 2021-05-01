/*-----------------------------------------------------------------------------/
 * File:   global.h                                                            /
 * Author: Neraildes                                                          /
 * Revisão 1.0.0                                                               /
 * Created on 16 de Fevereiro de 2018                                          /
 * Descrição:                                                                  /
 *		Configurações globais de todas as funções do Liofilizador              /
 * Metodo de Compilação                                                        /
 *      Licenciado Mensalmente com otimizaçao 1                                /
 -----------------------------------------------------------------------------*/
 
#ifndef GLOBAL_H
#define GLOBAL_H

#define _XTAL_FREQ 8000000
#define XTAL_8MHZ


//#define VACCUM_BOARD

#define PT100_UMIDADE

//#define NTC_BOARD  //Retorno para maquina real

//------------------------------------------------------------------------------

// PIC16F886 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = ON          // Code Protection bit (Program memory code protection is enabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.



#include "isr.h"
#include "global.h"
#include "protocolo.h"
#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define HEADER_LIOFILIZADOR 0xAABB
#define ENDERECO_PLACA      0x00
#define TODOS               0XFF

#define Lo(param) ((char *)&param)[0]
#define Hi(param) ((char *)&param)[1]
#define Lower(param)((char *)&param)[2]
#define High(param) ((char *)&param)[3]

#define HARDWARE_PIC_VERSION      100
#define HARDWARE_PROCULUS_VERSION PROCULUS_Read_Version()
#define FIRMWARE_PIC_VERSION      100
#define DATABASE_VERSION          100

#define TRUE  1
#define FALSE 0
#define ALL   0xFF

#define SOBE  1
#define DESCE 0

#define RELE_0 PORTCbits.RC0
#define RELE_1 PORTCbits.RC1
#define IN_1   PORTAbits.RA2
#define IN_2   PORTAbits.RA3


#define USART_BUFFER_SIZE   32+20

#define DEFAULT_LEDS 20 //multiplos de 10

/*----------------------------------------------------------------------------*/
struct {
    unsigned flag_usart_rx      : 1 ;
    unsigned flag_usart_error   : 1 ;
    unsigned flag_power_off     : 1 ;
    unsigned flag_led_tmr0      : 1 ;
    unsigned flag_led_usart     : 1 ;
    unsigned flag_led_memory    : 1 ;
    unsigned flag_global_hot    : 1 ;
    unsigned flag_main_loop_WDT : 1 ;
}  generic_status;
 
#define flag_usart_rx      generic_status.flag_usart_rx
#define flag_usart_error   generic_status.flag_usart_error
#define flag_power_off     generic_status.flag_power_off
#define flag_led_tmr0      PORTBbits.RB7
#define flag_led_usart     PORTBbits.RB6
#define flag_led_memory    PORTBbits.RB5
#define flag_global_hot    generic_status.flag_global_hot
#define flag_main_loop_WDT generic_status.flag_main_loop_WDT

union {
      unsigned char bits;
      struct {      
             unsigned flag_global_aquecimento : 1 ;
             unsigned flag_global_condensador : 1 ;
             unsigned flag_global_vacuo       : 1 ;
             };      
      } statuspower;    
      


#define flag_global_aquecimento  statuspower.flag_global_aquecimento     
#define flag_global_condensador  statuspower.flag_global_condensador
#define flag_global_vacuo        statuspower.flag_global_vacuo


//const unsigned char boardtype[]={"Mother Board",
//                                 "Vaccum Board",
//                                 "PT100 Board",
//                                 "NTC Board"};
#define Mother_Board 0
#define Vaccum_Board 1
#define PT100_Board  2
#define NTC_Board    3


#endif
