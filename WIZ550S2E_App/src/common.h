
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include "board.h"

#define RENEWAL_UDP_PORT    // add by kei

#define MAJOR_VER		1
#define MINOR_VER		1
#define MAINTENANCE_VER	5

#define SOCK_DATA		0
#define SOCK_CONFIG		1
#define SOCK_TFTP		2
#define SOCK_DHCP		3
#define SOCK_DNS		4

#define OP_COMMAND		0
#define OP_DATA			1

#define APP_BASE		0x6000
#define WORK_BUF_SIZE	1024

extern uint8_t op_mode;

#endif
