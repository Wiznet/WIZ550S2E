
#ifndef __UART_H__
#define __UART_H__

#include "ConfigData.h"

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 1024	/* Send */
#define UART_RRB_SIZE 1024	/* Receive */

extern RINGBUFF_T txring, rxring;
extern uint32_t baud_table[11];

void UART_buffer_flush(RINGBUFF_T *buf);
int UART_read(void *data, int bytes);
uint32_t UART_write(void *data, int bytes);
void serial_info_init(struct __serial_info *serial);
void UART_Init(void);
#endif
