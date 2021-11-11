################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/gui/src/externalstimulator_screen/externalStimulatorPresenter.cpp \
../Src/gui/src/externalstimulator_screen/externalStimulatorView.cpp 

OBJS += \
./Src/gui/src/externalstimulator_screen/externalStimulatorPresenter.o \
./Src/gui/src/externalstimulator_screen/externalStimulatorView.o 

CPP_DEPS += \
./Src/gui/src/externalstimulator_screen/externalStimulatorPresenter.d \
./Src/gui/src/externalstimulator_screen/externalStimulatorView.d 


# Each subdirectory must supply rules for building sources it contributes
Src/gui/src/externalstimulator_screen/%.o: ../Src/gui/src/externalstimulator_screen/%.cpp Src/gui/src/externalstimulator_screen/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Inc -I../Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../Src/generated/fonts/include -I../Src/generated/gui_generated/include -I../Src/generated/images/include -I../Src/generated/texts/include -I../Src/gui/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

