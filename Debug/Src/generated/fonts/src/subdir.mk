################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/generated/fonts/src/ApplicationFontProvider.cpp \
../Src/generated/fonts/src/CachedFont.cpp \
../Src/generated/fonts/src/FontCache.cpp \
../Src/generated/fonts/src/Font_verdana_16_4bpp_0.cpp \
../Src/generated/fonts/src/Font_verdana_20_4bpp_0.cpp \
../Src/generated/fonts/src/GeneratedFont.cpp \
../Src/generated/fonts/src/Kerning_verdana_16_4bpp.cpp \
../Src/generated/fonts/src/Kerning_verdana_20_4bpp.cpp \
../Src/generated/fonts/src/Table_verdana_16_4bpp.cpp \
../Src/generated/fonts/src/Table_verdana_20_4bpp.cpp \
../Src/generated/fonts/src/UnmappedDataFont.cpp 

OBJS += \
./Src/generated/fonts/src/ApplicationFontProvider.o \
./Src/generated/fonts/src/CachedFont.o \
./Src/generated/fonts/src/FontCache.o \
./Src/generated/fonts/src/Font_verdana_16_4bpp_0.o \
./Src/generated/fonts/src/Font_verdana_20_4bpp_0.o \
./Src/generated/fonts/src/GeneratedFont.o \
./Src/generated/fonts/src/Kerning_verdana_16_4bpp.o \
./Src/generated/fonts/src/Kerning_verdana_20_4bpp.o \
./Src/generated/fonts/src/Table_verdana_16_4bpp.o \
./Src/generated/fonts/src/Table_verdana_20_4bpp.o \
./Src/generated/fonts/src/UnmappedDataFont.o 

CPP_DEPS += \
./Src/generated/fonts/src/ApplicationFontProvider.d \
./Src/generated/fonts/src/CachedFont.d \
./Src/generated/fonts/src/FontCache.d \
./Src/generated/fonts/src/Font_verdana_16_4bpp_0.d \
./Src/generated/fonts/src/Font_verdana_20_4bpp_0.d \
./Src/generated/fonts/src/GeneratedFont.d \
./Src/generated/fonts/src/Kerning_verdana_16_4bpp.d \
./Src/generated/fonts/src/Kerning_verdana_20_4bpp.d \
./Src/generated/fonts/src/Table_verdana_16_4bpp.d \
./Src/generated/fonts/src/Table_verdana_20_4bpp.d \
./Src/generated/fonts/src/UnmappedDataFont.d 


# Each subdirectory must supply rules for building sources it contributes
Src/generated/fonts/src/%.o: ../Src/generated/fonts/src/%.cpp Src/generated/fonts/src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Inc -I../Src -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FatFs/src -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/touchgfx/framework/include -I../Src/generated/fonts/include -I../Src/generated/gui_generated/include -I../Src/generated/images/include -I../Src/generated/texts/include -I../Src/gui/include -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -femit-class-debug-always -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

