/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdio.h>

#include "dsp.h"
#include "handshake.h"
#include "matrix.h"
#include "settings.h"
#include "cycle_count.h"
#include "hann_window.h"

#if MAX_FRAMES == 256
#include "test_data_256.h"
#elif MAX_FRAMES == 512
#include "test_data_512.h"
#elif MAX_FRAMES == 1024
#include "test_data_1024.h"
#elif MAX_FRAMES == 2048
#include "test_data_2048.h"
#else
#error "Wrong dimension entered"
#endif

#if MAX_SAMPLES < DATA_COLS
#error "Wrong number of samples entered"
#endif


// Constants for DSP
const float F_N_S = (float)FPS / (float)MAX_FRAMES;
const uint8_t BIN_MIN_S = LOW_CUT_S / F_N_S;
const uint8_t BIN_MAX_S = HIGH_CUT_S / F_N_S;

const float F_N_F = (float)ADC_FREQ * 1000.0f / (float)MAX_SAMPLES;
const uint8_t BIN_MIN_F = LOW_CUT_F / F_N_F;
const uint8_t BIN_MAX_F = HIGH_CUT_F / F_N_F;



hann_type hanning_window[MAX_FRAMES];


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

LPTIM_HandleTypeDef hlptim1;

UART_HandleTypeDef hlpuart1;

TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM3_Init(void);
static void MX_LPTIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if MODE_ADC_VALIDATION == 1 || MODE_DEMO == 1
	volatile uint16_t acq_progress = 0;
	volatile uint8_t run_sp = 0;
#endif

#ifdef FIXED
	int16_t dsp[MAX_FRAMES*MAX_SAMPLES] = {0};
#endif
#ifdef FLOAT
	float dsp[MAX_FRAMES*MAX_SAMPLES] = {0};
#endif


#if MAX_FRAMES < 1024
#ifdef FIXED
	volatile int16_t fft_check_buffer[MAX_FRAMES*MAX_SAMPLES] = {0};
#endif
#ifdef FLOAT
	volatile float fft_check_buffer[MAX_FRAMES*MAX_SAMPLES] = {0};
#endif
#endif


// Callback for Lptimer reset - We use it just to wake up
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef * hlptim){

}



int arrays_are_equal(uint8_t* array1, uint8_t* array2, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (array1[i] != array2[i]) {
        	printf("Not the same in position %d\r\n", i);
            return 0;
        }
    }
    return 1;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  MX_LPUART1_UART_Init();

#if MODE_ADC_VALIDATION == 1 || MODE_DEMO == 1
  MX_DMA_Init();
  MX_ADC1_Init();
#endif

#if SIGNAL_GEN == 1
  MX_TIM3_Init();
  MX_LPTIM1_Init();
#endif
  /* USER CODE BEGIN 2 */
#if SIGNAL_GEN == 1
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
#endif


#if MODE_DSP_VALIDATION == 1 || MODE_ADC_VALIDATION == 1
  uint8_t tx_handshake[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  uint8_t confirm_handshake[10] = {255, 254, 253, 252, 251, 250, 249, 248, 247, 246};
  handshake(&hlpuart1, tx_handshake, confirm_handshake); // Blocking until handshake succesfull
#endif
#if MODE_DSP_SELFTEST == 1
  printf("Welcome to the DSP test application!\r\n");
#endif
#if MODE_DEMO == 1
printf("Welcome to the DEMO application!\r\n");
#endif

#if MODE_ADC_VALIDATION == 1
		  uint16_t protocol_info[5] = {WINDOW, FPS, ADC_SAMPLES, ADC_FREQ, sizeof(*dsp)};
		  // Small delay for the Python application
		  HAL_Delay(100);
		  HAL_UART_Transmit(&hlpuart1, (uint8_t *) protocol_info, 10, HAL_MAX_DELAY);
      // Turning UART off for power benchmarking
      HAL_UART_DeInit(&hlpuart1);
#endif

#if MODE_DSP_VALIDATION == 1
	  uint16_t samples;
	  uint16_t frames;
      // Receive how many data to validate
	  protocol_details(&hlpuart1, &samples, &frames);
	  // Generating Hanning window
	  gen_hanning_window(hanning_window, frames);
	  // Receiving sample data from PC - zero pad
	  receive_matrix(&hlpuart1, (uint8_t *) dsp, frames, samples, MAX_SAMPLES, sizeof(*dsp));
#endif

#if MODE_DEMO == 1
    uint16_t samples = ADC_SAMPLES;
    uint16_t frames = WINDOW * FPS;
    // Generating Hanning window
    gen_hanning_window(hanning_window, frames);
    printf("Starting DEMO...\r\n");
    // Turning UART off for power benchmarking
    HAL_UART_DeInit(&hlpuart1);
#endif

#if MODE_DSP_SELFTEST == 1
      printf("Loading saved data...\r\n");
      copy_matrix((uint8_t *) dsp, (uint8_t *) test_data, DATA_ROWS, DATA_COLS, MAX_SAMPLES, sizeof(*test_data));
      uint16_t samples = DATA_COLS;
      uint16_t frames = DATA_ROWS;

      // Generating Hanning window
      gen_hanning_window(hanning_window, frames);

      printf("Running DSP...\r\n");

      #ifdef FIXED
      printf("Fixed point implementation\r\n");
      #endif
      #ifdef FLOAT
      printf("Floating point implementation\r\n");
      #endif

      // Turning UART off for power benchmarking
      HAL_UART_DeInit(&hlpuart1);

    volatile float extracted_freq  = 0;
	  // DSP start -> GPIO DSP_FLAG HIGH - For benchmarking
	  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_SET);

	  uint32_t cycles; /* number of cycles */

	  KIN1_InitCycleCounter(); /* enable DWT hardware */
	  KIN1_ResetCycleCounter(); /* reset cycle counter */
	  KIN1_EnableCycleCounter(); /* start counting */

	  extracted_freq = (float) run_dsp(frames, samples, dsp, 0, MAX_FRAMES, hanning_window, &hlpuart1);

	  cycles = KIN1_GetCycleCounter(); /* get cycle counter */
	  KIN1_DisableCycleCounter(); /* disable counting if not used any more */

	  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_RESET);
	  // Converting bin to frequency
	  extracted_freq = extracted_freq*F_N_S;

