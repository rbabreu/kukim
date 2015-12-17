#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7) //#include <usart.c> //para usart

long valorint;
long temp;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
signed int m=0;
int n = 0;
int p = 0;
long angulos[7] = {1500, 1500, 1500, 1500, 1500, 1500, 1500};

int ShortToBytes[2];
int LS;
int MS;

#int_rda
void trata_rs232(){
   n = getc();
   if(n<=7){
      delay_us(5);
      valorint = angulos[n-1];
      for (m = 1; m >=0; --m){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
        putc(*(&valorint + m)); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
      }
   }
   /*
   *(long *)ShortToBytes = valorint;
   LS = ShortToBytes[0];
   MS = ShortToBytes[1];
   putc(MS);
   putc(LS);*/
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
         angulos[p] = temp*2+500;
      }
      /*if(kbhit()){  //pode ser usado isso ao invés da interrupção
         n = getc();
         delay_us(5);
         valorint = angulos[n-1];
         //for (m = 1; m >=0; --m){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
         //  putc(*(&valorint + m)); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
         //}
         *(long *)ShortToBytes = valorint;
         LS = ShortToBytes[0];
         MS = ShortToBytes[1];
         putc(MS);
         putc(LS);
      }*/
   }
}
