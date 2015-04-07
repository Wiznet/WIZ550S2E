
#ifndef __S2E_H__
#define __S2E_H__

//#define __TRIG_DEBUG__
#define TRIG_STATE_NONE			0
#define TRIG_STATE_READY		1
#define TRIG_STATE_1			2
#define TRIG_STATE_2			3
#define TRIG_STATE_3			4

extern uint32_t uart_send_cnt;
extern uint32_t uart_recv_cnt;
extern uint32_t ether_send_cnt;
extern uint32_t ether_recv_cnt;

void s2e_run(uint8_t sock);

#endif
