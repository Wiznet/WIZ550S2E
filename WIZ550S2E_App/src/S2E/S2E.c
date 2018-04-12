#include <string.h>
#include "common.h"
#include "socket.h"
#include "S2E.h"
#include "ConfigData.h"
#include "ring_buffer.h"

#include "timerHandler.h"
#include "eepromHandler.h"
#include "uartHandler.h"

extern uint8_t g_send_buf[WORK_BUF_SIZE];
extern uint8_t g_recv_buf[WORK_BUF_SIZE];

static uint32_t trigger_state = TRIG_STATE_NONE;
static int uart_size_prev = 0;
static int pattern_cnt = 0;
static int ready_cnt = 0;
static int pattern_offset = 0;

#ifdef RENEWAL_UDP_PORT
	static uint16_t udp_port = 5000;
#endif

uint32_t uart_send_cnt = 0;
uint32_t uart_recv_cnt = 0;
uint32_t ether_send_cnt = 0;
uint32_t ether_recv_cnt = 0;

static int RingBuffer_memcmp(RINGBUFF_T *RingBuff, const uint8_t *pattern, uint32_t start, int n)
{
	uint8_t *ptr;

	while(n-- > 0) {
		ptr = RingBuff->data + (start & (RingBuff->count - 1));

		if(*ptr != *pattern)
			return (*ptr - *pattern);

		start++;
		pattern++;
	}
	return 0;
}

static int RingBuffer_SerachPattern(RINGBUFF_T *RingBuff, int offset, const uint8_t *pattern, uint32_t size)
{
	int i;
	uint32_t buf_size = 0;

	if(RingBuff->itemSz != 1)
		return 0;

	buf_size = RingBuffer_GetCount(RingBuff) - offset;
	if(buf_size < size)
		return 0;

	for(i = 0 ; i < (buf_size - size + 1) ; i++) {
		if(!RingBuffer_memcmp(RingBuff, pattern, RingBuff->tail + offset + i, size))
			return (i + size + offset);
	}
	return 0;
}

static void auth_process(uint8_t sock)
{
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);
	uint8_t sock_state;
	uint16_t len = 0, pw_len = 0;
	static uint16_t prev_len = 0;

	pw_len = strlen(option->pw_connect);

	getsockopt(sock, SO_STATUS, &sock_state);

	if(sock_state == SOCK_ESTABLISHED) {
		getsockopt(sock, SO_RECVBUF, &len);

		if(len != prev_len) {
			auth_time = 0;
			prev_len = len;
		}

		if(auth_time >= 5) {
			disconnect(sock);
			auth_flag = auth_time = 0;
			return;
		}

		if(len >= pw_len) {
			len = recv(sock, g_recv_buf, pw_len);
			if(len != pw_len) {
				disconnect(sock);
				auth_flag = auth_time = 0;
				return;
			}
			
			if(memcmp(option->pw_connect, g_recv_buf, pw_len))		// Password Fault
				disconnect(sock);

			auth_flag = auth_time = 0;
		}
	}
}

static void ether_to_uart(uint8_t sock)
{
	uint8_t sock_state;
	uint8_t dstip[4];
	uint16_t dstport, len = 0;

	getsockopt(sock, SO_STATUS, &sock_state);

	switch (sock_state) {
		case SOCK_UDP:
			getsockopt(sock, SO_RECVBUF, &len);

			if(len > UART_SRB_SIZE)
				len = UART_SRB_SIZE;

			if((len > 0) && len <= RingBuffer_GetFree(&txring)) {
				len = recvfrom(sock, g_recv_buf, sizeof(g_recv_buf), dstip, &dstport);
#ifdef RENEWAL_UDP_PORT
				struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
				udp_port = dstport;
				net->remote_port = udp_port;
//				printf(" peer port : %d \r\n", udp_port); //for debug
#endif
				if(len < 0) {
					//printf("recvfrom error\r\n");
					return;
				}
				ether_recv_cnt += len;
			} else
				len = 0;

			break;

		case SOCK_ESTABLISHED:
			getsockopt(sock, SO_RECVBUF, &len);

			if(len > UART_SRB_SIZE)
				len = UART_SRB_SIZE;

			if((len > 0) && len <= RingBuffer_GetFree(&txring)) {
				len = recv(sock, g_recv_buf, sizeof(g_recv_buf));
				if(len < 0) {
					//printf("recv error\r\n");
					return;
				}
				ether_recv_cnt += len;
			} else
				len = 0;

			break;
		default:
			break;
	}

	if(len) {
		Chip_UART_SendRB(LPC_USART, &txring, g_recv_buf, len);
		uart_send_cnt += len;
	}
}

