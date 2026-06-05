#include "settings.h"

#ifdef FIXED

#include "dsp.h"
#include "matrix.h"
#include "settings.h"
#include "hann_window.h"

extern float F_N_S;
extern uint8_t BIN_MIN_S;
extern uint8_t BIN_MAX_S;
 
extern float F_N_F;
extern uint8_t BIN_MIN_F;
extern uint8_t BIN_MAX_F;


uint32_t run_dsp(uint16_t frames, uint16_t samples, int16_t *dsp, int16_t begin_new_data, int16_t end_new_data, hann_type *hanning_window, UART_HandleTypeDef *huart) {
	int16_t dsp_buffer_0[MAX_FRAMES*2] = {0}; // Real FFT needs 2*N
	int16_t dsp_buffer_1[200] = {0}; // Only need space for frequencies of interest - Set to 200 for 2048 samples
  // Differtentiation and amplification
  for (int i = begin_new_data; i < end_new_data; i++) {
    arm_sub_q15(dsp + i * MAX_SAMPLES + 1, dsp + i * MAX_SAMPLES, dsp_buffer_0, (samples - 1));
	// Shifting data to use all 16 bits
    arm_shift_q15(dsp_buffer_0, 7, dsp + i * MAX_SAMPLES, samples - 1);
    // Zeroing all the elements in dsp array not involved in subtraction - data already consumed
    memset(dsp + i * MAX_SAMPLES + samples - 1, 0, (MAX_SAMPLES - samples + 1) * sizeof( * dsp));
  }

  #ifdef BIN_PRINT
  send_matrix(huart, (uint8_t * ) dsp, MAX_FRAMES, MAX_SAMPLES, sizeof( * dsp));
  #endif

  arm_rfft_instance_q15 rfft;
  if (arm_rfft_init_q15( & rfft, MAX_SAMPLES, 0, 1) != ARM_MATH_SUCCESS) {
    return -1;
  }

  for (int i = begin_new_data; i < end_new_data; i++) {
    // Input gets modified
    arm_rfft_q15( & rfft, dsp + i * MAX_SAMPLES, dsp_buffer_0);
    // Boosting signal
    arm_shift_q15(dsp_buffer_0, 6, dsp_buffer_0, MAX_SAMPLES);
    // Getting only half of output because symmetrical
    memcpy(dsp + i * MAX_SAMPLES, dsp_buffer_0, MAX_SAMPLES * sizeof( * dsp));
  }
  #ifdef BIN_PRINT
  send_matrix(huart, (uint8_t * ) dsp, MAX_FRAMES, MAX_SAMPLES, sizeof( * dsp));
  #endif

#ifdef BIN_PRINT
  union {
          float f;
          uint32_t i;
      } u;
  u.f = F_N_S;
  HAL_UART_Transmit(huart, (uint8_t *) &u.f, 4, HAL_MAX_DELAY);
  u.i = BIN_MIN_S;
  HAL_UART_Transmit(huart, (uint8_t *) &u.i, 2, HAL_MAX_DELAY);
  u.i = BIN_MAX_S;
  HAL_UART_Transmit(huart, (uint8_t *) &u.i, 2, HAL_MAX_DELAY);
  u.i = BIN_MIN_F;
  HAL_UART_Transmit(huart, (uint8_t *) &u.i, 2, HAL_MAX_DELAY);
  u.i = BIN_MAX_F;
  HAL_UART_Transmit(huart, (uint8_t *) &u.i, 2, HAL_MAX_DELAY);
#endif

  arm_cfft_instance_q15 cfft;
  if (arm_cfft_init_q15( & cfft, MAX_FRAMES) != ARM_MATH_SUCCESS) {
    return -1;
  }

  for (int i = BIN_MIN_F; i < BIN_MAX_F; i++) {
    // Output of buffer 2*N
    memset(dsp_buffer_0, 0, 2*MAX_FRAMES*sizeof(*dsp_buffer_0));
    read_column((uint8_t *) dsp,(uint8_t *) dsp_buffer_0, i, MAX_FRAMES, MAX_SAMPLES, sizeof( * dsp), 2);
    
    // Applying window to data
    arm_cmplx_mult_real_q15(dsp_buffer_0, hanning_window, dsp_buffer_0, frames);
    
    arm_cfft_q15( & cfft, dsp_buffer_0, 0, 1);
    #ifdef BIN_PRINT
      send_matrix(huart, (uint8_t * ) dsp_buffer_0, 1, MAX_FRAMES * 2, sizeof( * dsp_buffer_0));
    #endif

    arm_cmplx_mag_q15(dsp_buffer_0 + BIN_MIN_S*2, dsp_buffer_0, BIN_MAX_S - BIN_MIN_S); // *2 bc complex
    arm_add_q15(dsp_buffer_1, dsp_buffer_0, dsp_buffer_1, BIN_MAX_S - BIN_MIN_S);

    arm_cmplx_mag_q15(dsp_buffer_0 + BIN_MIN_S*2 + frames, dsp_buffer_0, BIN_MAX_S - BIN_MIN_S); // frames /2 (negative freq) * 2 (complex)
    arm_add_q15(dsp_buffer_1, dsp_buffer_0, dsp_buffer_1, BIN_MAX_S - BIN_MIN_S);

  }

  #ifdef BIN_PRINT
	  // Sending the added frequencies
      send_matrix(huart, (uint8_t *) dsp_buffer_1, 1, BIN_MAX_S - BIN_MIN_S, sizeof(*dsp_buffer_1));
  #endif

  uint32_t max_bin;
  // Filtering only interesting frequencies
  arm_max_q15(dsp_buffer_1, BIN_MAX_S-BIN_MIN_S, dsp_buffer_0, &max_bin); // Throwing max value

  return max_bin + BIN_MIN_S;
}

#endif
