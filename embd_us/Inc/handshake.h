#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "settings.h"

int handshake(UART_HandleTypeDef *huart, uint8_t* tx_handshake, uint8_t* rx_handshake);

void protocol_details(UART_HandleTypeDef *huart, uint16_t *samples, uint16_t *frames);

#endif
