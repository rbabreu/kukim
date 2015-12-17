#include <16f877a.h>
#device ADC=10  //NECESS�RIO PARA HABILITAR CONVERS�O EM 10 BITS
#fuses HS,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7)
#include <24256.c>

#define SWITCH_RS_AD PIN_B1
#define LED_1 PIN_C2

char valor;
signed long valorint;
long temp;  //ESSA VARI�VEL GUARDA AT� VALOR DE 16 BITS
int i=0;
int j, k;
long angulos[7] = {1500, 1500, 1500, 1500, 1500, 1500, 1500};
int output = 0b00000000;
int ang = 0;
long int endereco = 0;


WRITE_LONG_EXT_EEPROM(long int n, long data) {
   int c;
   for (c = 0; c < 2; c++)
     write_ext_eeprom(c + n, *(&data + c) );
}

long READ_LONG_EXT_EEPROM(long int n) {
   int c;
   long data;
   for (c = 0; c < 2; c++)
     *(&data + c) = read_ext_eeprom(c + n);
   return(data);
}

#int_timer1
void trata_t1() {
   ////////////////// FAZ PULSO ALTO ///////////////////
   bit_clear(output, i);
   output_d(output);
   if(i<7){
      if(j==0)
         output_low(LED_1);
      ++i;
      bit_set(output, i);
      output_d(output);
      set_timer1(65536-angulos[i-1]);
   }else{
      ///////////// FAZ DELAY DE TEMPO BAIXO //////////////
      i=0;
      output=0;
      output_d(output);
      set_timer1(55536);
      if(!input(SWITCH_RS_AD)){
         ////////////////// FAZ AQUISI��O ////////////////////
         for (k = 0; k < 6; ++k){
            set_adc_channel( k );
            delay_us(20);
            temp = read_adc();
            //angulos[k] = temp*2+500;
            angulos[k] = (float)temp*1.76+600;
         }
         angulos[6] = READ_LONG_EXT_EEPROM(endereco);
         /*if(kbhit()){
            k = getc();
            ang = (signed long)(angulos[k]-1500)*2/15+120;
            putc(ang);
         }*/
      }
   }
}

#INT_EXT
void trata_botao() {
   set_adc_channel( 6 );
   delay_us(20);
   temp = read_adc();
   temp = (float)temp*1.76+600;
   endereco+=2;
   WRITE_LONG_EXT_EEPROM(endereco, temp);
}



void main() {
   set_tris_a(0b11111111);
   set_tris_d(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );

   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   enable_interrupts (global);
   init_ext_eeprom();
   enable_interrupts(INT_EXT);
   enable_interrupts (int_timer1);
   set_timer1(0);
   while (true){
      output_high(LED_1);
      if(input(SWITCH_RS_AD)){
         ////////////// FAZ COMUNICA��O SERIAL ///////////////
         for (j = 0; j < 7; ++j){
            putc(j+1);
            valorint = getc();
            valorint = valorint - 100;
            angulos[j] = valorint*15/2 + 1500;
         }
      }
   }
}
