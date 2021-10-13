################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ATcmd/atcmd.c \
../src/ATcmd/cmdrun.c 

OBJS += \
./src/ATcmd/atcmd.o \
./src/ATcmd/cmdrun.o 

C_DEPS += \
./src/ATcmd/atcmd.d \
./src/ATcmd/cmdrun.d 


# Each subdirectory must supply rules for building sources it contributes
src/ATcmd/%.o: ../src/ATcmd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\netutil" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\callback" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\ATcmd" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Configuration" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Ethernet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT\MQTTPacket\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\PlatformHandler" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\S2E" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\lpc_chip_11exx\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


