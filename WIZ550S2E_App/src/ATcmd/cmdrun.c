/**
 * @file		cmdrun.c
 * @brief		AT Command Module - Implementation Part Source File
 * @version	1.0
 * @date		2013/02/22
 * @par Revision
 *			2013/02/22 - 1.0 Release
 * @author	Mike Jeong
 * \n\n @par Copyright (C) 2013 WIZnet. All rights reserved.
 */

//#define FILE_LOG_SILENCE
#include "common.h"
#include "cmdrun.h"
#include "ConfigData.h"
#include "eepromHandler.h"
#include "DNS/dns.h"
#include "uartHandler.h"
#include "timerHandler.h"

#define SOCK_STAT_TCP_MASK	0x2
#define SOCK_STAT_PROTMASK	0x3
#define SOCK_STAT_IDLE		0x0
#define SOCK_STAT_UDP		0x1
#define SOCK_STAT_TCP_SRV	0x2
#define SOCK_STAT_TCP_CLT	0x3
#define SOCK_STAT_CONNECTED	0x4

#define ARG_CLEAR(arg_p) arg_p[0] = 0;
#define MAKE_TCMD_DIGIT(arg_p, dgt_v)	sprintf((char*)arg_p, "%d", dgt_v)
#define MAKE_TCMD_CHAR(arg_p, char_v)	sprintf((char*)arg_p, "%c", char_v)
#define MAKE_TCMD_ADDR(arg_p, a1_p, a2_p, a3_p, a4_p) \
	sprintf((char*)arg_p, "%d.%d.%d.%d", a1_p, a2_p, a3_p, a4_p)
#define MAKE_TCMD_STRING(arg_p, argsize_v, str_p) do { \
	uint8 len = strlen((char*)str_p); \
	if(len > argsize_v-1) {memcpy((char*)arg_p, (char*)str_p, argsize_v-1); arg_p[argsize_v-1]=0;} \
	else {memcpy((char*)arg_p, (char*)str_p, len); arg_p[len]=0;} \
} while(0)
#define EVENT_RESP(id_v, evt_v)			printf("[V,%d,%d]\r\n", id_v, evt_v)
#define EVENT_RESP_SIZE(id_v, evt_v, size_v) printf("[V,%d,%d,%d]\r\n", id_v, evt_v, size_v)

enum {
	SOCKEVENT_CONN		= 0,
	SOCKEVENT_DISCON	= 1,
	SOCKEVENT_CLS		= 2,
	SOCKEVENT_RECV		= 3
};

static int8_t   sockstat[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO] = {0,};	// for sock check (0 is ignored)
static int8_t   sockbusy[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO] = {0,};	// for sock busy check
static uint16_t sockport[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO] = {0,};	// for src port check
static uint8_t  udpip[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO][4] = {{0,},};	// to store UDP Destination address
static uint16_t udpport[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO]  = {0,};	// to store UDP Destination port
static int8_t   recvflag[ATC_SOCK_NUM_TOTAL+ATC_SOCK_AO] = {0,};	// for recv check

static int8_t sock_get(int8_t initval, uint16_t srcport)
{
	int8_t i;

	DBGCRTCA(initval<1||initval>3, "wrong init value(%d)", initval);

	for(i=ATC_SOCK_NUM_START; i<=ATC_SOCK_NUM_END; i++) {
		if(sockstat[i] == SOCK_STAT_IDLE) {
			sockstat[i] = initval;
			sockport[i] = srcport;			//DBGA("Sock(%d) Assign stt(%d), port(%d)", i, sockstat[i], sockport[i]);
			return i;
		}
	}

	return RET_NOK;
}

static int8_t sock_put(uint8_t sock)
{
	DBGCRTCA(sock<ATC_SOCK_NUM_START||sock>ATC_SOCK_NUM_END, "wrong sock(%d)", sock);

	if(sockstat[sock] & SOCK_STAT_UDP) {
		udpip[sock][0] = udpip[sock][1] = udpip[sock][2] = 0;
		udpip[sock][3] = udpport[sock] = 0;
	}
	sockstat[sock] = SOCK_STAT_IDLE;
	sockport[sock] = 0;
	sockwatch_clr(sock, WATCH_SOCK_ALL_MASK);

	return RET_OK;
}

