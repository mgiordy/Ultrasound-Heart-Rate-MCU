################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.c \
../Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.c 

OBJS += \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.o \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.o 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.d \
./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Source/BasicMathFunctions/%.o Drivers/CMSIS/DSP/Source/BasicMathFunctions/%.su Drivers/CMSIS/DSP/Source/BasicMathFunctions/%.cyclo: ../Drivers/CMSIS/DSP/Source/BasicMathFunctions/%.c Drivers/CMSIS/DSP/Source/BasicMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DARM_MATH_LOOPUNROLL -DDEBUG -DUSE_HAL_DRIVER -DSTM32L496xx -c -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/embd_us" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/embd_us/Inc" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/Drivers/CMSIS/DSP/Include" -I"/Users/mgiordano/Documents/ultrasound/HR_Extraction/MCU-ADC-DSP/Drivers/CMSIS/DSP/PrivateInclude" -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-BasicMathFunctions

clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-BasicMathFunctions:
	-$(RM) ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_f32.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_add_q15.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_offset_f32.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_scale_f32.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_shift_q15.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_f32.su ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.cyclo ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.d ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.o ./Drivers/CMSIS/DSP/Source/BasicMathFunctions/arm_sub_q15.su

.PHONY: clean-Drivers-2f-CMSIS-2f-DSP-2f-Source-2f-BasicMathFunctions

