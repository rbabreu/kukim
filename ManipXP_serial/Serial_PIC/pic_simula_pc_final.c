#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7) //#include <usart.c> //para usart

long valorint;
long temp;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
int n = 0;
int p = 0;
long angulos[7] = {100, 100, 100, 100, 100, 100, 100};

#int_rda
void trata_rs232(){
   n = getc();
   if(n<=7){
      delay_us(5);
      valorint = angulos[n-1];
      putc(valorint); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
   }
}
void main() {
   set_tris_a(0b11111111);
   set_tris_b(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );
   enable_interrupts (global);
   enable_interrupts (int_rda);

   while (true){
      for (p = 0; p < 7; ++p){
         set_adc_channel( p );
         delay_us(20);
         temp = read_adc();
         angulos[p] = temp/5.12;//(temp*4-500)/15;
      }
   }
}