void atc_async_cb(uint8_t sock, uint8_t item, int32_t ret)
{
	DBGCRTCA(sock<ATC_SOCK_NUM_START||sock>ATC_SOCK_NUM_END, "wrong sock(%d)", sock);

	switch(item) {
	case WATCH_SOCK_UDP_SEND:	DBG("WATCH_SOCK_UDP_SEND");
		sockbusy[sock] = VAL_FALSE;	//DBGA("WATCH UDP Sent - sock(%d), item(%d)", sock, item);
		if(ret == RET_OK) {
			cmd_resp(RET_OK, sock);
		} else {
			DBGA("WATCH_SOCK_UDP_SEND fail - ret(%d)", ret);
			cmd_resp(RET_TIMEOUT, sock);
		}
		break;
	case WATCH_SOCK_TCP_SEND:	DBG("WATCH_SOCK_TCP_SEND");
		// 블로??모드로만 ?�작??그러므�?Watch???�요가 ?�음
		break;
	case WATCH_SOCK_CONN_TRY:	DBG("WATCH_SOCK_CONN_TRY");
		sockbusy[sock] = VAL_FALSE;
		if(ret == RET_OK) {
			BITSET(sockstat[sock], SOCK_STAT_CONNECTED);
			sockwatch_set(sock, WATCH_SOCK_CLS_EVT);
			sockwatch_set(sock, WATCH_SOCK_RECV);
			cmd_resp(RET_OK, sock);
		} else {
			DBGA("WATCH_SOCK_CONN_EVT fail - ret(%d)", ret);
			cmd_resp(RET_TIMEOUT, sock);
			sock_put(sock);
		}
		break;
	case WATCH_SOCK_CLS_TRY:	DBG("WATCH_SOCK_CLS_TRY");
		sockbusy[sock] = VAL_FALSE;
		if(ret == RET_OK) {
			cmd_resp(RET_OK, sock);
			sock_put(sock);
		} else {
			CRITICAL_ERRA("WATCH_SOCK_CONN_EVT fail - ret(%d)", ret);
		}
		break;
	case WATCH_SOCK_CONN_EVT:	DBG("WATCH_SOCK_CONN_EVT");
		if(ret == RET_OK) {
			BITSET(sockstat[sock], SOCK_STAT_CONNECTED);
			sockwatch_set(sock, WATCH_SOCK_CLS_EVT);
			sockwatch_set(sock, WATCH_SOCK_RECV);
			EVENT_RESP(sock, SOCKEVENT_CONN);
		} else {
			CRITICAL_ERRA("WATCH_SOCK_CONN_EVT fail - ret(%d)", ret);
		}
		break;
	case WATCH_SOCK_CLS_EVT:	DBG("WATCH_SOCK_CLS_EVT");
		sockbusy[sock] = VAL_FALSE;
		if(ret == RET_OK) {
			if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_TCP_SRV) {
				DBGA("Conn(%d) Closed - back to the Listen state", sock);
				BITCLR(sockstat[sock], SOCK_STAT_CONNECTED);
				sockwatch_clr(sock, WATCH_SOCK_ALL_MASK);

				socket(sock, Sn_MR_TCP, sockport[sock], SF_IO_NONBLOCK);
				ret = listen(sock);

				if(ret == SOCK_OK) {
					sockwatch_set(sock, WATCH_SOCK_CONN_EVT);
					EVENT_RESP(sock, SOCKEVENT_DISCON);
				} else {
					sock_put(sock);
					EVENT_RESP(sock, SOCKEVENT_CLS);
				}
			} else {
				sock_put(sock);
				EVENT_RESP(sock, SOCKEVENT_CLS);
			}
		} else {
			CRITICAL_ERRA("WATCH_SOCK_CONN_EVT fail - ret(%d)", ret);
		}
		break;
	case WATCH_SOCK_RECV:	DBG("WATCH_SOCK_RECV");
		act_nrecv(sock, WORK_BUF_SIZE);
		break;
	default: CRITICAL_ERRA("wrong item(0x%x)", item);
	}
}

