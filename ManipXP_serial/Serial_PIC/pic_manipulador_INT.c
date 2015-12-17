#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,WDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7, RESTART_WDT)

#define SWITCH_RS_AD PIN_B1
#define LED_1 PIN_C2

#define N_GDL 4

signed long valorint;
long temp;
int i=0;
int j, k;
long angulos[7] = {1500, 1500, 1500, 1500, 1500, 1500, 1500};
long soma_angulos = 0;
int output = 0b00000000;
int ang;

#int_timer1
void trata_t1() {
   ////////////////// FAZ PULSO ALTO ///////////////////
   bit_clear(output, i-1);
   output_d(output);
   if(i<7){
      bit_set(output, i);
      output_d(output);
      set_timer1(65536-angulos[i]);
      ++i;
   }else{
      ///////////// FAZ DELAY DE TEMPO BAIXO //////////////
      i=0;
      output=0;
      output_d(output);
      if(!input(SWITCH_RS_AD)){
         //////////// FAZ AQUISIÇÃO CASO CHAVE DESLIGADA //////////////
         for (k = 0; k < N_GDL; ++k){
            set_adc_channel( k );
            delay_us(20);
            temp = read_adc();
            angulos[k] = (float)temp*1.75953+500;//angulos[k] = temp*2+500; 
         }
         soma_angulos = angulos[0]+angulos[1]+angulos[2]+angulos[3]
                        +angulos[4]+angulos[5]+angulos[6];
      }
      set_timer1(47000+soma_angulos); //set_timer1(55536);
   }
}


void main() {
   set_tris_a(0b11111111);
   set_tris_d(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );
   
   setup_wdt(WDT_1152MS);
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   enable_interrupts (global);
   //enable_interrupts(INT_RDA);
   enable_interrupts (int_timer1);
   set_timer1(0);

   while (true){
      restart_wdt();
      if(input(SWITCH_RS_AD)){
         output_high(LED_1);
         //////// RECEBE ANGULOS POR RS232 CASO CHAVE LIGADA ////////
         for (j = 0; j < N_GDL; ++j){
            putc(j+1);
            valorint = getc();
            valorint = valorint - 100;
            angulos[j] = valorint*15/2 + 1500;
         }
         soma_angulos = angulos[0]+angulos[1]+angulos[2]+angulos[3]
                        +angulos[4]+angulos[5]+angulos[6];
      }
      else{
         output_low(LED_1);
         //////// ENVIA ANGULOS POR RS232 CASO CHAVE DESLIGADA ////////
         for (j = 0; j < N_GDL; ++j){
            ang = (signed long)(angulos[j]-1500)*2/15+121;//(float)((angulos[n-1]-600)/1.7595)+1;
            putc(j+1);
            putc(ang);
            valorint = getc(); //só para sincronizar
         }
      }
   }
}
