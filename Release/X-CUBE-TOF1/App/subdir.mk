################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../X-CUBE-TOF1/App/app_x-cube-tof1.c 

OBJS += \
./X-CUBE-TOF1/App/app_x-cube-tof1.o 

C_DEPS += \
./X-CUBE-TOF1/App/app_x-cube-tof1.d 


# Each subdirectory must supply rules for building sources it contributes
X-CUBE-TOF1/App/%.o X-CUBE-TOF1/App/%.su X-CUBE-TOF1/App/%.cyclo: ../X-CUBE-TOF1/App/%.c X-CUBE-TOF1/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../X-CUBE-TOF1/App -I../X-CUBE-TOF1/Target -I../Core/Inc -I../Drivers/BSP/STM32F4xx_Nucleo -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-X-2d-CUBE-2d-TOF1-2f-App

clean-X-2d-CUBE-2d-TOF1-2f-App:
	-$(RM) ./X-CUBE-TOF1/App/app_x-cube-tof1.cyclo ./X-CUBE-TOF1/App/app_x-cube-tof1.d ./X-CUBE-TOF1/App/app_x-cube-tof1.o ./X-CUBE-TOF1/App/app_x-cube-tof1.su

.PHONY: clean-X-2d-CUBE-2d-TOF1-2f-App

