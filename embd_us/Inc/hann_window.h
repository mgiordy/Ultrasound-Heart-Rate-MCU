/*
 * hann_window.h
 *
 *  Created on: Dec 23, 2024
 *      Author: mgiordano
 */

#ifndef HANN_WINDOW_H_
#define HANN_WINDOW_H_

#include "settings.h"
#include <stdint.h>

#ifdef FLOAT
typedef float hann_type;
#endif
#ifdef FIXED
typedef int16_t hann_type;
#endif

int16_t gen_hanning_window(hann_type *hanning_window, uint16_t frames);

#endif /* HANN_WINDOW_H_ */
