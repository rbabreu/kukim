#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#include <usart.c>

char valor;
signed long valorint;
long temp;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
long t;
long i;
long tempodelay;

void main () {
   set_tris_a(0b11111111);
   set_tris_B(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );
   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   set_adc_channel( 1 );
   delay_us(10);
   usart_inicializa (25,1);  //ESSES PARÂMETROS SELECIONAM BAUD RATE DE 9600 BPS
   while (true) {
      delay_ms(5);
      temp = read_adc();
      delay_ms(5);
      valor = usart_recebe ();
      valorint = valor;
      valorint -= 127;
      tempodelay = (valorint*15)/2+1500;
      rcsta.cren = 1;
      if(valorint == 0){
         t = temp*2+500;
         delay_ms(19);
         output_high(PIN_B7);
         set_timer1(0);
         while (get_timer1() < t);
         output_low(PIN_B7);
      }
      else{
         delay_ms(19);
         output_b(0b11111111);
         set_timer1(0);
         i=0;
         while (i < 2400){
            i=get_timer1();
            if(i>=2300)
               output_low(PIN_B1);
            if(i>=2175)
               output_low(PIN_B2);
            if(i>=1800)
               output_low(PIN_B3);
            if(i>=1500)
               output_low(PIN_B4);
            if(i>=1200)
               output_low(PIN_B5);
            if(i>=825)
               output_low(PIN_B6);
            if(i>=650)
               output_low(PIN_B7);
         }
         output_b(0b00000000);

      }
      temp = temp * 3;
      for (i = 0; i < 2; i++){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
        usart_transmite(*(&temp + i)); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
      }
   }
}