static void uart_to_ether(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
	uint8_t sock_state, snd_flag = 0, mask_bit = 0;
	uint16_t len = 0;
	int ret, uart_read_len = sizeof(g_send_buf);

	if(RingBuffer_IsEmpty(&rxring))
		return;

	/* Serial Trigger Process */
	if((trigger_state != TRIG_STATE_NONE) && (trigger_state != TRIG_STATE_READY))
		return;

	/* UART Packing Size Check */
	if(net->packing_size)
		mask_bit |= 0x01;

	if(RingBuffer_GetCount(&rxring) < net->packing_size)
		snd_flag |= 0x01;
	else {
		if(net->packing_size)
			uart_read_len = net->packing_size;
	}

	/* Nagle Timer Process */
	if(net->packing_time)
		mask_bit |= 0x02;

	if(!nagle_flag && net->packing_time)
		snd_flag |= 0x02;

	/* Seperator Process */
	if(net->packing_delimiter_length)  {
		mask_bit |= 0x04;
		if(!(ret = RingBuffer_SerachPattern(&rxring, 0, net->packing_delimiter, net->packing_delimiter_length)))
			snd_flag |= 0x04;
		else {
			if(!(mask_bit & 0x01) || (snd_flag & 0x01) || (ret + net->packing_data_appendix < net->packing_size)) {
				uart_read_len = ret + net->packing_data_appendix;

				if(RingBuffer_GetCount(&rxring) < uart_read_len)
					snd_flag |= 0x04;
			}
		}
	}

	//printf("snd_flag = %02x / mask_bit = %02x\r\n", snd_flag, mask_bit);
	if(mask_bit != 0)				// mask_bit : Enable Uart Packing Condition bit
		if((snd_flag & mask_bit) == mask_bit)
			return;

	/* Serial Trigger Process */
	if(trigger_state == TRIG_STATE_READY)
		if(ready_cnt != RingBuffer_GetCount(&rxring))
			return;

	getsockopt(sock, SO_STATUS, &sock_state);

	if(sock_state != SOCK_UDP && sock_state != SOCK_ESTABLISHED)
		return;

	/* Data Transfer */
	len = Chip_UART_ReadRB(LPC_USART, &rxring, &g_send_buf, uart_read_len);
	if(len < 0) {
		//printf("uart recv error\r\n");
		return;
	}
	uart_recv_cnt += len;
	pattern_offset = 0;

	if(sock_state == SOCK_UDP)
	{
		uint8_t remote_ip[4];
		memcpy(remote_ip, net->remote_ip, sizeof(remote_ip));
		ret = sendto(sock, g_send_buf, len, remote_ip, net->remote_port);
//		if(ret != len) {
//			//printf("sendto error - ret : %d  //  len : %d\r\n", ret, len); //for debugging
//			return;
//		}
		ether_send_cnt += len;
	}
	else if(sock_state == SOCK_ESTABLISHED)
	{
		ret = send(sock, g_send_buf, len);
		if(ret != len) {
			//printf("send error\r\n");
			return;
		}
		ether_send_cnt += len;
	}
	nagle_flag = nagle_time = uart_recv_count = 0;
}

