################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/netutil/subdir.mk
-include src/PlatformHandler/subdir.mk
-include src/Internet/TFTP/subdir.mk
-include src/Ethernet/W5500/subdir.mk
-include src/Ethernet/subdir.mk
-include src/Configuration/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: WIZ550S2E_Boot.axf

# Tool invocations
WIZ550S2E_Boot.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\_GitHub\WIZ550S2E\wiznet_s2e_wiz550s2e_board\Release" -L"D:\_GitHub\WIZ550S2E\lpc_chip_11exx\Release" -Xlinker -Map="WIZ550S2E_Boot.map" -Xlinker --gc-sections -Xlinker --allow-multiple-definition -mcpu=cortex-m0 -mthumb -T "Bootloader.ld" -L ../linkscripts -o "WIZ550S2E_Boot.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) WIZ550S2E_Boot.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "WIZ550S2E_Boot.axf"; arm-none-eabi-objcopy -O ihex "WIZ550S2E_Boot.axf" "WIZ550S2E_Boot.hex"; # arm-none-eabi-objcopy -v -O binary "WIZ550S2E_Boot.axf" "WIZ550S2E_Boot.bin" ; # checksum -p LPC11E36_501 -d "WIZ550S2E_Boot.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
