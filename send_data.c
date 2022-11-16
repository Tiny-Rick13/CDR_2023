#include <Arduino.h>
#include "C:\COM2\lib\.platformio\packages\framework-arduino-mbed\cores\arduino\Serial.h"
#include "C:\COM2\lib\.platformio\packages\framework-arduino-mbed\cores\arduino\api\cstdio"
#include <uart.h>
#include <stdio_uart.h>
#include <stdio.h>
#include <stdlib.h>

uint BAUD_RATE = 112500; //vitesse de communication de la pico en usb 
uart_inst_t UART_ID = uart0;

int DATA_BITS = 8; //taille de la trame 
int START_BIT = 0; //bit de départ (provisoire)

int A = 27; //contrôle vitesse moteur 1
int B = 26; //contrôle direction moteur 1

int codeuseA = 25;
int codeuseB = 24; 
int UART_TX_PIN = 6;
int UART_RX_PIN = 7;

uart_init(UART_ID, BAUD_RATE);

void setup() {
    // Initialise entrées/sortie
    pinMode(codeuseA, INPUT_PULLUP);
    pinMode(codeuseB, INPUT_PULLUP);

    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
}

// Définition des fonctions 

void stop() {
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
}

void avancer(char a) {
    analogWrite(A, a); //contrôle vitesse en PMW (tension en créneau)
    digitalWrite(B, HIGH);
}

void reculer(char a) {
    analogWrite(A, a); //contrôle vitesse en PMW (tension en créneau)
    digitalWrite(B, LOW);
}

int main() {
    
    while (1) {
        char buffer_reception[DATA_BITS]; //stocke la trame reçue, byte de taille 8 : 255 possibilités
        
        if (uart_is_readable(UART_ID) != 0) {
            
            uart_read_blocking(UART_ID, buffer_reception, DATA_BITS);
            
            if (buffer_reception[0] == START_BIT) { // Le programme ne fonctionne que si le 1er bit est le bit de départ
               
                printf("Début de l'ordre");
                
                for( int i; i < DATA_BITS; i ++ ) { // On lit chaque bit reçu
                   
                    switch(buffer_reception[i]) {
                        case 1 : 
                            uart_puts( UART_ID, "ORDRE 1");
                            break;
                        case 2 :
                            uart_puts( UART_ID, "ORDRE 2");
                            break; //etc jusqu'à 8 
                        default : //si le bit ne correspond à aucun ordre, renvoyer une erreur
                            uart_puts( UART_ID, "CODE D'ERREUR");
                            break;
                    }
                }
            
            }
        }
       
    }
}