static void trigger_none_process(uint8_t sock_state)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	if(trigger_flag == 2) {
		trigger_state = TRIG_STATE_READY;
#ifdef __TRIG_DEBUG__
		printf("TRIG_STATE_READY\r\n");
#endif
		trigger_flag = 0;
		uart_size_prev = RingBuffer_GetCount(&rxring);
		return;
	}

	if(uart_size_prev == RingBuffer_GetCount(&rxring)) {			// UART 수신 데이터가 없으면
		if(trigger_flag == 0)
			trigger_flag = 1;
	} else {
		trigger_flag = trigger_time = 0;
		uart_size_prev = RingBuffer_GetCount(&rxring);
		if((sock_state != SOCK_ESTABLISHED) && (sock_state != SOCK_UDP) && (net->working_mode != TCP_MIXED_MODE)) {
			UART_buffer_flush(&rxring);
			uart_size_prev = 0;
		}
	}
}

static void trigger_ready_process()
{
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);

	if((pattern_cnt = RingBuffer_SerachPattern(&rxring, pattern_offset, option->serial_trigger, 1))) {
		pattern_offset = pattern_cnt;
		trigger_state = TRIG_STATE_1;
#ifdef __TRIG_DEBUG__
		printf("TRIG_STATE_1\r\n");
#endif
		trigger_time = uart_size_prev = 0;
		trigger_flag = 1;
		return;
	} else
		ready_cnt = RingBuffer_GetCount(&rxring);
		
	if(uart_size_prev != RingBuffer_GetCount(&rxring)) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE\r\n", __func__);
#endif
		trigger_flag = trigger_time = uart_size_prev = 0;
	}
}

static void trigger_state1_process()
{
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);
	int len = 0;

	if(trigger_flag == 2) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #1\r\n", __func__);
#endif
		trigger_flag = uart_size_prev = 0;
		return;
	}

	if((len = RingBuffer_SerachPattern(&rxring, pattern_offset - 1, option->serial_trigger, 2))) {
		if((pattern_cnt + 1) == len) {
			trigger_state = TRIG_STATE_2;
#ifdef __TRIG_DEBUG__
			printf("TRIG_STATE_2\r\n");
#endif
			trigger_time = 0;
			pattern_offset = pattern_cnt = len;
			return;
		}
	}

	if(pattern_cnt < RingBuffer_GetCount(&rxring)) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #2\r\n", __func__);
#endif
		trigger_flag = trigger_time = uart_size_prev = 0;
	}
}

static void trigger_state2_process()
{
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);
	int len = 0;

	if(trigger_flag == 2) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #1\r\n", __func__);
#endif
		trigger_flag = uart_size_prev = 0;
		return;
	}

	if((len = RingBuffer_SerachPattern(&rxring, pattern_offset - 2, option->serial_trigger, 3))) {
		if((pattern_cnt + 1) == len) {
			trigger_state = TRIG_STATE_3;
#ifdef __TRIG_DEBUG__
			printf("TRIG_STATE_3\r\n");
#endif
			trigger_time = 0;
			pattern_offset = pattern_cnt = len;
			uart_size_prev = RingBuffer_GetCount(&rxring);
			return;
		}
	}

	if(pattern_cnt < RingBuffer_GetCount(&rxring)) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #2\r\n", __func__);
#endif
		trigger_flag = trigger_time = uart_size_prev = 0;
	}
}

static void trigger_state3_process(uint8_t sock)
{
	if(trigger_flag == 2) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #1\r\n", __func__);
#endif
		trigger_flag = uart_size_prev = 0;
		pattern_offset = 0;

		disconnect(sock);
		UART_buffer_flush(&rxring);
		UART_buffer_flush(&txring);

		Chip_UART_SendRB(LPC_USART, &txring, "\r\n\r\n\r\n[W,0]\r\n", 13);
		Chip_UART_SendRB(LPC_USART, &txring, "[S,0]\r\n", 7);
		op_mode = OP_COMMAND;

		close(sock);
		return;
	}

	if(pattern_cnt < RingBuffer_GetCount(&rxring)) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #2\r\n", __func__);
