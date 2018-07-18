# WIZ550S2E

Serial to Ethernet Module based on [W5500](http://wizwiki.net/wiki/doku.php?id=products:w5500:start) & Cortex-M0

## Firmware

These are Firmware projects (source code) based on [LPCXpresso](http://www.nxp.com/products/microcontrollers-and-processors/arm-processors/lpc-cortex-m-mcus/lpc1100-cortex-m0-plus-m0/lpcxpresso-ide-v8.2.2:LPCXPRESSO) IDE or [MCUXpresso](http://www.nxp.com/products/software-and-tools/run-time-software/mcuxpresso-software-and-tools/mcuxpresso-integrated-development-environment-ide:MCUXpresso-IDE).

- [Direct Download Link for LPCXpresso7.5.0_254 for Windows](https://s3.amazonaws.com/LPCXpresso7/LPCXpresso_7.5.0_254.exe)
	
Firmware source code are two types as follows.
- Application
- Bootloader

**The order of compile**

1. lpc_chip_11exx.
2. wiznet_s2e_wiz550s2e_board.
3. WIZ550S2E_App or WIZ550S2E_Boot.
4. [More details](https://forum.wiznet.io/t/topic/2538)

![s2e](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428621268&w=499&h=203&tok=26df60&media=products:wiz550s2e:s2e.png "s2e")

**Firmware download**

In this case there is only just [.hex file.](https://github.com/Wiznet/WIZ550S2E/releases)

If you would like to use [.bin file](http://wizwiki.net/wiki/doku.php?id=products:wiz550s2e:wiz550s2e_download), look at the following that : Configuration Tool/FW Uploading in [WIZ550S2E Wiki page](http://wizwiki.net/wiki/doku.php?id=products:wiz550s2e:wiz550s2e_download)

1. Connect WIZ550S2E and RS232I/F board and DC+5V adpater and RS232 serial cable and lan cable.
<p align="left">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?t=1434087198&w=500&h=374&tok=390d68&media=products:wiz550s2e:wiz550s2e_fw_up_1.png" />
</p>

2. HW/TRIG switch off.
<p align="left">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?t=1434087199&w=500&h=378&tok=d39e60&media=products:wiz550s2e:wiz550s2e_fw_up_2.png" />
</p>

3. Check LED.
<p align="left">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?t=1434087198&w=335&h=242&tok=a856af&media=products:wiz550s2e:wiz550s2e_fw_up_3.png" />
</p>

4. Short J5 and push reset switch. 
<p align="left">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?t=1434087199&w=500&h=373&tok=b6167f&media=products:wiz550s2e:wiz550s2e_fw_up_4.png" />
</p>

5. firmware download using [Flash Magic](http://www.flashmagictool.com/).
<p align="left">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?t=1434087199&w=500&h=494&tok=4e6a04&media=products:wiz550s2e:wiz550s2e_fw_up_5.png" />
</p>

## Images

### WIZ550S2E Module

<!-- WIZ550S2E pic -->
![WIZ550S2E](http://wizwiki.net/wiki/lib/exe/fetch.php?cache=&media=products:wiz550s2e:wiz550s2eds:wiz550s2e_pin.jpg "WIZ550S2E")

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
- 2.54mm Header x2

### WIZ550S2E-232 Interface Board

<!-- WIZ550S2E-232 Interface Board pic -->
<p align="center">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?cache=&media=products:wiz550s2e:wiz550s2eds:rs232if_pin_20140729.jpg" />
</p>
<p align="center">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?cache=&media=products:wiz550s2e:wiz550s2eds:%ED%81%AC%EA%B8%B0%EB%B3%80%ED%99%98_wiz550s2e-232-evb.png" />
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
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?cache=&media=products:wiz550s2e:wiz550s2eds:rs485-422if_pin_20140729.jpg" />
</p>
<p align="center">
  <img width="70%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?cache=&media=products:wiz550s2e:wiz550s2eds:%ED%81%AC%EA%B8%B0%EB%B3%80%ED%99%98_wiz550s2e-485-evb.png" />
</p>

#### Features

- Line Driver SP3485EN
- System Reset Switch
- Factory Reset Switch
- Hardware-Trigger Mode Switch
- Status LEDs
- DC-Jack (for +5V Input Power)

For more details, please refer to [WIZ550S2E Wiki page](http://wizwiki.net/wiki/doku.php?id=products:wiz550s2e:start) in [WIZnet Wiki](http://wizwiki.net).

## Configuration Tool

WIZnet Configuration Tool made Java base. so runs on any OS that JRE be installed. 

Install JAVA Runtime Environment 

1. [java page](http://www.java.com).
2. Go to the Download tab to download the installation files for the PC environment.
![1](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428563629&w=500&h=321&tok=b6bae3&media=products:wiz550s2e:1.png "1")
3. Install the downloaded installation file.
4. When you run the Terminal, type the "java -version" command, the following message is displayed.
![2](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428563628&w=500&h=100&tok=1cadd9&media=products:wiz550s2e:2.png "2")
5. [WIZ550S2E wiki page](http://wizwiki.net/wiki/doku.php?id=products:wiz550s2e:wiz550s2e_download).
6. Configuration Tool for WIZ550S2E download.                                     
![3](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428563629&w=500&h=375&tok=02960c&media=products:wiz550s2e:3.png "3")
7. Then Unzipped the downloaded zip file. Excutable jar file is created.
8. Excute file.
 * Jar file double click at GUI.
 * Type the “java -jar WIZ550S2E_Configuration_Tool.jar” command at Terminal.
![4](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428563629&w=500&h=412&tok=8ab846&media=products:wiz550s2e:4.png "4")

## Revision History

<p align="left">
  <img width="100%" src="https://wizwiki.net/wiki/lib/exe/fetch.php?media=products:wiz550s2e:new_wireframe_2.png" />
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
		- v1.10 Configuration Tool is required.

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
