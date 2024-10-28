################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../DeviceLibrary/Src/BH1750.c \
../DeviceLibrary/Src/BMP180.c \
../DeviceLibrary/Src/CLCD_I2C.c \
../DeviceLibrary/Src/DHT20.c \
../DeviceLibrary/Src/ledRGB.c 

OBJS += \
./DeviceLibrary/Src/BH1750.o \
./DeviceLibrary/Src/BMP180.o \
./DeviceLibrary/Src/CLCD_I2C.o \
./DeviceLibrary/Src/DHT20.o \
./DeviceLibrary/Src/ledRGB.o 

C_DEPS += \
./DeviceLibrary/Src/BH1750.d \
./DeviceLibrary/Src/BMP180.d \
./DeviceLibrary/Src/CLCD_I2C.d \
./DeviceLibrary/Src/DHT20.d \
./DeviceLibrary/Src/ledRGB.d 


# Each subdirectory must supply rules for building sources it contributes
DeviceLibrary/Src/%.o DeviceLibrary/Src/%.su DeviceLibrary/Src/%.cyclo: ../DeviceLibrary/Src/%.c DeviceLibrary/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/DA_TKLL/STMF103C8/DeviceLibrary/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-DeviceLibrary-2f-Src

clean-DeviceLibrary-2f-Src:
	-$(RM) ./DeviceLibrary/Src/BH1750.cyclo ./DeviceLibrary/Src/BH1750.d ./DeviceLibrary/Src/BH1750.o ./DeviceLibrary/Src/BH1750.su ./DeviceLibrary/Src/BMP180.cyclo ./DeviceLibrary/Src/BMP180.d ./DeviceLibrary/Src/BMP180.o ./DeviceLibrary/Src/BMP180.su ./DeviceLibrary/Src/CLCD_I2C.cyclo ./DeviceLibrary/Src/CLCD_I2C.d ./DeviceLibrary/Src/CLCD_I2C.o ./DeviceLibrary/Src/CLCD_I2C.su ./DeviceLibrary/Src/DHT20.cyclo ./DeviceLibrary/Src/DHT20.d ./DeviceLibrary/Src/DHT20.o ./DeviceLibrary/Src/DHT20.su ./DeviceLibrary/Src/ledRGB.cyclo ./DeviceLibrary/Src/ledRGB.d ./DeviceLibrary/Src/ledRGB.o ./DeviceLibrary/Src/ledRGB.su

.PHONY: clean-DeviceLibrary-2f-Src

