#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#include <usart.c>

char valor;
signed long valorint;
long temp;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
long i, j, k, l;
long tempodelay;
long angulos[7]={1500, 1500, 1500, 1500, 1500, 1500, 1500};

#int_timer1
void trata_t1 ()
{
   if(l == 10){
      for (k = 0; k < 7; ++k){
         set_adc_channel( k );
         delay_us(20);
         temp = read_adc();
         angulos[k] = temp*2+500;
      }
   }
   output_b(0b11111111);
   set_timer1(0);
   i=0;
   while (i < 2600){
      i = get_timer1();
      if(i>=angulos[0])
         output_low(PIN_B1);
      if(i>=angulos[1])
         output_low(PIN_B2);
      if(i>=angulos[2])
         output_low(PIN_B3);
      if(i>=angulos[3])
         output_low(PIN_B4);
      if(i>=angulos[4])
         output_low(PIN_B5);
      if(i>=angulos[5])
         output_low(PIN_B6);
      if(i>=angulos[6])
         output_low(PIN_B7);
   }
   output_b(0b00000000);
   l = 10; ////////////////////// COLOCADA DEPOIS
   set_timer1(45536);
}

void main () {
   set_tris_a(0b11111111);
   set_tris_B(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );
   usart_inicializa (25,1);  //ESSES PARÂMETROS SELECIONAM BAUD RATE DE 9600 BPS

   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   enable_interrupts (global);
	enable_interrupts (int_timer1);
   set_timer1(0);
   l=10;

   while (true) {
      for (j = 0; j < 7; ++j){
         usart_transmite(j+1);         
         rcsta.cren = 1;
         valorint = usart_recebe ();
         l=j;
         valorint = valorint - 100;
         angulos[j] = (valorint*15)/2 + 1500;//temp*2+500;         
         delay_us(20);
      }

//      temp = temp * 3;
//      for (i = 0; i < 2; i++){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
//        usart_transmite(*(&temp + i)); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
//      }
   }
}
