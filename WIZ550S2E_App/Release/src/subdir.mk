################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc11exx.c \
../src/crp.c \
../src/main.c \
../src/sysinit.c 

S_SRCS += \
../src/aeabi_romdiv_patch.s 

OBJS += \
./src/aeabi_romdiv_patch.o \
./src/cr_startup_lpc11exx.o \
./src/crp.o \
./src/main.o \
./src/sysinit.o 

C_DEPS += \
./src/cr_startup_lpc11exx.d \
./src/crp.d \
./src/main.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -DNDEBUG -D__CODE_RED -D__USE_ROMDIVIDE -mcpu=cortex-m0 -mthumb -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\netutil" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\callback" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\ATcmd" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Configuration" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Ethernet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT\MQTTPacket\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\PlatformHandler" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\S2E" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\lpc_chip_11exx\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


