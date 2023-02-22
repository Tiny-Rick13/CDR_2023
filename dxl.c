#include "TypeDef.h"
#include "crc_calculation.h"
#include "crc_calculation.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

#define true 1
#define false 0
#define BROADCAST_ID 0xFE //254
#define DXL_HEADER_SIZE 4
#define DXL_CRC_SIZE 2

//registres 
#define ADDR_TORQUE_ENABLE 0x40
#define ADDR_GOAL_POSITION 0x74
#define ADDR_PRESENT_POSITION 0x8C

//activation/desactivation du couple 
#define TORQUE_ENABLE 1
#define TORQUE_DISABLE 0

uint8_t Pos0 = 0;
uint8_t Pos1 = 0;
uint8_t Pos2 = 0;
uint8_t Pos3 = 0;

unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
static UART_HandleTypeDef *handler_dxl = NULL;

void DXL_Transmit(uint8_t *packet, uint16_t size_packet) {
    HAL_UART_Transmit(handler_dxl /*uart module*/, packet /*pointer to data*/, size_packet, 1000 /*TimeOut*/);
}

void DXL_Init(uint8_t id) {
    //Active le couple
    uint8_t Torque_Enable_Packet[] = {0xFF, 0xFF, 0xFD, 0x00, id, 0x04, ADDR_TORQUE_ENABLE, TORQUE_ENABLE, 0x00};
    DXL_Transmit(Torque_Enable_Packet, sizeof(Torque_Enable_Packet));
    //Positionne le DXL à sa pos initiale
    uint8_t pos_init[] = {0xFF, 0xFF, 0xFD, 0x00, id, 0x07, ADDR_GOAL_POSITION, 0x00, 0x00, 0x00};
    DXL_Transmit(pos_init, sizeof(pos_init));
}

void DXL_Angle( uint8_t id, uint32_t angle) {
    Pos0 = (angle & 0x000000FF);
    Pos1 = (angle & 0x0000FF00) >> 8;
    Pos2 = (angle & 0x00FF0000) >> 16;
    Pos3 = (angle & 0xFF000000) >> 24;

    uint8_t Packet[] = { 0xFF, 0xFF, 0xFD, 0x00, id, 0x09, 0x00, 0x03, 0x74, 0x00, Pos0, Pos1, Pos2, Pos3, 0x00, 0x00 };
    update_crc_packet(Packet, 16);
    DXL_Transmit(Packet, 13);
}

int is_DXL_trame_valid(uint8_t trame[]) {
    int size = sizeof(trame);
    //verifie taille de la trame 
    if ( size < (DXL_HEADER_SIZE + DXL_CRC_SIZE)) {
    return false;
    }
    //verifie head de trame 
    if (trame[0] != 0xFF || trame[1] != 0xFF || trame[2] != 0xFD || trame[3] != 0x00) {
    return false;
    }
    //verifie id
    uint8_t id = trame[4];
    if (id > 0xFE) {
    return 0;
    }
    //verfie crc
    
}

DXL_Led_ON(uint8_t id) {
    uint8_t Led_ON[] = { 0xFF, 0xFF, 0xFD, 0x00, id, 0x06, 0x00, 0x03, 0x41, 0x00, 0x01, 0x00, 0x00 };
    update_crc_packet(Led_ON, 13);
    DXL_Transmit(Led_ON, 13);
}

DXL_Led_OFF(uint8_t id) {
    uint8_t Led_OFF[] = { 0xFF, 0xFF, 0xFD, 0x00, id, 0x06, 0x00, 0x03, 0x41, 0x00, 0x00, 0x00, 0x00 };
    update_crc_packet(Led_OFF, 13);
    DXL_Transmit(Led_OFF, 13);
}