void act_nset_q(int8_t num)
{
	wiz_NetInfo ni = {{0, },};

	ctlnetwork(CN_GET_NETINFO, (void*) &ni);
	if(num == 1) {
		if(ni.dhcp == NETINFO_DHCP) 
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'D');
		else MAKE_TCMD_CHAR(atci.tcmd.arg1, 'S');
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} else if(num > 1) {
		switch(num) {
		case 2: MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.ip[0], ni.ip[1], ni.ip[2], ni.ip[3]); break;
		case 3: MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.sn[0], ni.sn[1], ni.sn[2], ni.sn[3]); break;
		case 4: MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.gw[0], ni.gw[1], ni.gw[2], ni.gw[3]); break;
		case 5: MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.dns[0], ni.dns[1], ni.dns[2], ni.dns[3]); break;
		case 6: cmd_resp(RET_NOT_ALLOWED, VAL_NONE); return;
		}
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} else {
		if(ni.dhcp == NETINFO_DHCP) 
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'D');
		else MAKE_TCMD_CHAR(atci.tcmd.arg1, 'S');
		dhcp_get_storage(&ni);
		MAKE_TCMD_ADDR(atci.tcmd.arg2, ni.ip[0], ni.ip[1], ni.ip[2], ni.ip[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg3, ni.sn[0], ni.sn[1], ni.sn[2], ni.sn[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg4, ni.gw[0], ni.gw[1], ni.gw[2], ni.gw[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg5, ni.dns[0], ni.dns[1], ni.dns[2], ni.dns[3]);
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		ARG_CLEAR(atci.tcmd.arg2);
		ARG_CLEAR(atci.tcmd.arg3);
		ARG_CLEAR(atci.tcmd.arg4);
		ARG_CLEAR(atci.tcmd.arg5);
		return;
	}
}

void act_nset_a(int8_t mode, uint8_t *ip, uint8_t *sn, uint8_t *gw, uint8_t *dns1, uint8_t *dns2)
{
	wiz_NetInfo ni = {{0, },};

	get_S2E_Packet(&ni);

	if(ip) memcpy(ni.ip, ip, 4);
	if(sn) memcpy(ni.sn, sn, 4);
	if(gw) memcpy(ni.gw, gw, 4);
	if(dns1) memcpy(ni.dns, dns1, 4);
	if(dns2) {
		MAKE_TCMD_DIGIT(atci.tcmd.arg1, 6);
		cmd_resp(RET_NOT_ALLOWED, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	}

	if(mode == 'S') 
		ni.dhcp = NETINFO_STATIC;
	if(mode == 'D') 
		ni.dhcp = NETINFO_DHCP;

	set_S2E_Packet(&ni);
	Net_Conf();
	cmd_resp(RET_OK, VAL_NONE);
	return;
}

void act_nstat(int8_t num)
{
	wiz_NetInfo ni = {{0},};

	//GetNetInfo(&ni);
	ctlnetwork(CN_GET_NETINFO, (void*) &ni);
	if(num == 1) {
		if(ni.dhcp == NETINFO_DHCP) 
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'D');
		else 
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'S');
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} 
	else if(num > 1) {
		switch(num) {
			case 2:
				MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.ip[0], ni.ip[1], ni.ip[2], ni.ip[3]); 
				break;
			case 3: 
				MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.sn[0], ni.sn[1], ni.sn[2], ni.sn[3]); 
				break;
			case 4:
				MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.gw[0], ni.gw[1], ni.gw[2], ni.gw[3]); 
				break;
			case 5:
				MAKE_TCMD_ADDR(atci.tcmd.arg1, ni.dns[0], ni.dns[1], ni.dns[2], ni.dns[3]); 
				break;
			case 6:
				cmd_resp(RET_NOT_ALLOWED, VAL_NONE); 
				return;
		}
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} 
	else {
		if(ni.dhcp == NETINFO_DHCP) 
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'D');
		else
			MAKE_TCMD_CHAR(atci.tcmd.arg1, 'S');
		MAKE_TCMD_ADDR(atci.tcmd.arg2, ni.ip[0], ni.ip[1], ni.ip[2], ni.ip[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg3, ni.sn[0], ni.sn[1], ni.sn[2], ni.sn[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg4, ni.gw[0], ni.gw[1], ni.gw[2], ni.gw[3]);
		MAKE_TCMD_ADDR(atci.tcmd.arg5, ni.dns[0], ni.dns[1], ni.dns[2], ni.dns[3]);
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		ARG_CLEAR(atci.tcmd.arg2);
		ARG_CLEAR(atci.tcmd.arg3);
		ARG_CLEAR(atci.tcmd.arg4);
		ARG_CLEAR(atci.tcmd.arg5);
		return;
	}
}

void act_nmac_q(void)
{
	wiz_NetInfo ni = {{0, },};

	ctlnetwork(CN_GET_NETINFO, (void*) &ni);
	sprintf((char*)atci.tcmd.arg1, "%02x:%02x:%02x:%02x:%02x:%02x", 
		ni.mac[0], ni.mac[1], ni.mac[2], ni.mac[3], ni.mac[4], ni.mac[5]);
	cmd_resp(RET_OK, VAL_NONE);
}

void act_nmac_a(uint8_t *mac)
{
#if 1 // Enable MAC change
	wiz_NetInfo ni = {{0, },};

	set_mac(mac);
	save_S2E_Packet_to_eeprom();

	ctlnetwork(CN_GET_NETINFO, (void*) &ni);
	memcpy(ni.mac, mac, 6);
	ctlnetwork(CN_SET_NETINFO, (void*) &ni);
	cmd_resp(RET_OK, VAL_NONE);
#else
	cmd_resp(RET_NOT_ALLOWED, VAL_NONE);
#endif
}

void act_nopen_q(void)
{
	cmd_resp(RET_NOT_ALLOWED, VAL_NONE);
}

