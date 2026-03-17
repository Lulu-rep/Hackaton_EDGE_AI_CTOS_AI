################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/vl53l5cx/porting/platform.c 

OBJS += \
./Drivers/BSP/Components/vl53l5cx/porting/platform.o 

C_DEPS += \
./Drivers/BSP/Components/vl53l5cx/porting/platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/vl53l5cx/porting/%.o Drivers/BSP/Components/vl53l5cx/porting/%.su Drivers/BSP/Components/vl53l5cx/porting/%.cyclo: ../Drivers/BSP/Components/vl53l5cx/porting/%.c Drivers/BSP/Components/vl53l5cx/porting/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../X-CUBE-TOF1/App -I../X-CUBE-TOF1/Target -I../Core/Inc -I../Drivers/BSP/STM32F4xx_Nucleo -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx-2f-porting

clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx-2f-porting:
	-$(RM) ./Drivers/BSP/Components/vl53l5cx/porting/platform.cyclo ./Drivers/BSP/Components/vl53l5cx/porting/platform.d ./Drivers/BSP/Components/vl53l5cx/porting/platform.o ./Drivers/BSP/Components/vl53l5cx/porting/platform.su

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-vl53l5cx-2f-porting

