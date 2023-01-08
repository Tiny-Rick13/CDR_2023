#include <stdio.h>
#include <stdlib.h>
#include "hardware/uart.h"
#include <stdint.h>


void erreur(char ordre[]) {
 char id = getID(ordre[0]);
 uart_puts(uart0, "0100");
 uart_putc(uart0, id);
 const char ret = 00000000000000000000000000000000;
 uart_puts(uart0, ret);

}

void acknowledgement(char ordre[], uart_inst_t *UART_ID) 
//renvoie la trame d'ordre à la pi 
{
    for (int i = 0; i < 4; i ++)
    {
        uart_putc(uart0, 0);
    }
    
    for (int i = 0; i < 4; i ++)
    {
        uart_putc(uart0, extraire_bit( ordre[0] , i));
    }

    for (int i = 0; i < 32; i ++)
    {
        uart_putc(uart0, 0);
    }
    
}

void terminaison(char ordre[], uart_inst_t *UART_ID)
//renvoie terminaison a la pi quand action finie  
{
    for (int i = 0; i < 3; i ++)
    {
        uart_putc(uart0, 0);
    }
    uart_putc(uart0, 1);
    for (int i = 0; i < 8; i ++)
    {
        uart_putc(uart0, extraire_bit(ordre[0] , i));
    }
    for (int i = 0; i < 32; i ++)
    {
        uart_putc(uart0, 0);
    }
}

void allumer_moteurs(char double_octet)
{
    
    char buffer[32];
    for ( int i = 0; i < 16; i++ ) 
    {
        unsigned int bit = extraire_bit(double_octet, i);
        if (bit == 1)
        {
        uart_read_blocking(uart0, buffer, 5);
        unsigned int double_octet1 = concatene16(buffer[1],buffer[2]);
        unsigned int double_octet2 = concatene16(buffer[3],buffer[4]);
        int ID = getCOMP(buffer[0]);
        int pos = getARG(concatene32(double_octet1,double_octet2));
        //Allumer le moteur ID avec argument "pos"
        }
    
    }
}

void allumer_pompes(char double_octet)
{
    for ( int i = 0; i < 16; i++ ) 
    {
        unsigned int bit = extraire_bit(double_octet, i);
        if (bit == 1)
        {//Allumer pompe

        }
    }

}

unsigned int concatene16(unsigned int octet1, unsigned int octet2)
{
    unsigned int result = (uint16_t)(octet1 << 8)|octet2;
    return result;
}

unsigned int concatene32(unsigned int double_octet1, unsigned int double_octet2)
{
    unsigned int result = (uint32_t)(double_octet1 << 16)|double_octet2;
    return result;
}

int extraire_bit(unsigned long data, int n) {
 
    unsigned int MASK = 0x1;
    unsigned long bit = (data >> n) & MASK;
 
    return bit;
}

unsigned int getID(unsigned int octet0) {
    unsigned int MASK = 0x0f;
    return (octet0 >> 4) & MASK;
    
}

unsigned int getCOMP(unsigned int octet0) {
    return octet0 & 0x0f;
}

unsigned int getARG(unsigned int double_octet)
{
    return double_octet >> 0;
}

float getARG_float(unsigned short buffer_reception[])
{
    unsigned short id = getID(buffer_reception[0]);
    unsigned short comp = getCOMP(buffer_reception[0]);

    unsigned short arg0 = (((unsigned short) buffer_reception[1]) << 8) + ((unsigned short) buffer_reception[2]);
    unsigned short arg1 = (((unsigned short) buffer_reception[3]) << 8) + ((unsigned short) buffer_reception[4]);
    unsigned int tmp = (((unsigned int) arg0) << 16) + (unsigned int) arg1;
    float floatArg = *(float*) &tmp;

    return floatArg;

}