void act_nopen_a(int8_t type, uint16_t sport, uint8_t *dip, uint16_t dport)
{
	int8_t sock, i;
	int8_t ret;

	for(i=ATC_SOCK_NUM_START; i<=ATC_SOCK_NUM_END; i++) {
		if(sockstat[i] != SOCK_STAT_IDLE && sockport[i] == sport) {
			DBGA("src port(%d) is using now by sock(%d)", sport, i);
			MAKE_TCMD_DIGIT(atci.tcmd.arg1, 2);
			cmd_resp(RET_USING_PORT, VAL_NONE);
			ARG_CLEAR(atci.tcmd.arg1);
			return;
		}
	}

	if(type == 'S') {
		sock = sock_get(SOCK_STAT_TCP_SRV, sport);
		if(sock == RET_NOK) {
			cmd_resp(RET_NO_SOCK, VAL_NONE);
			return;
		}

		socket(sock, Sn_MR_TCP, sport, SF_IO_NONBLOCK);
		if(listen(sock) != SOCK_OK) {
			cmd_resp(RET_UNSPECIFIED, VAL_NONE);
			return;
		}

		sockwatch_set(sock, WATCH_SOCK_CONN_EVT);
		MAKE_TCMD_DIGIT(atci.tcmd.arg1, sock);
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;

	} else if(type == 'C') {
		sock = sock_get(SOCK_STAT_TCP_CLT, sport);
		if(sock == RET_NOK) {
			cmd_resp(RET_NO_SOCK, VAL_NONE);
			return;
		}
		socket(sock, Sn_MR_TCP, sport, SF_IO_NONBLOCK);
		ret = connect(sock, dip, dport);
		if(ret != SOCK_BUSY && ret != SOCK_OK) {
			DBGA("connect fail - ret(%d)", ret);
			cmd_resp(RET_WRONG_ADDR, VAL_NONE);
			return;
		}
		sockwatch_set(sock, WATCH_SOCK_CONN_TRY);
		sockbusy[sock] = VAL_TRUE;
		cmd_resp(RET_ASYNC, sock);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} else if(type == 'U') {
		sock = sock_get(SOCK_STAT_UDP, sport);
		if(sock == RET_NOK) {
			cmd_resp(RET_NO_SOCK, VAL_NONE);
			return;
		}
		if(dip != NULL) {
			memcpy(udpip[sock], dip, 4);
			udpport[sock] = dport;
		}
		socket(sock, Sn_MR_UDP, sport, SF_IO_NONBLOCK);
		sockwatch_set(sock, WATCH_SOCK_RECV);
		MAKE_TCMD_DIGIT(atci.tcmd.arg1, sock);
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	}
	else {	// 'A' mode
		S2E_Packet *packet = get_S2E_Packet_pointer();

		switch(packet->network_info[0].working_mode) {
		case TCP_CLIENT_MODE:
			sock = sock_get(SOCK_STAT_TCP_CLT, packet->network_info[0].local_port);
			if(sock == RET_NOK) {
				cmd_resp(RET_NO_SOCK, VAL_NONE);
				return;
			}
			socket(sock, Sn_MR_TCP, packet->network_info[0].local_port, SF_IO_NONBLOCK);
			ret = connect(sock, packet->network_info[0].remote_ip, packet->network_info[0].remote_port);
			if(ret != SOCK_BUSY && ret != SOCK_OK) {
				DBGA("connect fail - ret(%d)", ret);
				cmd_resp(RET_WRONG_ADDR, VAL_NONE);
				return;
			}
			sockwatch_set(sock, WATCH_SOCK_CONN_TRY);
			sockbusy[sock] = VAL_TRUE;
			cmd_resp(RET_ASYNC, sock);
			break;
		case TCP_SERVER_MODE:
			sock = sock_get(SOCK_STAT_TCP_SRV, packet->network_info[0].local_port);
			socket(sock, Sn_MR_TCP, packet->network_info[0].local_port, SF_IO_NONBLOCK);
			if(listen(sock) != SOCK_OK) {
				cmd_resp(RET_UNSPECIFIED, VAL_NONE);
				return;
			}

			sockwatch_set(sock, WATCH_SOCK_CONN_EVT);
			MAKE_TCMD_DIGIT(atci.tcmd.arg1, sock);
			cmd_resp(RET_OK, VAL_NONE);
			break;
		case TCP_MIXED_MODE:
			cmd_resp(RET_NOT_ALLOWED, VAL_NONE);
			break;
		case UDP_MODE:
			sock = sock_get(SOCK_STAT_UDP, packet->network_info[0].local_port);
			if(sock == RET_NOK) {
				cmd_resp(RET_NO_SOCK, VAL_NONE);
				return;
			}

			memcpy(udpip[sock], packet->network_info[0].remote_ip, 4);
			udpport[sock] = packet->network_info[0].remote_port;

			socket(sock, Sn_MR_UDP, packet->network_info[0].local_port, SF_IO_NONBLOCK);
			sockwatch_set(sock, WATCH_SOCK_RECV);

			MAKE_TCMD_DIGIT(atci.tcmd.arg1, sock);
			cmd_resp(RET_OK, VAL_NONE);
			break;
		default:
			break;
		}
		ARG_CLEAR(atci.tcmd.arg1);
	}
}

void act_nclose(uint8_t sock)
{
	int8_t ret;

	if(sockbusy[sock] == VAL_TRUE) {
		cmd_resp(RET_BUSY, VAL_NONE);
	} else if(sockstat[sock] == SOCK_STAT_IDLE) {
		cmd_resp(RET_SOCK_CLS, VAL_NONE);
	} else if(sockstat[sock] & SOCK_STAT_TCP_MASK) {
		ret = disconnect(sock);
		if(ret != SOCK_BUSY && ret != SOCK_OK) {
			cmd_resp(RET_SOCK_CLS, VAL_NONE);
			return;
		}
		sockwatch_clr(sock, WATCH_SOCK_CLS_EVT);
		sockwatch_set(sock, WATCH_SOCK_CLS_TRY);
		sockbusy[sock] = VAL_TRUE;
		cmd_resp(RET_ASYNC, sock);
	} else {
		close(sock);
		sock_put(sock);
		cmd_resp(RET_OK, VAL_NONE);
	}
}

