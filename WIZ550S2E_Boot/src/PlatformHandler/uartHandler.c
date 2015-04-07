/**
 * @file main.c
 * @brief S2E Bootloader Main Source File.
 * @version 0.1.0
 * @author Sang-sik Kim
 */

/* Includes -----------------------------------------------------*/
#include "board.h"
#include "uarthandler.h"

/* define -------------------------------------------------------*/
#ifdef UART_IRQHandler
#undef UART_IRQHandler
#endif

/* typedef ------------------------------------------------------*/

/* Extern Variable ----------------------------------------------*/

/* Extern Functions ---------------------------------------------*/

/* Global Variable ----------------------------------------------*/
RINGBUFF_T txring, rxring;
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];
uint32_t baud_table[11] = {
	baud_300,
	baud_600,
	baud_1200,
	baud_2400,
	baud_4800,
	baud_9600,
	baud_19200,
	baud_38400,
	baud_57600,
	baud_115200, 
	baud_230400,
};

/* static function define ---------------------------------------*/


/* Functions ----------------------------------------------------*/
/**
 * @brief	UART interrupt handler using ring buffers
 * @return	Nothing
 */
void UART_IRQHandler(void)
{
	/* Want to handle any errors? Do it here. */

	/* Use default ring buffer handler. Override this with your own
	   code if you need more capability. */
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}

void serial_info_init(struct __serial_info *serial)
{
	uint32_t config = 0;

	/* Set Baud Rate */
	Chip_UART_SetBaud(LPC_USART, serial->baud_rate);

	/* Set Data Bits */
	switch(serial->data_bits) {
		case word_len5:
			config |= UART_LCR_WLEN5;
			break;
		case word_len6:
			config |= UART_LCR_WLEN6;
			break;
		case word_len7:
			config |= UART_LCR_WLEN7;
			break;
		case word_len8:
			config |= UART_LCR_WLEN8;
			break;
		default:
			config |= UART_LCR_WLEN8;
			serial->data_bits = word_len8;
			break;
	}

	/* Set Stop Bits */
	switch(serial->stop_bits) {
		case stop_bit1:
			config |= UART_LCR_SBS_1BIT;
			break;
		case stop_bit2:
			config |= UART_LCR_SBS_2BIT;
			break;
		default:
			config |= UART_LCR_SBS_1BIT;
			serial->stop_bits = stop_bit1;
			break;
	}

	/* Set Parity Bits */
	switch(serial->parity) {
		case parity_none:
			config |= UART_LCR_PARITY_DIS;
			break;
		case parity_odd:
			config |= (UART_LCR_PARITY_EN | UART_LCR_PARITY_ODD);
			break;
		case parity_even:
			config |= (UART_LCR_PARITY_EN | UART_LCR_PARITY_EVEN);
			break;
		default:
			config |= UART_LCR_PARITY_DIS;
			serial->parity = parity_none;
			break;
	}
	Chip_UART_ConfigData(LPC_USART, config);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 7, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));		// CTS
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));	// RTS 
	switch(serial->flow_control) {
		case flow_none:
			Chip_UART_ClearModemControl(LPC_USART, UART_MCR_RTS_CTRL | UART_MCR_AUTO_RTS_EN | UART_MCR_AUTO_CTS_EN);
			Chip_UART_ClearRS485Flags(LPC_USART, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_NMM_EN | UART_RS485CTRL_OINV_1);
			break;
		case flow_rts_cts:
			Chip_UART_ClearRS485Flags(LPC_USART, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_NMM_EN | UART_RS485CTRL_OINV_1);
			Chip_UART_SetModemControl(LPC_USART, UART_MCR_AUTO_RTS_EN | UART_MCR_AUTO_CTS_EN);
			break;
		case flow_rs422:
			Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_FUNC0 | IOCON_MODE_INACT | IOCON_DIGMODE_EN));	// RTS 
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 17, 0);
			Chip_UART_ClearModemControl(LPC_USART, UART_MCR_RTS_CTRL | UART_MCR_AUTO_RTS_EN | UART_MCR_AUTO_CTS_EN);
			Chip_UART_ClearRS485Flags(LPC_USART, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_NMM_EN | UART_RS485CTRL_OINV_1);
			break;
		case flow_rs485:
			Chip_UART_ClearModemControl(LPC_USART, UART_MCR_RTS_CTRL | UART_MCR_AUTO_RTS_EN | UART_MCR_AUTO_CTS_EN);
			Chip_UART_SetRS485Flags(LPC_USART, UART_RS485CTRL_DCTRL_EN | UART_RS485CTRL_NMM_EN | UART_RS485CTRL_OINV_1);
			break;
		default:
			break;
	}
}

int UART_read(void *data, int bytes)
{
	return Chip_UART_ReadRB(LPC_USART, &rxring, data, bytes);
}

uint32_t UART_write(void *data, int bytes)
{
	return Chip_UART_SendRB(LPC_USART, &txring, data, bytes);
}

void UART_Init(void)
{
	struct __serial_info *serial = (struct __serial_info *)(get_S2E_Packet_pointer()->serial_info);

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_USART);

	serial_info_init(serial);
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV3));
	Chip_UART_TXEnable(LPC_USART);

	/* Before using the ring buffers, initialize them using the ring
	   buffer init function */
	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(UART0_IRQn, 1);
	NVIC_EnableIRQ(UART0_IRQn);
}
