   #include "global.h"
   #include "my_delay.h"
   
   
   void my_delay_ms(long time){
        long i;
        for(i=0;i<time;i++)
            {           
            __delay_ms(1);
            }
   }   
   
   void my_delay_ms_WDT(long time){
        long i;
        for(i=0;i<time;i++)
            {
            asm("CLRWDT");
            __delay_ms(1);
            }
   }