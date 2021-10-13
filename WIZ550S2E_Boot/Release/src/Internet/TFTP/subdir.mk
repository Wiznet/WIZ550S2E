################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Internet/TFTP/tftp.c 

OBJS += \
./src/Internet/TFTP/tftp.o 

C_DEPS += \
./src/Internet/TFTP/tftp.d 


# Each subdirectory must supply rules for building sources it contributes
src/Internet/TFTP/%.o: ../src/Internet/TFTP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src\netutil" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src\Configuration" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src\Ethernet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src\Internet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_Boot\src\PlatformHandler" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\lpc_chip_11exx\inc" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