#endif
		trigger_flag = trigger_time = uart_size_prev = 0;
	}

	if(uart_size_prev != RingBuffer_GetCount(&rxring)) {
		trigger_state = TRIG_STATE_NONE;
#ifdef __TRIG_DEBUG__
		printf("[%s] TRIG_STATE_NONE #3\r\n", __func__);
#endif
		trigger_flag = trigger_time = uart_size_prev = 0;
	}
}

static void s2e_sockclose_process(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	switch(net->working_mode) {
		case TCP_CLIENT_MODE:
		case TCP_SERVER_MODE:
		case TCP_MIXED_MODE:
			//socket(sock, Sn_MR_TCP, net->local_port, 0);
			socket(sock, Sn_MR_TCP, net->local_port, SF_IO_NONBLOCK);
			//printf("TCP Socket\r\n");
			break;

		case UDP_MODE:
			//socket(sock, Sn_MR_UDP, net->local_port, 0);
			socket(sock, Sn_MR_UDP, net->local_port, SF_IO_NONBLOCK);
			//printf("UDP Socket\r\n");
			break;

		default:
			break;
	}
}

#define MIXED_SERVER	0
#define MIXED_CLIENT	1
static int mixed_state = MIXED_SERVER;
static void s2e_sockinit_process(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
	uint8_t remote_ip[4];

	memcpy(remote_ip, net->remote_ip, sizeof(remote_ip));

	switch(net->working_mode) {
		case TCP_CLIENT_MODE:
			if(!reconn_flag) {
				connect(sock, remote_ip, net->remote_port);
				reconn_flag = 1;
			}

			break;

		case TCP_SERVER_MODE:
			listen(sock);
			break;
		case TCP_MIXED_MODE:
			if(RingBuffer_IsEmpty(&rxring)) {
				listen(sock);
				mixed_state = MIXED_SERVER;
			} else {
				if(!reconn_flag) {
					connect(sock, remote_ip, net->remote_port);
					reconn_flag = 1;
				}
				mixed_state = MIXED_CLIENT;
			}
			break;

		case UDP_MODE:
			close(sock);
			break;

		default:
			break;
	}
}

static void s2e_socklisten_process(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	switch(net->working_mode) {
		case TCP_CLIENT_MODE:
			close(sock);
			break;

		case TCP_SERVER_MODE:
			break;

		case TCP_MIXED_MODE:
			if(!RingBuffer_IsEmpty(&rxring))
				close(sock);
			break;
		case UDP_MODE:
			close(sock);
			break;

		default:
			break;
	}
}

static void s2e_sockestablished_process(uint8_t sock)
{
	uint8_t tmp;
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	if(reconn_flag)
		reconn_flag = reconn_time = 0;

	switch(net->working_mode) {
		case TCP_MIXED_MODE:
		case TCP_SERVER_MODE:
			if(auth_flag) {
				auth_process(sock);
				return;
			}
			if((inactive_flag == 0) && net->inactivity)
				inactive_flag = 1;
			else if(inactive_flag == 2) {
				inactive_flag = 0;
				disconnect(sock);
			}
		case TCP_CLIENT_MODE:
			if((inactive_flag == 0) && net->inactivity)
				inactive_flag = 1;
			else if(inactive_flag == 2) {
				inactive_flag = 0;
				disconnect(sock);
			}

			if(keepsend_flag == 0)
			{
				keepsend_flag = 1;
			}
			else if(keepsend_flag == 2)
			{
				keepsend_flag = 0;

				ctlwizchip(CW_GET_PHYLINK, (void*) &tmp);
				if(tmp == PHY_LINK_OFF)
				{
					disconnect(sock);
				}
			}
			ether_to_uart(sock);
			uart_to_ether(sock);
			break;

		case UDP_MODE:
			close(sock);
			break;

		default:
			break;
	}
}

