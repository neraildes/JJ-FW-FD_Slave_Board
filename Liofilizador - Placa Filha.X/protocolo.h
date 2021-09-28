/* 
 * File:   protocolo.h
 * Author: Nera
 *
 * Created on 2 de Junho de 2018, 07:09
 */

#include "global.h"

#ifndef PROTOCOLO_H
#define	PROTOCOLO_H

#define HEADER_SLAVE 0xAABB
#define DEVICE_BOARD (((0x0F)&(~PORTB)))




#define SIZE_VETOR_PROTOCOL 52

typedef struct {
        unsigned int  header;
        unsigned char origem;
        unsigned char destino;        
        unsigned char command;
        unsigned char size;
        char value[SIZE_VETOR_PROTOCOL];
} t_usart_protocol;

#define CODIGO_ERRO_PROTOCOLO            "E0"
#define CODIGO_ERRO_DISPOSITIVO_INVALIDO "E1"
#define CODIGO_ERRO_SEM_RESPOSTA         "E2"

#define COMMAND_SYNCRONIZE      0x00 
#define COMMAND_LOAD_WORK       0x01 
#define COMMAND_SAVE_LOG        0x02 
#define COMANDO_QUEM_EH_VOCE    0x03 
#define COMMAND_RUN_ON          0x04
#define COMMAND_RUN_OFF         0x05
#define COMMAND_UPDATE_ON       0x06
#define COMMAND_UPDATE_OFF      0x07
//------- INTERNAL EEPROM-----------
#define COMMAND_IEE_W_BYTE      0x08
#define COMMAND_IEE_R_BYTE      0x09
#define COMMAND_IEE_W_INT       0x0A
#define COMMAND_IEE_R_INT       0x0B
#define COMMAND_IEE_W_STR       0x0C
#define COMMAND_IEE_R_STR       0x0D
#define COMMAND_IEE_W_BUF       0x0E
#define COMMAND_IEE_R_BUF       0x0F
#define COMMAND_IEE_FILL_ALL    0x10 
//-------- EXTERNAL EEPROM ---------
#define COMMAND_EEE_W_BYTE      0x11
#define COMMAND_EEE_R_BYTE      0x12
#define COMMAND_EEE_W_INT       0x13
#define COMMAND_EEE_R_INT       0x14
#define COMMAND_EEE_W_STR       0x15
#define COMMAND_EEE_R_STR       0x16
#define COMMAND_EEE_W_BUF       0x17
#define COMMAND_EEE_R_BUF       0x18
#define COMMAND_EEE_FILL_ALL    0x19
//-------- ANALOGIC CHANNEL --------
#define COMMAND_READ_ANALOG     0X1A
#define COMMAND_SCHD_ANALOG     0X1B
#define COMMAND_SCHD_START      0X1C
#define COMMAND_SCHD_STOP       0X1D
//----------- PROCULUS -------------
#define COMMAND_LCD_W_VP_INT    0X20
#define COMMAND_LCD_R_VP_INT    0X21
#define COMMAND_LCD_W_VP_STR    0X22
#define COMMAND_LCD_R_VP_STR    0X23
#define COMMAND_PROCULUS_Buzzer 0X24
#define COMMAND_LDC_PAGE        0X25
#define COMMAND_CONTROL_ACTIVE  0X26
#define COMMAND_READ_TOTALBOARD 0X27
//...
#define COMMAND_PROC_CLOCK_R    0X2D
#define COMMAND_CLK_RTC_R       0X2E
#define COMMAND_CLK_RTC_W       0X2F
//---------------SLAVE--------------
#define COMMAND_RELAY           0x30
#define COMMAND_INPUT_LED       0X31
//...
//----------------------------------
#define COMMAND_GLOBAL_HOT      0x40
#define COMMAND_VERSION         0x41
#define COMMAND_SHOW_PROGRAM    0x42
#define COMMAND_FORMAT          0x43
#define COMMAND_UPLOAD_PRG      0x44
#define COMMAND_EEE_R_32B       0x45  //Ler    EEPROM EXTERNA COM 32 Bits
#define COMMAND_EEE_W_32B       0x46  //Gravar EEPROM EXTERNA COM 32 Bits (Não Implementado)
#define COMMAND_EEE_R_BUF_DIR   0x47  //Ler    Buvffer Direto
#define COMMAND_EEE_W_BUF_DIR   0x48  //Gravar Buffer Direto
//...
#define COMMAND_FLUIDO_TERMICO  0x50  //Comando para placa aquecedora de fluido térmico 
//----------------------------------



/*
#define COMMAND_SYNCRONIZE      0x00 
#define COMMAND_LOAD_WORK       0x01 
#define COMMAND_SAVE_LOG        0x02 
#define COMANDO_QUEM_EH_VOCE    0x03 
#define COMMAND_RUN_ON          0x04
#define COMMAND_RUN_OFF         0x05
#define COMMAND_UPDATE_ON       0x06
#define COMMAND_UPDATE_OFF      0x07
//------- INTERNAL EEPROM-----------
#define COMMAND_IEE_W_BYTE      0x08
#define COMMAND_IEE_R_BYTE      0x09
#define COMMAND_IEE_W_INT       0x0A
#define COMMAND_IEE_R_INT       0x0B
#define COMMAND_IEE_W_STR       0x0C
#define COMMAND_IEE_R_STR       0x0D
#define COMMAND_IEE_W_BUF       0x0E
#define COMMAND_IEE_R_BUF       0x0F
#define COMMAND_IEE_FILL_ALL    0x10 
//-------- EXTERNAL EEPROM ---------
#define COMMAND_EEE_W_BYTE      0x11
#define COMMAND_EEE_R_BYTE      0x12
#define COMMAND_EEE_W_INT       0x13
#define COMMAND_EEE_R_INT       0x14
#define COMMAND_EEE_W_STR       0x15
#define COMMAND_EEE_R_STR       0x16
#define COMMAND_EEE_W_BUF       0x17
#define COMMAND_EEE_R_BUF       0x18
#define COMMAND_EEE_FILL_ALL    0x19
//-------- ANALOGIC CHANNEL --------
#define COMMAND_READ_ANALOG     0X1A
#define COMMAND_SCHD_ANALOG     0X1B
#define COMMAND_SCHD_START      0X1C
#define COMMAND_SCHD_STOP       0X1D
//----------- OUTROS ---------------
#define COMMAND_CLK_PIC_W       0X1E
#define COMMAND_CLK_PIC_R       0X1F
#define COMMAND_LDC_PAGE        0X20
#define COMMAND_PROCULUS_Buzzer 0X21
#define COMMAND_LCD_W_VP_INT    0X22
#define COMMAND_LCD_R_VP_INT    0X23
#define COMMAND_LCD_W_VP_STR    0X24
#define COMMAND_LCD_R_VP_STR    0X25
//---------------SLAVE--------------
#define COMMAND_RELAY           0x26
#define COMMAND_INPUT_LED       0X27
//----------------------------------
#define COMMAND_READ_GRAUS_TEXT 0x28
#define COMMAND_READ_GRAUS_REAL 0x29
//----------------------------------
#define COMMAND_GLOBAL_HOT      0x2A
//----------------------------------
#define COMMAND_VERSION         0X2B

*/

char Package_Usart_is_for_me();




#endif	/* PROTOCOLO_H */
