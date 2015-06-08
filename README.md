# WIZ550S2E
Serial to Ethernet Module based on W5500 & Cortex-M0

## Firmware
These are Firmware projects (source code) based on LPCXpresso IDE.
Firmware source code.
- Application
- Boot

**the order of compile**

1. lpc_chip_11exx.
2. wiznet_s2e_wiz550s2e_board.
3. WIZ550S2E_App.

![s2e](http://wizwiki.net/wiki/lib/exe/fetch.php?t=1428621268&w=499&h=203&tok=26df60&media=products:wiz550s2e:s2e.png "s2e")

**Firmware fusing**
updating...

1. 
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
v1.0.0
- First release : Apr. 2015