int8_t act_nsend_chk(uint8_t sock, uint16_t *len, uint8_t *dip, uint16_t *dport){
	uint16_t availlen = 0;

	if(sockbusy[sock] == VAL_TRUE) {
		cmd_resp(RET_BUSY, VAL_NONE);
		return RET_NOK;
	}
	if(sockstat[sock] == SOCK_STAT_IDLE) {
		cmd_resp(RET_SOCK_CLS, VAL_NONE);
		return RET_NOK;
	}

	if(sockstat[sock] & SOCK_STAT_TCP_MASK) {	// TCP
		if(!(sockstat[sock] & SOCK_STAT_CONNECTED)) {
			cmd_resp(RET_NOT_CONN, VAL_NONE);
			return RET_NOK;
		}
		if(dip || dport) {
			if(dip) MAKE_TCMD_DIGIT(atci.tcmd.arg1, 3);
			else MAKE_TCMD_DIGIT(atci.tcmd.arg1, 4);
			cmd_resp(RET_WRONG_ARG, VAL_NONE);
			ARG_CLEAR(atci.tcmd.arg1);
			return RET_NOK;
		}
	} else {									// UDP
		if(dip == NULL) {
			if(udpip[sock][0]==0 && udpip[sock][1]==0 &&
				udpip[sock][2]==0 && udpip[sock][3]==0) {
				DBG("No Predefined Dst IP");
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, 3);
				cmd_resp(RET_WRONG_ARG, VAL_NONE);
				ARG_CLEAR(atci.tcmd.arg1);
				return RET_NOK;
			} else memcpy(atci.sendip, udpip[sock], 4);
		}
		if(dport == NULL) {
			if(udpport[sock] == 0) {
				DBG("No Predefined Dst Port");
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, 4);
				cmd_resp(RET_WRONG_ARG, VAL_NONE);
				ARG_CLEAR(atci.tcmd.arg1);
				return RET_NOK;
			} else atci.sendport = udpport[sock];
		}
	}

	getsockopt(sock, SO_SENDBUF, &availlen);
	if(*len > availlen) {
		DBGA("tx buf busy - req(%d), avail(%d)", *len, availlen);
		MAKE_TCMD_DIGIT(atci.tcmd.arg1, availlen);
		cmd_resp(RET_BUSY, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return RET_NOK;
	}

	return RET_OK;
}
void act_nsend(uint8_t sock, int8_t *buf, uint16_t len, uint8_t *dip, uint16_t *dport){
	int32_t ret;
	uint16_t availlen = 0;
	uint8_t io_mode;

	if(sockstat[sock] & SOCK_STAT_TCP_MASK) {	// TCP
		io_mode = SOCK_IO_NONBLOCK;
		ctlsocket(sock, CS_SET_IOMODE, &io_mode);

		ret = send(sock, (uint8_t *)buf, len);

		io_mode = SOCK_IO_BLOCK;
		ctlsocket(sock, CS_SET_IOMODE, &io_mode);

		if(ret == SOCK_BUSY) {
			getsockopt(sock, SO_SENDBUF, &availlen);
			CRITICAL_ERRA("Impossible TCP send busy - len(%d), avail(%d)", len, availlen);
		}
		if(ret < SOCK_BUSY) {
			cmd_resp(RET_NOT_CONN, VAL_NONE);
			return;
		}
		sockbusy[sock] = VAL_FALSE;
		cmd_resp(RET_OK, sock);

	} else {									// UDP
		uint8_t remote_ip[4];
		uint16_t remote_port;
		// To void error
		memcpy(remote_ip, dip, 4);
		remote_port = *dport;

		ret = sendto(sock, (uint8_t *)buf, len, remote_ip, remote_port);
/*
		if(ret == SOCK_BUSY) {
			getsockopt(sock, SO_SENDBUF, &availlen);
			CRITICAL_ERRA("Impossible TCP send busy - len(%d), avail(%d)", len, availlen);
		}
*/
		if(ret < SOCK_BUSY) {
			DBGA("sendto fail - ret(%d)", ret);
			cmd_resp(RET_WRONG_ADDR, VAL_NONE);
			return;
		} else {
			DBGA("sendto SUCC - len(%d),sent(%d)", len, ret);
		}
		sockwatch_set(sock, WATCH_SOCK_UDP_SEND);
		sockbusy[sock] = VAL_TRUE;
	}
}

