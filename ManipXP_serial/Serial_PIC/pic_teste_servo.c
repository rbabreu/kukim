#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //#include <usart.c> //para usart


long temp = 0;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
long j = 0;
long i, k;
long angulo = 1500;
int output = 0b00000000;

#int_timer1
void trata_t1 ()
{
   temp = read_adc();
   angulo = temp*2+500;   
   bit_set(output, 7);
   output_b(output);
   set_timer1(0);
   j=0;
   while(j<angulo)
      j=get_timer1();
   //for(j=0; j<angulo; j=get_timer1());// delay de tempo alto
   bit_clear(output, 7);
   output_b(output); //ver definição de pinos da porta B em 18F877.h
   k=j-angulo;
   set_timer1(45536+angulo); // delay de tempo baixo // 65536-(XTclock/4/prescaler)*Tpulsobaixo/7 = 65536-2857
}

void main() {
   set_tris_a(0b11111111);
   set_tris_B(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );

   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   enable_interrupts (global);
	enable_interrupts (int_timer1);
   set_timer1(0);
   set_adc_channel( 0 );

   while (true){
      for (i = 0; i < 2; i++){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
        putc(*(&k + i)); //A FUNÇÂO SÓ TRANSMITE VALOR DE 8 BITS (char)
      }
   } 
}
