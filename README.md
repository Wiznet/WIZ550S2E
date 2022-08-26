- [WIZ550S2E](#wiz550s2e)
	- [Firmware](#firmware)
		- [The order of compile](#the-order-of-compile)
		- [Firmware download](#firmware-download)
	- [Module and Interface boards](#module-and-interface-boards)
		- [WIZ550S2E Module](#wiz550s2e-module)
		- [WIZ550S2E-232 Interface Board](#wiz550s2e-232-interface-board)
		- [WIZ550S2E-485/422 Interface Board](#wiz550s2e-485422-interface-board)
	- [Configuration Tool](#configuration-tool)
	- [Revision History](#revision-history)
		- [v1.3.0](#v130)
		- [v1.2.2](#v122)
		- [v1.2.1](#v121)
		- [v1.2.0](#v120)
		- [v1.1.5](#v115)
		- [v1.1.4](#v114)
		- [v1.1.3](#v113)
		- [v1.1.2](#v112)
		- [v1.1.1](#v111)
		- [v1.1.0](#v110)
		- [v1.0.4](#v104)
		- [v1.0.3](#v103)
		- [v1.0.2](#v102)
		- [v1.0.1](#v101)
		- [v1.0.0](#v100)

----

# WIZ550S2E

Serial to Ethernet Module based on [W5500](https://docs.wiznet.io/Product/iEthernet/W5500/overview) & Cortex-M0.

## Firmware

These are Firmware projects (source code) based on [LPCXpresso](http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/lpc1100-cortex-m0-plus-m0/lpcxpresso-ide-v8.2.2:LPCXPRESSO) IDE or [MCUXpresso](http://www.nxp.com/products/software-and-tools/run-time-software/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).

- [Direct Download Link for LPCXpresso7.5.0_254 for Windows](https://s3.amazonaws.com/LPCXpresso7/LPCXpresso_7.5.0_254.exe)
	
Firmware source code are two types as follows.
- Application
- Bootloader

### The order of compile

1. lpc_chip_11exx.
2. wiznet_s2e_wiz550s2e_board.
3. WIZ550S2E_App or WIZ550S2E_Boot.
4. [More details](https://forum.wiznet.io/t/topic/2538)

![s2e](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/s2e.png?raw=true "s2e")

### Firmware download

In this case there is only just [.hex file.](https://github.com/Wiznet/WIZ550S2E/releases)

If you would like to use [.bin file](https://docs.wiznet.io/Product/S2E-Module/WIZ550S2E/download#binary), look at the following that: 

  * Configuration Tool F/W Uploading in [WIZ550S2E document page](https://docs.wiznet.io/Product/S2E-Module/WIZ550S2E/download#binary)

**1. Connect WIZ550S2E and RS232I/F board and DC+5V adpater and RS232 serial cable and lan cable.**
<p align="left">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2e_fw_up_1.png?raw=true" />
</p>

**2. HW/TRIG switch off.**
<p align="left">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2e_fw_up_2.png?raw=true" />
</p>

**3. Check LED.**
<p align="left">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2e_fw_up_3.png?raw=true" />
</p>

**4. Short J5 and push reset switch.**
<p align="left">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2e_fw_up_4.png?raw=true" />
</p>

**5. firmware download using [Flash Magic](http://www.flashmagictool.com/).**
<p align="left">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2e_fw_up_5.png?raw=true" />
</p>

**6. Must push the "Factory Reset" button on EVB for over 5 seconds.**

----

## Module and Interface boards

### WIZ550S2E Module

<!-- WIZ550S2E pic -->
![WIZ550S2E](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2eds/wiz550s2e_pin.jpg?raw=true "WIZ550S2E")

#### Features & Benefits & Hardware Specification

- Serial to Ethernet Module based on TCP/IP Controller W5500 & Cortex-M0(MCU LPC11E36FHN33) 
- RJ-45 mounted, Pin-header type module
- Serial signals : TXD, RXD, RTS, CTS, GND
- Support the configuration method of AT command & Configuration tool program
- Configuration tool program operates on Windows, Linux & MAC OS
- Support data communication using AT command : able to send data without re-configuring and re-boot of the module when
- changing the destination IP
- Support the interface board for RS-232 and RS422/485  
- 10/100Mbps Ethernet & Max.230kbps serial speed
- Support WIZ VSP (Virtual Serial Port) program
- Dimension (mm) : 55(L) x 30 (W) x 23.49 (H)
- 2.54mm Header 

### WIZ550S2E-232 Interface Board

<!-- WIZ550S2E-232 Interface Board pic -->
<p align="center">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2eds/rs232if_pin_20140729.jpg?raw=true" />
</p>
<p align="center">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2eds/%ED%81%AC%EA%B8%B0%EB%B3%80%ED%99%98_wiz550s2e-232-evb.png?raw=true" />
</p>

#### Features

- Line Driver SP3232EBEY
- System Reset Switch
- Factory Reset Switch
- Hardware-Trigger Mode Switch
- Status LEDs
- DC-Jack (for +5V Input Power)

### WIZ550S2E-485/422 Interface Board

<!-- WIZ550S2E-232 Interface Board pic -->
<p align="center">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2eds/rs485-422if_pin_20140729.jpg?raw=true" />
</p>
<p align="center">
  <img width="70%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/wiz550s2eds/%ED%81%AC%EA%B8%B0%EB%B3%80%ED%99%98_wiz550s2e-485-evb.png?raw=true" />
</p>

#### Features

- Line Driver SP3485EN
- System Reset Switch
- Factory Reset Switch
- Hardware-Trigger Mode Switch
- Status LEDs
- DC-Jack (for +5V Input Power)

For more details, please refer to [WIZ550S2E document page](https://docs.wiznet.io/Product/S2E-Module/WIZ550S2E) in [WIZnet Documents](https://docs.wiznet.io).

----

## Configuration Tool

WIZnet Configuration Tool made Java base. so runs on any OS that JRE be installed. 

**Install JAVA Runtime Environment**

1. [Java page](http://www.java.com).
2. Go to the Download tab to download the installation files for the PC environment.
![1](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/1.png?raw=true "1")
1. Install the downloaded installation file.
2. When you run the Terminal, type the "java -version" command, the following message is displayed.
![2](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/2.png?raw=true "2")
5. [WIZ550S2E document page](https://docs.wiznet.io/Product/S2E-Module/WIZ550S2E/download).
6. Download the Configuration Tool for WIZ550S2E.                                     
![3](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/3.png?raw=true "3")
7. Then Unzipped the downloaded zip file. Excutable jar file is created.
8. Excute file.
 * Double click the **WIZnet_Configuration_Tool.jar** file.
  
	or, 
 * Type the `java -jar WIZnet_Configuration_Tool.jar` command at Terminal.
![4](https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/4.png?raw=true "4")

----

## Revision History

<p align="left">
  <img width="100%" src="https://github.com/Wiznet/document_framework/blob/master/static/img/products/wiz550s2e/new_wireframe_2.png?raw=true" />
</p>

This is the MQTT version repository, see the following link for the Modbus version repository.

https://github.com/Wiznet/WIZ550S2E-Modbus

### v1.3.0

- Added features
	- AT+MQTTSET : (AT command) This command is used to initialize the necessary parameters of MQTT connection(set information automatically saved)
	- AT+MQTTCON : (AT command) Implement MQTT to connect the Broker
	- AT+MQTTPUB : (AT command) Publish the message to subscribed Client(set topic automatically saved)
	- AT+MQTTSUB : (AT command) Such topics will be pushed to the WIZ550S2E when received by the Broker(set topic automatically saved)

### v1.2.2

- Fixed bugs
	- Fixed a bug that vector relocation RAM problem due to buffer initialization
	- Fixed a bug that MQTT connection message transmission when operating as a TCP server
	- Fixed a bug that MQTT subscription setting error when there is no topic

### v1.2.1

- Fixed bugs
	- Fixed a bug that stops when the module switches to data mode and at-command mode
	- The MQTT socket has been modified to open as a nonblock.
	- Modified the memory used for MQTT publishing from static to dynamic allocation
	- Fixed to update connection LED and network state when switching to at-command mode

### v1.2.0

- Added features
	- MQTT
		- [v1.10 Configuration Tool](https://github.com/Wiznet/WIZnet_Configuration_Tool) is required.

### v1.1.5

- Added features
	- AT+MPASS : (AT command) Change the module's name(automatically saved)
	- AT+MNAME : (AT command) Change the module's password(automatically saved)
- Changed
	- AT+NMODE -> AT+MMODE

### v1.1.4

- Fixed Problems receiving data in AT Mode

### v1.1.3

- Fixed UDP Send Fail in AT Mode

### v1.1.2

- Fixed recevied data loss(During trans to serial) problem in AT Mode
	- Limit max size(1000byte) of receive data from ethernet.

### v1.1.1

- Added inactivity time function in Server/Mixed Mode

### v1.1.0

- ioLibrary Update
- Dependency Removal (remove "board.h" in "w5500.h")
- Remove garbage file

### v1.0.4

- Fixed bug
	- Time packing problem in UDP mode.
		- Before : Not work normally after the first time.
		- After : Work normally
	- Upper and lower case problems in config Data.
		- Before : Only uppercase letters work normally.
		- After : Works in both uppercase and lowercase.

### v1.0.3

- Fixed bug
	- AT Command Trigger
		- Before : AT command is actived regardless of AT command enable.
		- After : AT command is actived regard of AT command enable.
- Added features
	- AT+NMODE : (AT command) Change the settings that operates in data mode(automatically saved)

### v1.0.2

- Fixed bug 
	- In mixed mode
		- Before : when connecting to the server, the '+' had to be include in the serial data.
		- After : when serial data is generated directly connecting.
	- In AT command
		- Before : at = a t. in other words, ignore space.
		- After : at != a t. in other words, admitte space.
- Added features
	- Check unlink in mixed mode
	- AT+FDNS = domain name

### v1.0.1

- Fixed Project set

### v1.0.0

- First release : Apr. 2015