#if MAX_FRAMES < 1024
	  // Copy the FFT output to check later with the online implementation
	  copy_matrix((uint8_t *) fft_check_buffer, (uint8_t *) dsp, DATA_ROWS, DATA_COLS, MAX_SAMPLES, sizeof(*dsp));
#endif

	// Keeping UART disabled for power benchmarking
	MX_LPUART1_UART_Init();
	printf("Expected frequency: 0.83Hz - Extracted frequency: %f\r\n", extracted_freq);
	printf("DSP Cycle count: %d\r\n", (int) cycles);

	printf("\r\nOnline implementation\r\n\r\n");
	extracted_freq = 0;

	memset(dsp, 0, MAX_FRAMES*MAX_SAMPLES*sizeof(*dsp));

	uint8_t tot_windows_test = MAX_FRAMES/(FPS*WINDOW);
	uint8_t offset = MAX_FRAMES - tot_windows_test*(FPS*WINDOW);


	memcpy(dsp, test_data+(MAX_FRAMES-offset)*MAX_SAMPLES, offset * MAX_SAMPLES * sizeof(*test_data));
	// Running DSP to save the FFT in the array
	run_dsp(frames, samples, dsp, 0, offset, hanning_window, &hlpuart1); //MAX(0, MAX_FRAMES - (i+1)*(FPS*WINDOW))
//	dsp[0] = 0;
	// Move down the fft
	memcpy(dsp + FPS*WINDOW*MAX_SAMPLES, dsp, offset*MAX_SAMPLES*sizeof(*dsp));
	for(int i=0; i<tot_windows_test; i++){

		memcpy(dsp, test_data + (MAX_FRAMES-offset-(i+1)*FPS*WINDOW)*MAX_SAMPLES, FPS*WINDOW*MAX_SAMPLES*sizeof(*dsp));

		HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_SET);

		  KIN1_InitCycleCounter(); /* enable DWT hardware */
		  KIN1_ResetCycleCounter(); /* reset cycle counter */
		  KIN1_EnableCycleCounter(); /* start counting */

		  extracted_freq = (float) run_dsp(frames, samples, dsp, 0, FPS*WINDOW, hanning_window, &hlpuart1); //MAX(0, MAX_FRAMES - (i+1)*(FPS*WINDOW))
//		  dsp[0] = 0;

			// Making space for new readings at top of the buffer
		  void *not_opt;
		  if(i!=tot_windows_test-1){
			 // Taking care of elements out of win dimension
			  not_opt = memcpy(dsp + tot_windows_test*FPS*WINDOW*MAX_SAMPLES, dsp + (tot_windows_test-1)*FPS*WINDOW*MAX_SAMPLES, offset*MAX_SAMPLES*sizeof(*dsp));
			  for(int f=tot_windows_test-1; f>0; f--){
				  not_opt = memcpy(dsp + f*FPS*WINDOW*MAX_SAMPLES, dsp + (f-1)*FPS*WINDOW*MAX_SAMPLES, FPS*WINDOW*MAX_SAMPLES*sizeof(*dsp));
			  }
//				  memmove(dsp + FPS*WINDOW*MAX_SAMPLES, dsp, (MAX_FRAMES - FPS*WINDOW) * MAX_SAMPLES * sizeof(*dsp));

		  }


		  cycles = KIN1_GetCycleCounter(); /* get cycle counter */
		  KIN1_DisableCycleCounter(); /* disable counting if not used any more */

		  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_RESET);
		  // Converting bin to frequency
		  extracted_freq = extracted_freq*F_N_S;

			printf("Expected frequency: 0.83Hz - Extracted frequency: %f\r\n", extracted_freq);
			printf("DSP Cycle count: %d\r\n", (int) cycles);

	}

	printf("Online extraction FINISHED\r\n");