static void s2e_sockclosewait_process(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	switch(net->working_mode) {
		case TCP_CLIENT_MODE:
		case TCP_SERVER_MODE:
		case TCP_MIXED_MODE:
		case UDP_MODE:
			disconnect(sock);
			close(sock);
			break;

		default:
			break;
	}
}

static void s2e_sockudp_process(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;

	switch(net->working_mode) {
		case TCP_CLIENT_MODE:
		case TCP_SERVER_MODE:
		case TCP_MIXED_MODE:
			close(sock);
			break;

		case UDP_MODE:
			/* S2E 동작 */
			ether_to_uart(sock);
			uart_to_ether(sock);
			break;

		default:
			break;
	}
}

#ifdef __TRIG_DEBUG__
static uint32_t tmp1 = TRIG_STATE_NONE;
#endif
void s2e_run(uint8_t sock)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);
	uint8_t sock_state;

	getsockopt(sock, SO_STATUS, &sock_state);

#ifdef __TRIG_DEBUG__
	if(tmp1 != trigger_state) {
//		printf("trigger state : %d\r\n", trigger_state);
//		printf("Uart Prev Size : %d\r\n\r\n", uart_size_prev);
		printf("Pattern Offset : %d\r\n", pattern_offset);
		tmp1 = trigger_state;
	}
#endif

	/* Serial Trigger Process */
	if(option->serial_command)
	{
		switch(trigger_state) {
			case TRIG_STATE_NONE:
				trigger_none_process(sock_state);
				break;
			
			case TRIG_STATE_READY:
				trigger_ready_process();
				break;

			case TRIG_STATE_1:
				trigger_state1_process();
				break;

			case TRIG_STATE_2:
				trigger_state2_process();
				break;

			case TRIG_STATE_3:
				trigger_state3_process(sock);
				if(op_mode == OP_COMMAND)
					return;
				break;

			default:
				break;
		}
	}
	else
	{
		if((sock_state != SOCK_ESTABLISHED) && (sock_state != SOCK_UDP) && (net->working_mode != TCP_MIXED_MODE)) {
			UART_buffer_flush(&rxring);
			uart_size_prev = 0;
		}
	}
	/* Network State Process */
	switch(net->state) {
		case net_disconnect:
			if(sock_state == SOCK_UDP)
				net->state = net_udp;
			else if(sock_state == SOCK_ESTABLISHED) {
				net->state = net_connect;
				Board_LED_Set(1, true);

				if(option->pw_connect[0] != 0) {
					if(net->working_mode == TCP_MIXED_MODE) {
						if(mixed_state == MIXED_SERVER)
							auth_flag = 1;
					} else
						auth_flag = 1;
				}
			}
			break;

		case net_connect:
			if(sock_state != SOCK_ESTABLISHED) {
				net->state = net_disconnect;
				Board_LED_Set(1, false);

				mixed_state = MIXED_SERVER;
			}
			break;

		case net_udp:
			if(sock_state != SOCK_UDP) {
				net->state = net_disconnect;
				Board_LED_Set(1, false);
			}
			break;

		default:
			net->state = net_disconnect;
			Board_LED_Set(1, false);
			break;
	}

	switch (sock_state) {
		case SOCK_CLOSED:
			s2e_sockclose_process(sock);
			break;

		case SOCK_INIT:
			s2e_sockinit_process(sock);
			break;

		case SOCK_LISTEN:
			s2e_socklisten_process(sock);
			break;

		case SOCK_ESTABLISHED:
			s2e_sockestablished_process(sock);
			break;

		case SOCK_CLOSE_WAIT:
			s2e_sockclosewait_process(sock);
			break;

		case SOCK_UDP:
			s2e_sockudp_process(sock);
			break;

		default:
			break;
	}
}
