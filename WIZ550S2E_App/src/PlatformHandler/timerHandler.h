
#ifndef __TIMER_H__
#define __TIMER_H__

#define TICKRATE_HZ1    (1000)	/* 1000 ticks per second */

extern volatile uint8_t nagle_flag;
extern volatile uint32_t nagle_time;
extern volatile uint32_t uart_recv_count;

extern uint8_t reconn_flag;	
extern uint32_t reconn_time;

extern uint8_t inactive_flag;
extern uint32_t inactive_time;
extern uint32_t ether_send_prev;
extern uint32_t ether_recv_prev;
extern uint8_t keepsend_flag;
extern uint32_t keepsend_time;

extern uint8_t trigger_flag;
extern uint32_t trigger_time;

extern uint8_t factory_flag;
extern uint32_t factory_time;

extern uint8_t auth_flag;
extern uint32_t auth_time;

void timer_Init(void);
void delay_cnt(uint32_t count);

#endif
