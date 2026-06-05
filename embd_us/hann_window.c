#include "hann_window.h"

#include "arm_math.h"

int16_t gen_hanning_window(hann_type *hanning_window, uint16_t frames){
    if (frames > MAX_FRAMES) {
        return -1; // Error: frames exceed maximum allowed
    }

    float32_t temp_array[MAX_FRAMES] = {0};

    // Generate the array (0, 1, ..., FRAMES-1)
    for (int i = 0; i < frames; i++) {
    	temp_array[i] = (float) i;
    }

    // Compute the cosine values: cos(2 * PI * i / (FRAMES - 1))
    float32_t multiplier = 2.0f * M_PI / (frames - 1);
    arm_scale_f32(temp_array, multiplier, temp_array, frames);
    for (int i = 0; i < frames; i++) {
    	temp_array[i] = arm_cos_f32(temp_array[i]);
    }

    // Scale and shift: 0.5 - 0.5 * cos(...)
    arm_scale_f32(temp_array, -0.5f, temp_array, frames);
    arm_offset_f32(temp_array, 0.5f, temp_array, frames);

    if (sizeof(hanning_window[0]) ==  sizeof(float32_t)){
    	memcpy(hanning_window, temp_array, frames * sizeof(float32_t));
    }
    else if(sizeof(hanning_window[0]) == sizeof(int16_t)){
		  arm_float_to_q15(temp_array, (q15_t *)hanning_window, frames);
    } else {
    	return -2;
    }

    return 0;

}
