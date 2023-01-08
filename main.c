#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
//#include <uart.h>


#define BAUD_RATE 115200 //vitesse de com 
#define UART_ID uart0 
#define UART_TX_PIN  6
#define UART_RX_PIN  7

// début def fonctions
float getARG_float(unsigned short buffer_reception[]);
unsigned int getARG(unsigned int double_octet);
unsigned int getCOMP(unsigned int octet0);
unsigned int getID(unsigned int octet0);
int extraire_bit(unsigned long data, int n);
unsigned int concatene16(unsigned int octet1, unsigned int octet2);
unsigned int concatene32(unsigned int double_octet1, unsigned int double_octet2);
void allumer_pompes(char double_octet);
void allumer_moteurs(char double_octet);
void terminaison(char ordre[], uart_inst_t *UART_ID);
void acknowledgement(char ordre[], uart_inst_t *UART_ID);
void erreur(char ordre[]);
// fin


void main() 
{
    stdio_init_all();
    uart_init( UART_ID, BAUD_RATE );
    while (1) 
    {

        unsigned char buffer_reception[5]; //stocke la trame reçue, 5 octets
        

        if (uart_is_readable(uart0) != 0) 
        {
            //lit les informations recues par le UART et stocke dans buffer
            uart_read_blocking(uart0, buffer_reception, 5);

            unsigned char octet0 = buffer_reception[0]; //octet ID et complément
            unsigned char octet1 = buffer_reception[1];
            unsigned char octet2 = buffer_reception[2]; 
            unsigned char octet3 = buffer_reception[3];
            unsigned char octet4 = buffer_reception[4];

            unsigned int double_octet1 = concatene16(octet1, octet2);
            unsigned int double_octet2 = concatene16(octet3, octet4);

            unsigned int arg = getARG(concatene32(double_octet1, double_octet2)); //argument


                unsigned int ID = getID( octet0 ); //ID
                unsigned int COMP = getCOMP( octet0 ); //Complement

                unsigned int ID_MOT, temps, ticks, pos, valeur;

                switch(ID) 
                    {
                        case 0 : //Lidar
                            if ( COMP == 0 )  
                            { //Arrêt
                                
                            }
                            if ( COMP == 1 )
                            { //Reprise
                            
                            }
                            break;

                        case 1 : //Avancer/Reculer
                            ticks = arg;
                            if (COMP = 1) 
                            { //Avancer

                            }
                            if (COMP = 0) 
                            { //Reculer

                            }
                            break;

                        case 2 : //Tourner
                            ticks = arg;
                            if (COMP = 1) 
                            { //Sens trigo

                            }
                            if (COMP= 0) 
                            { //Sens horaire

                            }
                            break;

                        case 3 : //Annuler déplacements
                            
                            break;   

                        case 4 : //Actionner moteur seul (valeur)
                            ID_MOT = COMP;
                            pos = arg;
                            break;

                        case 5 : //Actionner moteur seul (durée)
                            ID_MOT = COMP;
                            temps = arg; //ms
                            break;

                        case 6 : //Actionner pompes
                            allumer_pompes(double_octet2);
                            break; 

                        case 9 : //Injecter variable
                            valeur = arg;
                            break; 

                        case 10 : //Requete variable
                            
                            break; 

                        case 11 : //Tracking PID
                            
                            break; 

                        default : 
                            if (ID == 7)
                            {//ne rien faire (traitement apres l'acknowledgement)

                            }
                            if (ID== 8)
                            {//ne rien faire

                            }
                            else 
                            {
                            //correspond à aucun ordre, renvoie erreur
                            erreur(buffer_reception);
                            }
                            break;
            
            
                    }
                acknowledgement( buffer_reception, uart0 ); //envoie trame d'acknowledgement

                //pas d'acknowledgement pour ordre 7 et 8
                if (ID == 7)
                            {//Actionner plusieurs moteurs
                            int nb = COMP; //nbre de moteurs à actionner
                            allumer_moteurs(double_octet2);                                         //les nb prochaines trames seront allouées aux arguments 
                            }
       
        }
    }
}

