//INCLUDE DIRECTORIES
#include <avr/io.h>
#include <util/delay.h>
#include "project.h"
#include <string.h>

//DEFINE BAUD RATE
#define BAUD 9600 

//BUTTON PIN 4
const int PUSH_BUTTON = 4; 
//LED PIN 8
const int LED = 8; 
//LED STATE = 1 MEANS OFF, LED STATE = 2 MEANS ON
int LED_state = 0b00000001; 

int main(void){
DDRD |= 0b10000000;
DDRB |= 0b00000001;

char compare_onCmd[] = "led_on";
char compare_offCmd[] = "led_off";
char receive_string[8];
int counter = 0;
char received;

//SET BAUD RATE
uart_init(BAUD); 

int prevButton_state = digitalRead(PUSH_BUTTON); 

//RUN CODE
while(1){ 
  if((UCSR0A)&(1<<RXC0)){

   //LED INPUT
   received = uart_receive(); 

   //RECIEVE INPUT
   if(received != '\n'){
     receive_string[counter++] = received;
    }

   //READ INPUT
   else{
     if( counter == 6 && strncmp(receive_string,compare_onCmd,6)==0 ){ 
       uart_transmit_array("LED Is On\n",10);
        PORTB |= 0b00000001;
      }

     else if( counter == 7 && strncmp(receive_string,compare_offCmd,7)==0){ 
       uart_transmit_array("LED Is Off\n",11);
       PORTB &= 0b11111110;
      }

     else{
       uart_transmit_array("ERROR\n",6);
      }

     counter = 0;
    }
  }
 //BUTTON CODE
  int Button_state = digitalRead(PUSH_BUTTON); 

  if(Button_state == HIGH && Button_state != prevButton_state){ 
   uart_transmit_array("Button Pressed\n",15);
  }

  else if (Button_state == LOW && Button_state != prevButton_state){
   uart_transmit_array("Button not Pressed\n",19);
  }
  prevButton_state  = Button_state;
}

return 0;
}