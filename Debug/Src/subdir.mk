################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/main.c \
../Src/misc.c \
../Src/onewire.c \
../Src/segm.c 

OBJS += \
./Src/main.o \
./Src/misc.o \
./Src/onewire.o \
./Src/segm.o 

C_DEPS += \
./Src/main.d \
./Src/misc.d \
./Src/onewire.d \
./Src/segm.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F0 -DSTM32F051R8Tx -c -I../Inc -I"C:/pets/dse-test-task/CMSIS/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/main.d ./Src/main.o ./Src/main.su ./Src/misc.d ./Src/misc.o ./Src/misc.su ./Src/onewire.d ./Src/onewire.o ./Src/onewire.su ./Src/segm.d ./Src/segm.o ./Src/segm.su

.PHONY: clean-Src