#if MAX_FRAMES < 1024
	int check_fft_equal = arrays_are_equal((uint8_t *) dsp, (uint8_t *) fft_check_buffer, MAX_FRAMES*MAX_SAMPLES*sizeof(*dsp));

	printf("Check FFTs of offline and online - Expected: 1, Output: %d\r\n", check_fft_equal);
#endif





	printf("job done\r\n");


#endif

#if MODE_DSP_VALIDATION == 1
		  // DSP start -> GPIO DSP_FLAG HIGH - For benchmarking
		  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_SET);

		  float extracted_freq = (float) run_dsp(frames, samples, dsp, 0, MAX_FRAMES, hanning_window,  &hlpuart1);

		  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_RESET);

		  // Converting bin to frequency
		  extracted_freq = extracted_freq*F_N_S;

      // Sending the extracted frequency
      HAL_UART_Transmit(&hlpuart1, (uint8_t *) &extracted_freq, sizeof(extracted_freq), HAL_MAX_DELAY);

#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
#if MODE_ADC_VALIDATION == 1 || MODE_DEMO == 1
  // Starting timer for frame acquisition - wakeup from stop mode
  HAL_LPTIM_Counter_Start_IT(&hlptim1, 10);
#endif

  while (1)
  {
#if MODE_ADC_VALIDATION == 1 || MODE_DEMO == 1
  // Preparing to go in Stop mode
  HAL_ADC_DeInit(&hadc1);
#if SIGNAL_GEN == 1
  HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
#endif

  HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

  // Setting clocks when waking up
	SystemClock_Config();

	// Disabling systick to not wakeup while waiting for ADC EoC
//	SysTick->CTRL = 0;
//	HAL_LPTIM_Counter_Stop_IT(&hlptim1);

#if SIGNAL_GEN == 1
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
#endif

	MX_DMA_Init();
	MX_ADC1_Init();

	// Signal ADC start for benchmarking
	HAL_GPIO_WritePin(ADC_FLAG_GPIO_Port, ADC_FLAG_Pin, GPIO_PIN_SET);
	// Get ADC ready for triggering

  uint16_t acq_temp = acq_progress;

  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) (dsp + acq_progress*MAX_SAMPLES), ADC_SAMPLES);

  // Trigger the pulser
  // Could also use Lptim signal out. Using GPIO we can control better timing.
  HAL_GPIO_WritePin(US_TRIGGER_GPIO_Port, US_TRIGGER_Pin, GPIO_PIN_SET);

  // We have to use this hack instead of going in sleep mode.
  // For some reason if ltpim enabled MCU wakes up immediately from sleep mode, even if lptim interrupt disabled
  // Turning it on and off every time too time/energy consuing. This hack works and is not energy hungry.
  while(acq_temp == acq_progress){
  }


  // Wait in low power mode for ADC acquisition to finish
//  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
//	HAL_LPTIM_Counter_Start_IT(&hlptim1, 1);

  // Trigger the
    HAL_GPIO_WritePin(US_TRIGGER_GPIO_Port, US_TRIGGER_Pin, GPIO_PIN_RESET);

  // When window is full start SP
	if(run_sp==1){
	  run_sp = 0;
    HAL_UART_Init(&hlpuart1);
    #if MODE_ADC_VALIDATION == 1
	  send_matrix(&hlpuart1, (uint8_t *) dsp, MAX_FRAMES, MAX_SAMPLES, sizeof(*dsp));
    #endif
    #if MODE_DEMO == 1
	  	  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_SET);
    	  float extracted_freq = (float) run_dsp(frames, samples, dsp, 0, MAX_FRAMES, hanning_window,  &hlpuart1);
    	  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_RESET);
	      // Converting bin to frequency
	      extracted_freq = extracted_freq*F_N_S;
        printf("Extracted frequency: %f\r\n", extracted_freq);
    #endif
	}
	#endif

//
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_EXT_IT11;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief LPTIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPTIM1_Init(void)
{

  /* USER CODE BEGIN LPTIM1_Init 0 */

  /* USER CODE END LPTIM1_Init 0 */

  /* USER CODE BEGIN LPTIM1_Init 1 */

  /* USER CODE END LPTIM1_Init 1 */
  hlptim1.Instance = LPTIM1;
  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV128;
  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPTIM1_Init 2 */

  /* USER CODE END LPTIM1_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 921600;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 4;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 5;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DSP_FLAG_GPIO_Port, DSP_FLAG_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, ADC_FLAG_Pin|US_TRIGGER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pin : DSP_FLAG_Pin */
  GPIO_InitStruct.Pin = DSP_FLAG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DSP_FLAG_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ADC_FLAG_Pin US_TRIGGER_Pin */
  GPIO_InitStruct.Pin = ADC_FLAG_Pin|US_TRIGGER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
