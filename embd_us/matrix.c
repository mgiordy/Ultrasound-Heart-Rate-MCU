#include <string.h>

#include "matrix.h"

void send_matrix(UART_HandleTypeDef *huart, uint8_t *buffer, uint32_t x_size, uint32_t y_size, uint8_t n_bytes){
	for(int i=0; i<x_size; i++){
		HAL_UART_Transmit(huart, buffer + i*y_size*n_bytes, y_size*n_bytes, HAL_MAX_DELAY);
	}
}

void receive_matrix(UART_HandleTypeDef *huart, uint8_t *buffer, uint32_t x_data_size, uint32_t y_data_size, uint32_t y_size, uint8_t n_bytes){
	for(uint32_t i=0; i<x_data_size; i++){
	    HAL_UART_Receive(huart, buffer + i*y_size*n_bytes, y_data_size*n_bytes, HAL_MAX_DELAY);
  }
}

void copy_matrix(uint8_t *dst_buffer, uint8_t *src_buffer, uint32_t x_src_size, uint32_t y_src_size, uint32_t y_dst_size, uint8_t n_bytes){
	for(uint32_t i=0; i<x_src_size; i++){
		memcpy(dst_buffer + i*y_dst_size*n_bytes, src_buffer + i*y_src_size*n_bytes, y_src_size*n_bytes);
  }
}

void read_column(uint8_t *matrix, uint8_t *buffer, uint32_t y_column, uint32_t x_size, uint32_t y_size, uint8_t n_bytes, uint8_t num_elements){
	for(int i=0; i<x_size; i++){
		memcpy(buffer + i*n_bytes*num_elements, matrix + i*y_size*n_bytes + y_column*n_bytes*num_elements, n_bytes*num_elements);
	}
}

void write_column(uint8_t *matrix, uint8_t *buffer, uint32_t y_column, uint32_t x_size, uint32_t y_size, uint8_t n_bytes){
	for(int i=0; i<x_size; i++){
		memcpy(matrix + (i*y_size + y_column)*n_bytes, buffer + i*n_bytes, n_bytes);
	}
}
