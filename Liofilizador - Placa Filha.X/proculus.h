/*-----------------------------------------------------------------------------/
 * File:   proculus.h 
 * Author: Neraildes
 * Revisão 1.0.0
 * Created on 17 de Outubro de 2017, 09:17
 * Descrição:
 *		Funções de comunicação com o display victor vision.
 *		Partnumber/Modelo: P48272V43C_T07
 *      Tamanho: 4.3 polegadas
 *      Resolução: 480x272
 -----------------------------------------------------------------------------*/

#ifndef PROCULUS_H
#define PROCULUS_H


#define Control_Register_Write 0x80
#define Control_Register_Read  0x81
#define VP_Write               0x82
#define VP_Read                0x83
#define Trend_Curve_Write      0x84

//---------------CONTROL REGISTER-----------------
#define VERSION_INFO           0x00
#define DEFAULT_BACKLIGHT      0x01
#define ACTIVE_BUZZER          0x02
#define PIC_ID                 0x03


#define NAME_TASK_SIZE         0x0A


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define PROCULUS_Header            0x5AA5
#define CONSTANT_FIX_01            0x0683
#define CONSTANT_FIX_02            0x0582
#define PROCULUS_Reg_Read          0x0381
#define PROCULUS_Reg_Write         0x0380

#define PROCULUS_DATA_SIZE         20

#define WINDOW_TELA_01 1
#define WINDOW_TELA_02 2
#define WINDOW_TELA_03 3
#define WINDOW_TELA_04 4
#define WINDOW_TELA_05 5

#define TIME_AFTER_SEND_PROCULUS_COMMAND 1


typedef struct {
    unsigned int  header;
    unsigned char size;
    unsigned char function;    
    unsigned int  VP;
    unsigned char length;
    unsigned int  data[PROCULUS_DATA_SIZE];      
    }t_proculus;


void PROCULUS_Control_Register_Write(unsigned char *vetor, unsigned char size);
void PROCULUS_Control_Register_Read(unsigned char reg, unsigned char size, unsigned char *retorno);

void PROCULUS_VP_Write(unsigned int vp,char *vetor,char size);
void PROCULUS_VP_Read(unsigned int vp,char *vetor,char size);

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


//--------------------------------- byte ---------------------------------------
void PROCULUS_VP_Write_Byte(unsigned int vp, unsigned char value);
unsigned char PROCULUS_VP_Read_Byte(unsigned int vp);


//-------------------------------- int16 ---------------------------------------
void PROCULUS_VP_Write_UInt16(unsigned int vp, unsigned int value);
unsigned int PROCULUS_VP_Read_UInt16(unsigned int vp);


//-------------------------------- long ----------------------------------------
long PROCULUS_VP_Read_Long32(unsigned int vp);
void PROCULUS_VP_Write_Long32(unsigned int vp,long value);

//-------------------------------- float ---------------------------------------
/*
void PROCULUS_VP_Write_Float24(unsigned int vp, float value);
float PROCULUS_VP_Read_Float24(unsigned int vp);
*/ 
/*
void PROCULUS_VP_Write_Float32(unsigned int vp, float value);
float PROCULUS_VP_Read_Float32(unsigned int vp);
*/


//-------------------------------- double --------------------------------------
void PROCULUS_VP_Write_Double24(unsigned int vp, double value);
double PROCULUS_VP_Read_Double24(unsigned int vp);

void PROCULUS_VP_Write_Double32(unsigned int vp, double value);
double PROCULUS_VP_Read_Double32(unsigned int vp);

//-----------------------------string-------------------------------------------
void PROCULUS_VP_Write_String(unsigned int vp,char *text);
void PROCULUS_VP_Read_String(unsigned int vp,char *text);   


unsigned char PROCULUS_Read_Version(void);
void PROCULUS_Buzzer(unsigned int time_ms_x_10);
void PROCULUS_Show_Screen(unsigned int screen);
void PROCULUS_Reset(void);

void PROCULUS_Buffer_to_Proculus(t_proculus *proculus);

void PROCULUS_Delay(unsigned int tempo_ms);   
    
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



#endif	/* PROCULUS_H */

