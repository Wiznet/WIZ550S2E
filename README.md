# WIZ550S2E
Serial to Ethernet Module based on W5500 & Cortex-M0

## Software
These are Firmware projects (source code) based on LPCXpresso IDE.
- Firmware source code
  - Application
  - Boot
if you want to modify App modify as follows:
..\WIZ550S2E\wiznet_s2e_wiz550s2e_board\inc\board.h

    #define _WIZ550S2E_APP_
    #define _WIZ550S2E_BOOT_

if you want to modify Boot modify as follows:
..\WIZ550S2E\wiznet_s2e_wiz550s2e_board\inc\board.h

    //#define _WIZ550S2E_APP_
    #define _WIZ550S2E_BOOT_

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

For more details, please refer to [WIZ550S2Eweb Wiki page](http://wizwiki.net/wiki/doku.php?id=products:wiz550s2e:start) in [WIZnet Wiki](http://wizwiki.net).

## Revision History
v1.0.0
- First release : Apr. 2015