void act_nrecv(int8_t sock, uint16_t maxlen){
	int8_t ret;
	uint8_t dstip[4], i;
	uint16_t dstport;
	int32_t len=0, offset=0;
	uint16_t recvsize;

	if(sock == VAL_NONE) {	DBG("sock==NONE");
		for(i=ATC_SOCK_NUM_START; i<=ATC_SOCK_NUM_END; i++) {
			if(recvflag[i] == VAL_SET) {
				DBGA("Found(%d)", i);
				sock = i;
				break;
			}
		}
		if(i > ATC_SOCK_NUM_END) {
			DBG("no recv data");
			ret = RET_NO_DATA;
			goto FAIL_RET;
		}
	}

	if(sockstat[sock] == SOCK_STAT_IDLE) {
		ret = RET_SOCK_CLS;
		goto FAIL_RET;
	}

	if(sockstat[sock] & SOCK_STAT_TCP_MASK) {	// TCP		DBGA("TCPdbg---rx(%d)",GetSocketRxRecvBufferSize(sock));
		if(!(sockstat[sock] & SOCK_STAT_CONNECTED)) {
			DBGA("not connected - sock(%d)", sock);
			ret = RET_NOT_CONN;
			goto FAIL_RET;
		}
		getsockopt(sock, SO_RECVBUF, (uint16_t*)&recvsize);
		if(recvsize == 0) {
			DBGA("no data - sock(%d)", sock);
			ret = RET_NO_DATA;
			goto FAIL_RET;
		}
		len = recv(sock, (uint8_t*)atci.recvbuf, maxlen);			//DBGA("TCPdbg---m(%d)l(%d)f(%d)", maxlen, len, GetSocketRxRecvBufferSize(sock));
	} else {									// UDP
		uint16_t bufleft = maxlen;

		getsockopt(sock, SO_RECVBUF, (uint16_t*)&recvsize);
		if(recvsize == 0) {
			DBGA("no data - sock(%d)", sock);
			ret = RET_NO_DATA;
			goto FAIL_RET;
		}

		if(bufleft < recvsize) {
			DBGA("buffer not enough - sock(%d),buf(%d),recv(%d)", sock, bufleft, recvsize);
			ret = RET_NO_FREEMEM;
			goto FAIL_RET;
		}

		offset = recvfrom(sock, (uint8_t*)&atci.recvbuf[len], bufleft, dstip, &dstport);
		if(offset <= 0 || offset > (int32_t)bufleft) {	// Abnormal case - I don't think this could happen but just in case.
			if(offset > (int32_t)bufleft) {
				ERRA("buf overflw - off(%d), maxlen(%d)", offset, bufleft);
				if(len == 0) {
					ret = RET_UNSPECIFIED;
					goto FAIL_RET;
				}
				bufleft = 0;
			} else {
				ERRA("wrong reaction - ret(%d)", offset);
				if(len == 0) {
					if(offset == SOCK_CLOSED) {
						ret = RET_SOCK_CLS;
						goto FAIL_RET;
					} else if(offset < 0) {
						ret = RET_UNSPECIFIED;
						goto FAIL_RET;
					} else {
						ret = RET_NO_DATA;
						goto FAIL_RET;
					}
				}
			}
		} else {			// Normal case
			DBGA("UDP Recv - off(%d), len(%d), maxlen(%d)", offset, len, bufleft);
			len += offset;
			bufleft -= offset;
		}
	}
										//DBGA("RECV prt-len(%d), max(%d)", len, maxlen);
	atci.recvbuf[len] = 0;
	recvflag[sock] = VAL_CLEAR;

	if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_IDLE) {	// ?�버그용?? ?�정?�면 간단?�게 ?�정??�
		CRITICAL_ERRA("Impossible status - recv from closed sock(%d)", sock);
	} else if(sockstat[sock] & SOCK_STAT_TCP_MASK) {	// TCP
		if(sockstat[sock] & SOCK_STAT_CONNECTED)
			sockwatch_set(sock, WATCH_SOCK_RECV);
	} else if(sockstat[sock] & SOCK_STAT_UDP) {
		sockwatch_set(sock, WATCH_SOCK_RECV);
	} else CRITICAL_ERRA("Impossible status - wrong sock state(0x%x)", sockstat[sock]);

	MAKE_TCMD_DIGIT(atci.tcmd.arg1, len);
	if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_UDP) {
		MAKE_TCMD_ADDR(atci.tcmd.arg2, dstip[0], dstip[1], dstip[2], dstip[3]);
		MAKE_TCMD_DIGIT(atci.tcmd.arg3, dstport);
	}
	cmd_resp(RET_RECV, sock);
	ARG_CLEAR(atci.tcmd.arg1);
	ARG_CLEAR(atci.tcmd.arg2);
	ARG_CLEAR(atci.tcmd.arg3);

	UART_write(atci.recvbuf, len);
	UART_write("\r\n", 2);

	return;

FAIL_RET:			//DBG("FAIL RET");

	cmd_resp(ret, VAL_NONE);
}

