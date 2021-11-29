################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/generated/gui_generated/src/containers/fileItemBase.cpp \
../Src/generated/gui_generated/src/containers/folderItemBase.cpp \
../Src/generated/gui_generated/src/containers/numPickerBase.cpp \
../Src/generated/gui_generated/src/containers/playerSettingsBase.cpp 

OBJS += \
./Src/generated/gui_generated/src/containers/fileItemBase.o \
./Src/generated/gui_generated/src/containers/folderItemBase.o \
./Src/generated/gui_generated/src/containers/numPickerBase.o \
./Src/generated/gui_generated/src/containers/playerSettingsBase.o 

CPP_DEPS += \
./Src/generated/gui_generated/src/containers/fileItemBase.d \
./Src/generated/gui_generated/src/containers/folderItemBase.d \
./Src/generated/gui_generated/src/containers/numPickerBase.d \
./Src/generated/gui_generated/src/containers/playerSettingsBase.d 


# Each subdirectory must supply rules for building sources it contributes
Src/generated/gui_generated/src/containers/%.o: ../Src/generated/gui_generated/src/containers/%.cpp Src/generated/gui_generated/src/containers/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Inc -I../Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../Src/generated/fonts/include -I../Src/generated/gui_generated/include -I../Src/generated/images/include -I../Src/generated/texts/include -I../Src/gui/include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../Src/generated/videos/include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

