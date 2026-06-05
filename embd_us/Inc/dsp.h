#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "stdint.h"
#include "stm32l4xx_hal.h"
#include "arm_math.h"
#include "hann_window.h"

#ifdef FIXED
uint32_t run_dsp(uint16_t frames, uint16_t samples, int16_t *dsp, int16_t begin_new_data, int16_t end_new_data, hann_type *hanning_window, UART_HandleTypeDef *huart);
#endif

#ifdef FLOAT
uint32_t run_dsp(uint16_t frames, uint16_t samples, float *dsp, int16_t begin_new_data, int16_t end_new_data, hann_type *hanning_window, UART_HandleTypeDef *huart);
#endif


#endif
