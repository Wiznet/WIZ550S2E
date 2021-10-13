################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTFormat.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTPacket.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c \
../src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./src/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTFormat.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTPacket.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o \
./src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./src/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTFormat.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTPacket.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d \
./src/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
src/Internet/MQTT/MQTTPacket/src/%.o: ../src/Internet/MQTT/MQTTPacket/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DNDEBUG -D__CODE_RED -DCORE_M0 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11EXX__ -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\netutil" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\callback" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\ATcmd" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Configuration" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Ethernet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT\MQTTPacket\src" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\PlatformHandler" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\S2E" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\wiznet_s2e_wiz550s2e_board\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\lpc_chip_11exx\inc" -I"D:\Austin\Workspace\LPCXpresso_7_5_0_254\WIZ550S2E_App\src\Internet\MQTT" -O3 -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


