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
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\netutil" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Configuration" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Ethernet" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\Internet" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\WIZ550S2E_Boot\src\PlatformHandler" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\WIZnet\Workspace_LPCXpresso_7.5.0_254\lpc_chip_11exx\inc" -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


