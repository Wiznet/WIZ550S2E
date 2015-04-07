#include "board.h"
#include "socket.h"
#include "ConfigData.h"
#include "timerHandler.h"
#include "uartHandler.h"
#include "S2E.h"
#include "DHCP/dhcp.h"
#include "DNS/dns.h"

uint8_t nagle_flag = 0;
uint32_t nagle_time = 0;
uint32_t uart_recv_count = 0;

uint8_t reconn_flag = 0;			/* 0 : connect / 1 : NONE */
uint32_t reconn_time = 0;

uint8_t inactive_flag = 0;
uint32_t inactive_time = 0;
uint32_t ether_send_prev;
uint32_t ether_recv_prev;

uint8_t trigger_flag = 0;
uint32_t trigger_time = 0;

uint8_t factory_flag = 0;
uint32_t factory_time = 0;

uint8_t auth_flag = 0;
uint32_t auth_time = 0;

extern uint8_t run_dns;

static uint32_t mill_cnt = 0;
static uint32_t sec_cnt = 0;

extern bool Board_factory_get(void);

/**
 * @brief	Handle interrupt from 32-bit timer 0
 * @return	Nothing
 */
void TIMER32_0_IRQHandler(void)
{
	struct __network_info *net = (struct __network_info *)get_S2E_Packet_pointer()->network_info;
	struct __options *option = (struct __options *)&(get_S2E_Packet_pointer()->options);
	uint32_t count = 0;

	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1))
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);

	mill_cnt++;

	/* UART Packing Timer Process */
	if(!nagle_flag && net->packing_time) {
		count = RingBuffer_GetCount(&rxring);

		if(count != 0 && count == uart_recv_count)
			nagle_time++;
		else {
			nagle_time = 0;
			uart_recv_count = count;
		}

		if(nagle_time >= net->packing_time) {
			nagle_flag = 1;
			nagle_time = 0;
		}
	}

	/* Reconnection Process */
	if(reconn_flag)
		reconn_time++;

	if(net->reconnection <= reconn_time) {
		reconn_flag = 0;
		reconn_time = 0;
	}

	/* Factory Reset Process */
	if(factory_flag) {
		factory_time++;

		if(Board_factory_get()) {
			factory_time = factory_flag = 0;
			NVIC_EnableIRQ(PIN_INT0_IRQn);
		} 

		if(factory_time >= 5000) {
			/* Factory Reset */
			set_S2E_Packet_to_factory_value();
			save_S2E_Packet_to_eeprom();

			NVIC_SystemReset();
		}
	}

	/* Serial Trigger Timer Process */	
	if(trigger_flag == 1)
		trigger_time++;

	if(trigger_time >= 500) {
		trigger_flag = 2;
		trigger_time = 0;
	}

	/* Second Process */
	if((mill_cnt % 1000) == 0) {
		Board_LED_Toggle(0);
		mill_cnt = 0;
		sec_cnt++;

		/* DHCP Process */
		if(option->dhcp_use)
			DHCP_time_handler();

		/* DNS Process */
		if(option->dns_use) {
			DNS_time_handler();
		}

		/* Inactive Time Process */
		if(inactive_flag == 1) {
			if((ether_recv_cnt == ether_recv_prev) && (ether_send_cnt == ether_send_prev))
				inactive_time++;
			else {
				ether_send_prev = ether_send_cnt;
				ether_recv_prev = ether_recv_cnt;
				inactive_time = 0;
			}
		}

		if(net->inactivity && (net->inactivity <= inactive_time)) {
			inactive_flag = 2;
			inactive_time = 0;
		}

		/* Connect Password Process */
		if(auth_flag)
			auth_time++;

		/* Minute Process */
		if((sec_cnt % 60) == 0) {
			sec_cnt = 0;

			/* DNS Process */
			if(option->dns_use) {
				run_dns = 1;
			}
		}
	}
}

void timer_Init(void)
{
	uint32_t timerFreq;

	/* Initialize 32-bit timer 0 clock */
	Chip_TIMER_Init(LPC_TIMER32_0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_0);

	/* Enable both timers to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);

	/* Setup 32-bit timer's duration (32-bit match time) */
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, (timerFreq / TICKRATE_HZ1));
	
	/* Setup both timers to restart when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 1);
	
	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER32_0);

	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);

	/* Enable both timer interrupts */
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
}

void delay_cnt(uint32_t count)
{
	volatile uint32_t tmp = count;

	while(tmp--);
}
