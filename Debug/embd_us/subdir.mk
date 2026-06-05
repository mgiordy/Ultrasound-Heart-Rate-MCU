################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../embd_us/adc_callback.c \
../embd_us/dsp.c \
../embd_us/dsp_float.c \
../embd_us/handshake.c \
../embd_us/hann_window.c \
../embd_us/main.c \
../embd_us/matrix.c \
../embd_us/printf.c 

OBJS += \
./embd_us/adc_callback.o \
./embd_us/dsp.o \
./embd_us/dsp_float.o \
./embd_us/handshake.o \
./embd_us/hann_window.o \
./embd_us/main.o \
./embd_us/matrix.o \
./embd_us/printf.o 

C_DEPS += \
./embd_us/adc_callback.d \
./embd_us/dsp.d \
./embd_us/dsp_float.d \
./embd_us/handshake.d \
./embd_us/hann_window.d \
./embd_us/main.d \
./embd_us/matrix.d \
./embd_us/printf.d 


# Each subdirectory must supply rules for building sources it contributes
embd_us/%.o embd_us/%.su embd_us/%.cyclo: ../embd_us/%.c embd_us/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_LOOPUNROLL -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/embd_us" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/embd_us/Inc" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/Drivers/CMSIS/DSP/Include" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/Drivers/CMSIS/DSP/PrivateInclude" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-embd_us

clean-embd_us:
	-$(RM) ./embd_us/adc_callback.cyclo ./embd_us/adc_callback.d ./embd_us/adc_callback.o ./embd_us/adc_callback.su ./embd_us/dsp.cyclo ./embd_us/dsp.d ./embd_us/dsp.o ./embd_us/dsp.su ./embd_us/dsp_float.cyclo ./embd_us/dsp_float.d ./embd_us/dsp_float.o ./embd_us/dsp_float.su ./embd_us/handshake.cyclo ./embd_us/handshake.d ./embd_us/handshake.o ./embd_us/handshake.su ./embd_us/hann_window.cyclo ./embd_us/hann_window.d ./embd_us/hann_window.o ./embd_us/hann_window.su ./embd_us/main.cyclo ./embd_us/main.d ./embd_us/main.o ./embd_us/main.su ./embd_us/matrix.cyclo ./embd_us/matrix.d ./embd_us/matrix.o ./embd_us/matrix.su ./embd_us/printf.cyclo ./embd_us/printf.d ./embd_us/printf.o ./embd_us/printf.su

.PHONY: clean-embd_us