void act_nsock(int8_t sock)
{
	uint8_t tip[4];
	uint16_t tport;

	if(sock < ATC_SOCK_NUM_START)
	{
		int8_t *dump, i, type, cnt_con=0, cnt_notcon=0;
		uint16_t dump_idx = 0;

		//DBG("NSOCK-start");
		for(i=ATC_SOCK_NUM_START; i<=ATC_SOCK_NUM_END; i++) {
			if(sockstat[i] != SOCK_STAT_IDLE) {
				if((sockstat[i] & SOCK_STAT_PROTMASK) == SOCK_STAT_UDP) {
					if(udpport[i] != 0) cnt_con++;
					else cnt_notcon++;
				} else {
					if(sockstat[i] & SOCK_STAT_CONNECTED) cnt_con++;
					else cnt_notcon++;
				}
			}
		} //DBGA("NSOCK-con(%d),not(%d)", cnt_con, cnt_notcon);

 		if(cnt_con+cnt_notcon == 0) {
			cmd_resp_dump(VAL_NONE, NULL);
			return;
 		}
		dump = malloc((36*cnt_con)+(16*cnt_notcon)+1);			//DBGA("DUMP BUF(%d),C(%d),N(%d)", (36*cnt_con)+(14*cnt_notcon)+1, cnt_con, cnt_notcon);
		if(dump == NULL) {
			cmd_resp(RET_NO_FREEMEM, VAL_NONE);
			return;
		}

		for(i=ATC_SOCK_NUM_START; i<=ATC_SOCK_NUM_END; i++) {	//DBGA("Checking Sock(%d)", i);
			if(sockstat[i] == SOCK_STAT_IDLE) continue;			//DBGA("Sock(%d) is NOT Idle", i);
			if(dump_idx != 0) {
				dump[dump_idx++] = '\r';
				dump[dump_idx++] = '\n';
			}

			if((sockstat[i]&SOCK_STAT_PROTMASK)==SOCK_STAT_UDP) {
				if(udpport[i]) {	//DBGA("U1s-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
					sprintf((char*)&dump[dump_idx], "%d,%c,%d,%d.%d.%d.%d,%d%s", i, 'U',
						sockport[i], udpip[i][0], udpip[i][1], udpip[i][2], udpip[i][3],
						udpport[i], recvflag[i]==VAL_SET? ",R": "");		//DBGA("U1e-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
				} else {			//DBGA("U2s-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
					sprintf((char*)&dump[dump_idx], "%d,%c,%d%s", i, 'U', sockport[i],
						recvflag[i]==VAL_SET? ",,,R": "");					//DBGA("U2e-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
				}
			} else {
				if((sockstat[i]&SOCK_STAT_PROTMASK)==SOCK_STAT_TCP_SRV) type = 'S';
				else type='C';
				if(sockstat[i] & SOCK_STAT_CONNECTED) {		//DBGA("T1s-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
					getsockopt(i, SO_DESTIP, tip);
					getsockopt(i, SO_DESTPORT, &tport);
					sprintf((char*)&dump[dump_idx], "%d,%c,%d,%d.%d.%d.%d,%d%s", i,
						type, sockport[i], tip[0], tip[1], tip[2], tip[3], tport,
						recvflag[i]==VAL_SET? ",R": "");	//DBGA("T1e-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
				} else {									//DBGA("T2s-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
					sprintf((char*)&dump[dump_idx], "%d,%c,%d", i, type, sockport[i]);	//DBGA("T2e-cnt(%d)-0x%02x-0x%02x-0x%02x-0x%02x", dump_idx, dump[dump_idx], dump[dump_idx+1], dump[dump_idx+2], dump[dump_idx+3]);
				}
			}
			dump_idx += strlen((char*)&dump[dump_idx]);		//DBGA("cnt(%d),DUMP(%s)", dump_idx, dump);
		}
		cmd_resp_dump(VAL_NONE, dump);
	}
	else if(sock <= ATC_SOCK_NUM_END)
	{
		if(sockstat[sock] == SOCK_STAT_IDLE) {
			sprintf((char*)atci.tcmd.arg1, "%c", 'I');
		} else {
			if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_UDP) {
				sprintf((char*)atci.tcmd.arg1, "%c", 'U');
				sprintf((char*)atci.tcmd.arg2, "%d", sockport[sock]);
				if(udpport[sock]) {
					sprintf((char*)atci.tcmd.arg3, "%d.%d.%d.%d",
						udpip[sock][0], udpip[sock][1], udpip[sock][2], udpip[sock][3]);
					sprintf((char*)atci.tcmd.arg4, "%d", udpport[sock]);
				}
			} else {
				if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_TCP_SRV)
					sprintf((char*)atci.tcmd.arg1, "%c", 'S');
				else if((sockstat[sock] & SOCK_STAT_PROTMASK) == SOCK_STAT_TCP_CLT)
					sprintf((char*)atci.tcmd.arg1, "%c", 'C');
				else CRITICAL_ERRA("wrong sock state(0x%d)", sockstat[sock]);
				sprintf((char*)atci.tcmd.arg2, "%d", sockport[sock]);
				if(sockstat[sock] & SOCK_STAT_CONNECTED) {
					getsockopt(sock, SO_DESTIP, tip);
					getsockopt(sock, SO_DESTPORT, &tport);
					sprintf((char*)atci.tcmd.arg3, "%d.%d.%d.%d",
						tip[0], tip[1], tip[2], tip[3]);
					sprintf((char*)atci.tcmd.arg4, "%d", tport);
				}
			}
		}
		cmd_resp(RET_OK, VAL_NONE);
	}
	else cmd_resp(RET_WRONG_ARG, VAL_NONE);
	ARG_CLEAR(atci.tcmd.arg1);
	ARG_CLEAR(atci.tcmd.arg2);
	ARG_CLEAR(atci.tcmd.arg3);
	ARG_CLEAR(atci.tcmd.arg4);
}

