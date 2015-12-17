#include <16f877a.h>
#device ADC=10  //NECESSÁRIO PARA HABILITAR CONVERSÃO EM 10 BITS
#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP
#use delay(clock=4000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) //#include <usart.c> //para usart

char valor;
signed long valorint;
long temp;  //ESSA VARIÁVEL GUARDA ATÉ VALOR DE 16 BITS
int i, j, k, l;
long tempodelay;
long angulos[7] = {1500, 1500, 1500, 1500, 1500, 1500, 1500};
long angordem[7] = {1, 2, 3, 4, 5, 6, 7};
long angulo = 1500;
int output = 0b00000000;


void Prepara(long a[], long c[]){
    //int i,j;
    long b;
    for(k=0; k<7; ++k)
        c[k]=k+1;
    for(k=0; k<7; ++k){
        for(l=k; l<7; ++l){
            if(a[l]<a[k]){
                //ordena os valores de "a"
                b=a[k];
                a[k]=a[l];
                a[l]=b;
                //guarda a ordem dos valores em "c"
                b=c[k];
                c[k]=c[l];
                c[l]=b;
            }
        }
    }
   // for(k=6; k>0; --k){
   //    a[k] = a[k]-a[k-1]+1;  //prepara para timer //+1 so para timer não ficar em cima do estouro (verificar)
   // }
}

#int_timer1
void trata_t1() {
   bit_clear(output, angordem[i]);
   output_b(output); //ver definição de pinos da porta B em 18F877a.h
   if(i<6){
      ++i;
      bit_set(output, angordem[i]);
      output_b(output);        
      set_timer1(65536-angulos[i]); // delay de tempo baixo // 65536-Tpulsoalto
   }else{
      disable_interrupts (int_timer1);
      output=0;
   }   
}

#int_timer2
void trata_t2(){
   l=0;
}

void main() {
   set_tris_a(0b11111111);
   set_tris_B(0b00000000);
   setup_port_a( ALL_ANALOG );
   setup_adc( ADC_CLOCK_DIV_16 );
   //usart_inicializa (25,1);  //ESSES PARÂMETROS SELECIONAM BAUD RATE DE 9600 BPS //para usart

   setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_1 ); //para delay de tempo alto
   setup_timer_2 (T2_DIV_BY_16, 250, 5); //para delay de tempo baixo
   enable_interrupts (global);

   while (true) {
      for (k = 0; k < 7; ++k){ // faz aquisição de entrada analogica
         set_adc_channel( k );
         delay_us(20);
         temp = read_adc();
         angulos[k] = temp*2+500;
      }


      output_b(0b00000000);
      l=1;
      enable_interrupts (int_timer2);
      while(l); //delay_us(TEMPO_BAIXO);
      disable_interrupts (int_timer2);

      //Prepara(angulos, angordem);
      i=0;
      set_timer1(0);
      //output=0b11111110;
      //output_b(output);
      bit_set(output, 0);
      bit_set(output, angordem[i]);
      output_b(output);  
      set_timer1(65536-angulos[i]); // delay de tempo baixo // 65536-Tpulsoalto  
      enable_interrupts (int_timer1);
      while(output!=0);
      disable_interrupts (int_timer1);


      /*for (k = 0; k < 7; ++k){
         bit_set(output, k+1);
         enable_interrupts (int_timer1);
         output_b(output);
         set_timer1(65536-angulos[k]); // delay de tempo baixo // 65536-Tpulsoalto
         while(bit_test(output,k+1));
         disable_interrupts (int_timer1);
      }
      */
      

      /*for (j = 0; j < 7; ++j){
         putc(j+1); //usart_transmite(j+1); //para usart
         //rcsta.cren = 1; //para usart
         valorint = getc(); //usart_recebe (); //para usart
         l=j;
         valorint = valorint - 100;
         tempodelay = valorint*15/2 + 1500;//temp*2+500;
         angulos[j] = tempodelay;
         delay_us(20);
      }*/
   }
}
