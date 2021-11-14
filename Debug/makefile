################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/gui/src/testfatfs_screen_screen/subdir.mk
-include Src/gui/src/soundrecord_screen/subdir.mk
-include Src/gui/src/playaudiofile_screen/subdir.mk
-include Src/gui/src/oscilloscope_screen/subdir.mk
-include Src/gui/src/model/subdir.mk
-include Src/gui/src/main_screen_screen/subdir.mk
-include Src/gui/src/externalstimulator_screen/subdir.mk
-include Src/gui/src/containers/subdir.mk
-include Src/gui/src/common/subdir.mk
-include Src/gui/src/codectestscreen_screen/subdir.mk
-include Src/generated/texts/src/subdir.mk
-include Src/generated/images/src/__designer/subdir.mk
-include Src/generated/images/src/subdir.mk
-include Src/generated/gui_generated/src/testfatfs_screen_screen/subdir.mk
-include Src/generated/gui_generated/src/soundrecord_screen/subdir.mk
-include Src/generated/gui_generated/src/playaudiofile_screen/subdir.mk
-include Src/generated/gui_generated/src/oscilloscope_screen/subdir.mk
-include Src/generated/gui_generated/src/main_screen_screen/subdir.mk
-include Src/generated/gui_generated/src/externalstimulator_screen/subdir.mk
-include Src/generated/gui_generated/src/containers/subdir.mk
-include Src/generated/gui_generated/src/common/subdir.mk
-include Src/generated/gui_generated/src/codectestscreen_screen/subdir.mk
-include Src/generated/fonts/src/subdir.mk
-include Src/build/MINGW32_NT-6.2/gui/src/testfatfs_screen_screen/subdir.mk
-include Src/build/MINGW32_NT-6.2/gui/src/main_screen_screen/subdir.mk
-include Src/build/MINGW32_NT-6.2/gui/src/containers/subdir.mk
-include Src/build/MINGW32_NT-6.2/gui/src/codectestscreen_screen/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include Middlewares/Third_Party/FatFs/src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Src/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM32F4_LCD
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32F4_LCD.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32F4_LCD.list \

OBJCOPY_BIN += \
STM32F4_LCD.bin \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: STM32F4_LCD.elf secondary-outputs

# Tool invocations
STM32F4_LCD.elf: $(OBJS) $(USER_OBJS) C:\Users\Ilya\Documents\Projects\Firmware\ Projects\STM32CudeIDE\STM32F4_LCD\STM32F429IGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-g++ -o "STM32F4_LCD.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Ilya\Documents\Projects\Firmware Projects\STM32CudeIDE\STM32F4_LCD\STM32F429IGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="STM32F4_LCD.map" -Wl,--gc-sections -static -L"C:\Users\Ilya\Documents\Projects\Firmware Projects\STM32CudeIDE\STM32F4_LCD\Middlewares\ST\touchgfx\lib\core\cortex_m4f\gcc" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32F4_LCD.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32F4_LCD.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32F4_LCD.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32F4_LCD.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJDUMP_LIST)$(C_UPPER_DEPS)$(S_DEPS)$(C_DEPS)$(OBJCOPY_BIN)$(CC_DEPS)$(SIZE_OUTPUT)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(CXX_DEPS)$(S_UPPER_DEPS)$(CPP_DEPS) STM32F4_LCD.elf
	-@echo ' '

post-build:
	arm-none-eabi-objcopy -O ihex STM32F4_LCD.elf STM32F4_LCD.hex
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
