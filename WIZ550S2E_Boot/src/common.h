
#ifndef __COMMON_H__
#define __COMMON_H__

#define MAJOR_VER		101
#define MINOR_VER		0
#define MAINTENANCE_VER	1

#define SOCK_DATA		0
#define SOCK_CONFIG		1
#define SOCK_TFTP		2
#define SOCK_DHCP		3

#define INFO_DBG		0x01
#define ERROR_DBG		0x02
#define DEBUG_DBG		0x04
#define IPC_DBG			0x08

#define DBG_PRINT(level, format, args...)		{ \
											if(dbg_level & level) \
												printf(format, ##args); \
										} 

#define NORMAL_MODE		0
#define TFTP_MODE		1

extern int dbg_level;

#endif
