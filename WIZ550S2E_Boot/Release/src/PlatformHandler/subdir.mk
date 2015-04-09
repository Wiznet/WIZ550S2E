################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/PlatformHandler/eepromHandler.c \
../src/PlatformHandler/flashHandler.c \
../src/PlatformHandler/sspHandler.c \
../src/PlatformHandler/timerHandler.c \
../src/PlatformHandler/uartHandler.c 

OBJS += \
./src/PlatformHandler/eepromHandler.o \
./src/PlatformHandler/flashHandler.o \
./src/PlatformHandler/sspHandler.o \
./src/PlatformHandler/timerHandler.o \
./src/PlatformHandler/uartHandler.o 

C_DEPS += \
./src/PlatformHandler/eepromHandler.d \
./src/PlatformHandler/flashHandler.d \
./src/PlatformHandler/sspHandler.d \
./src/PlatformHandler/timerHandler.d \
./src/PlatformHandler/uartHandler.d 


# Each subdirectory must supply rules for building sources it contributes
src/PlatformHandler/%.o: ../src/PlatformHandler/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\netutil" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Configuration" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Ethernet" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Internet" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\PlatformHandler" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\lpc_chip_11exx\inc" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


