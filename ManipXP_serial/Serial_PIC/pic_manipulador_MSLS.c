#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses HS,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=19200, xmit=PIN_C6, rcv=PIN_C7)

#define SWITCH_RS_AD PIN_B1
#define LED_1 PIN_C2

char valor;
signed long valorint;
long temp;  
int i=0;
int j, k;
long angulos[7] = {1500, 1500, 1500, 1500, 1500, 1500, 1500};
long soma_angulos = 0;
int output = 0b00000000;
int Byte_array[] = {0, 0};


//#INT_RDA
void trata_rs232(){
   //////// ENVIA ANGULOS POR RS232 CASO CHAVE DESLIGADA ////////
   signed long ang;
   int n, m;
   n = getc();
   ang = (signed long)(angulos[n-1]-1500)*2/15;//+120;
   for (m = 0; m < 2; m++){ //TRANSMITE A VARIAVEL DE 16 BITS EM 2 VEZES DE 8
      putc(*(&ang + m));
   }
}

#int_timer1
void trata_t1() {
   ////////////////// FAZ PULSO ALTO ///////////////////
   bit_clear(output, i);
   output_d(output);
   if(i<7){
      ++i;
      bit_set(output, i);
      output_d(output);
      set_timer1(65536-angulos[i-1]);
   }else{
      ///////////// FAZ DELAY DE TEMPO BAIXO //////////////
      i=0;
      output=0;
      output_d(output);
      set_timer1(41736+soma_angulos); //set_timer1(55536);
      if(!input(SWITCH_RS_AD)){
         //////////// FAZ AQUISIÇÃO CASO CHAVE DESLIGADA //////////////
         for (k = 0; k < 7; ++k){
            set_adc_channel( k );
            delay_us(20);
            temp = read_adc();
            angulos[k] = (float)temp*1.7595+600; //angulos[k] = temp*2+500;
         }
         soma_angulos = angulos[0]+angulos[1]+angulos[2]+angulos[3]
                        +angulos[4]+angulos[5]+angulos[6];
         if(kbhit()){
            trata_rs232();
         }
         output_low(LED_1);
      }
   }
}


void main() {
   set_tris_a(0b11111111);
   set_tris_d(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );

   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 );
   enable_interrupts (global);
   //enable_interrupts(INT_RDA);
   enable_interrupts (int_timer1);
   set_timer1(0);

   while (true){
      output_high(LED_1);
      if(input(SWITCH_RS_AD)){
         //////// RECEBE ANGULOS POR RS232 CASO CHAVE LIGADA ////////
         for (j = 0; j < 7; ++j){
            putc(j+1);
            Byte_array[1] = getc();  //MS
            Byte_array[0] = getc();  //LS
            angulos[j] = *(long *)Byte_array;
         }
         soma_angulos = angulos[0]+angulos[1]+angulos[2]+angulos[3]
                        +angulos[4]+angulos[5]+angulos[6];         
      }
   }
}