void act_mset_q(int8_t num){}
void act_mset_a(int8_t echo, int8_t poll, int8_t country){}

void act_mstat(void)
{
	sprintf((char *)(atci.tcmd.arg1), "%d.%d.%d", MAJOR_VER, MINOR_VER, MAINTENANCE_VER);
	cmd_resp(RET_OK, VAL_NONE);
	ARG_CLEAR(atci.tcmd.arg1);
}

char parity_char[] = {
	'N', 'O', 'E'
};
void act_uart_q(int8_t num)
{
	struct __serial_info *serial = (struct __serial_info *)get_S2E_Packet_pointer()->serial_info;

	if(num >= 1) {
		switch(num) {
			case 1:
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, serial->baud_rate);
				break;
			case 2:
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, serial->data_bits);
				break;
			case 3:
				MAKE_TCMD_CHAR(atci.tcmd.arg1, parity_char[serial->parity]);
				break;
			case 4:
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, serial->stop_bits);
				break;
			case 5:
				MAKE_TCMD_DIGIT(atci.tcmd.arg1, serial->flow_control);
				break;
			default:
				cmd_resp(RET_NOT_ALLOWED, VAL_NONE); 
				break;
		}
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		return;
	} else {
		MAKE_TCMD_DIGIT(atci.tcmd.arg1, serial->baud_rate);
		MAKE_TCMD_DIGIT(atci.tcmd.arg2, serial->data_bits);
		MAKE_TCMD_CHAR(atci.tcmd.arg3, parity_char[serial->parity]);
		MAKE_TCMD_DIGIT(atci.tcmd.arg4, serial->stop_bits);
		MAKE_TCMD_DIGIT(atci.tcmd.arg5, serial->flow_control);
		cmd_resp(RET_OK, VAL_NONE);
		ARG_CLEAR(atci.tcmd.arg1);
		ARG_CLEAR(atci.tcmd.arg2);
		ARG_CLEAR(atci.tcmd.arg3);
		ARG_CLEAR(atci.tcmd.arg4);
		ARG_CLEAR(atci.tcmd.arg5);
	}
}

void act_uart_a(struct __serial_info *serial)
{
	cmd_resp(RET_OK, VAL_NONE);
	while(RingBuffer_GetCount(&txring) != 0);
	delay_cnt(999999);
	serial_info_init(serial);
}

void act_mdata(void)
{
	uint8_t sock_state;

	getsockopt(SOCK_DATA, SO_STATUS, &sock_state);

	disconnect(SOCK_DATA);

	UART_buffer_flush(&rxring);
	UART_buffer_flush(&txring);
	op_mode = OP_DATA;

	sock_put(SOCK_DATA);
	close(SOCK_DATA);

	cmd_resp(RET_OK, VAL_NONE);
}

void act_msave(void)
{
	save_S2E_Packet_to_eeprom();
	cmd_resp(RET_OK, VAL_NONE);
}

void act_fdns(char *url)
{
	int8_t ret;
	uint8_t backup;
	S2E_Packet *value = get_S2E_Packet_pointer();
	uint8_t dns_server_ip[4], dns_domain_name[50];
	char str[16];

	memcpy(dns_server_ip, value->options.dns_server_ip, sizeof(dns_server_ip));
	if(url == NULL) {
		memcpy(dns_domain_name, value->options.dns_domain_name, 50);
	}
	else {
		memcpy(dns_domain_name, url, strlen(url));
		dns_domain_name[strlen(url)] = '\0';
	}

	backup = value->options.dns_use;
	value->options.dns_use = 1;
	while(1) {
		ret = DNS_run(dns_server_ip, dns_domain_name, value->network_info[0].remote_ip);
		if(ret == 1) {
			sprintf(str, "%d.%d.%d.%d\r\n", value->network_info[0].remote_ip[0], value->network_info[0].remote_ip[1], value->network_info[0].remote_ip[2], value->network_info[0].remote_ip[3]);
			cmd_resp_dump(VAL_NONE, (int8_t *)str);
			break;
		}
		else if(ret == -1) {
			sprintf(str, "DNS Timeout\r\n");
			cmd_resp_dump(VAL_NONE, (int8_t *)str);
			break;
		}
	}
	value->options.dns_use = backup;
}
