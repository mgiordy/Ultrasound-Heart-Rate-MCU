#ifndef MATRIX_H
#define MATRIX_H

#include "stdint.h"
#include "stm32l4xx_hal.h"

void send_matrix(UART_HandleTypeDef *huart, uint8_t *buffer, uint32_t x_size, uint32_t y_size, uint8_t n_bytes);
void receive_matrix(UART_HandleTypeDef *huart, uint8_t *buffer, uint32_t x_data_size, uint32_t y_data_size, uint32_t y_size, uint8_t n_bytes);
void copy_matrix(uint8_t *dst_buffer, uint8_t *src_buffer, uint32_t x_src_size, uint32_t y_src_size, uint32_t y_dst_size, uint8_t n_bytes);
void read_column(uint8_t *matrix, uint8_t *buffer, uint32_t y_column, uint32_t x_size, uint32_t y_size, uint8_t n_bytes, uint8_t num_elements);
void write_column(uint8_t *matrix, uint8_t *buffer, uint32_t y_column, uint32_t x_size, uint32_t y_size, uint8_t n_bytes);

#endif
