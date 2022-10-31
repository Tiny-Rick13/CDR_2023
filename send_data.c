// #include "C:\Pico\pico-sdk\src\rp2_common\pico_stdlib\stdlib.h"
#include "pico/stdio.h"
#include <serial>
// #include "C:\Pico\pico-sdk\src\rp2_common\hardware_irq\include\hardware\irq.h"
// #include "pico/multicore.h"
// #include "hardware/structspio.h"
#include "C:\Pico\pico-sdk\src\rp2040\hardware_regs\include\hardware\regs\uart.h"
// #include "uart_rx.pio.h"

#define UART_ID uart0
int BAUD_RATE 115200;
#define DATA_BITS 8 // taille constante de la trame
#define START_BITS 1 // bit alloué pour informer du début de la trame
#define PARITY    UART_PARITY_NONE // jsp à quoi ça sert mais bon
int UART_TX_PIN 6;
int UART_RX_PIN 7;

stdio_uart_init_full(UART_ID *uart, int BAUD_RATE, UART_TX_PIN, UART_RX_PIN);

char buffer_reception[8]; //byte de taille 8 : jusqu'à 255
// uart_init(UART_ID, BAUD_RATE); // je comprends pas l'erreur

int main() {

    //Initialise I/O
    stdio_init_all(); 

    //Main Loop 
    while(1) {
        

        if(uart_is_readable(UART_ID)) // vérifie que l'UART a fini sa phase d'envoi
  { 
    Serial.readBytes(buffer_reception, 8); // lit 8 bits de données envoyés par l'USB et les stock dans le buffer

    if (buffer_reception[0] == 0) // '0' est le caractère spécial provisoire annoncant le début d'un ordre.
      {
        printf("Début de l'ordre");
      
      for( int i = 1; i < 8; i ++ ) { // on lit chaque bit sauf le bit de début

        switch (buffer_reception[i]) { // chaque bit correspond à un ordre en fonction du caractère.
          case 1: 
             uart_puts(uart0, "ORDRE1"); // écrit à l'UART pour la transmission, la fonction bloque jusqu'à que tout soit envoyé.
             break;
          case 2:
             uart_puts(uart0, "ORDRE2");
             break;
          default:
             uart_puts(uart0, "Code d'erreur"); // si le bit ne correspond à aucun ordre -> erreur
             break;   

        }
      
          
      }
      

    Serial.flush(); // attend que la transmission se finisse
  }
}
    }
