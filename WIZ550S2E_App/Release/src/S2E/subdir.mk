################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/S2E/S2E.c 

OBJS += \
./src/S2E/S2E.o 

C_DEPS += \
./src/S2E/S2E.d 


# Each subdirectory must supply rules for building sources it contributes
src/S2E/%.o: ../src/S2E/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\netutil" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\callback" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\ATcmd" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\Configuration" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\Ethernet" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\Internet" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\PlatformHandler" -I"D:\_GitHub\WIZ550S2E\WIZ550S2E_App\src\S2E" -I"D:\_GitHub\WIZ550S2E\wiznet_s2e_wiz550s2e_board\inc" -I"D:\_GitHub\WIZ550S2E\lpc_chip_11exx\inc" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


