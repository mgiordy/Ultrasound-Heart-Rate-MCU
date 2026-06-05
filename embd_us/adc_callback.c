#include "settings.h"

#if MODE_ADC_VALIDATION == 1 || MODE_DEMO == 1
	#include "main.h"

	extern volatile uint16_t acq_progress;
	extern volatile uint8_t run_sp;


	void HAL_ADC_ConvCpltCallback (ADC_HandleTypeDef * hadc){
		HAL_GPIO_WritePin(ADC_FLAG_GPIO_Port, ADC_FLAG_Pin, GPIO_PIN_RESET);

		acq_progress++; // New frame to store
		// If window is full start from the beginning
		if(acq_progress==WINDOW*FPS){
			acq_progress = 0;
			run_sp = 1;
		}

	}

#endif
