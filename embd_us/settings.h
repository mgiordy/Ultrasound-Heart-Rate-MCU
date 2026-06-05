#ifndef SETTINGS_H
#define SETTINGS_H

// Choose only one of the following
#define MODE_ADC_VALIDATION 0 // Use this for data acquisition
#define MODE_DSP_VALIDATION 0 // Use this for DSP validation with python script
#define MODE_DSP_SELFTEST 1 // Can use this for DSP benchmarking
#define MODE_DEMO 0 // Use this for demo mode and memory usage benchmarking

// Activate on-board square wave generation to test ADC without the ultrasound setup
#define SIGNAL_GEN 1


// Choose only one
#define FIXED
//#define FLOAT

#ifdef FIXED
#ifdef FLOAT
#error "Choose one between the fixed and floating point implementation"
#endif
#endif


#if MODE_ADC_VALIDATION || MODE_DSP_VALIDATION
#define BIN_PRINT
#endif
#if MODE_DSP_SELFTEST || MODE_DEMO
#define DEBUG_PRINT
#endif

// ADC
#define ADC_SAMPLES 50
#define FPS 25
#define WINDOW 2 // Seconds
#define ADC_FREQ 4000 // kHz

// DSP
#define MAX_FRAMES 512
#define MAX_SAMPLES 64

// Frequencies considered for the analysis: 0.5Hz - 3Hz
#define LOW_CUT_S 0.5f
#define HIGH_CUT_S 3.0f

// Filtering between 0.1MHz and 2MHz
#define LOW_CUT_F 100000.0f
#define HIGH_CUT_F 2000000.0f


// Checking that samples fit in DSP buffer
#if ADC_SAMPLES > MAX_SAMPLES
    #error "ERROR: ADC_SAMPLES must be less than MAX_SAMPLES."
#endif
#if WINDOW * FPS > MAX_FRAMES
    #error "ERROR: ADC_SAMPLES * FPS must be less than MAX_SAMPLES."
#endif


#endif
