#include <string.h>

#include "handshake.h"

int handshake(UART_HandleTypeDef *huart, uint8_t* tx_handshake, uint8_t* rx_handshake){
	  uint8_t buffer[10] = {0};
	  while(1){
			  HAL_UART_Transmit(huart, tx_handshake, 10, HAL_MAX_DELAY);
			  HAL_UART_Receive(huart, buffer, 10, 100);
			  if(memcmp(buffer, rx_handshake, (uint8_t) 10)==0){
				  return 0;
			  }
		  }
}

void protocol_details(UART_HandleTypeDef *huart, uint16_t *samples, uint16_t *frames){
	HAL_UART_Receive(huart, (uint8_t *) frames, 2, HAL_MAX_DELAY);
	HAL_UART_Receive(huart, (uint8_t *) samples, 2, HAL_MAX_DELAY);

	// Sending MAX frames and sample values
	// Reusing the dsp_buffer to send the data, will be overwritten
	uint16_t buffer = MAX_FRAMES;
	HAL_UART_Transmit(huart, (uint8_t *) &buffer, 2, HAL_MAX_DELAY);
	buffer = MAX_SAMPLES;
	HAL_UART_Transmit(huart, (uint8_t *) &buffer, 2, HAL_MAX_DELAY);
}
