#include "board.h"
#include "timerHandler.h"
#include "socket.h"
#include "ConfigData.h"
#include "TFTP/tftp.h"

extern void DHCP_time_handler(void);

static uint32_t mill_cnt = 0;

/**
 * @brief	Handle interrupt from 32-bit timer 0
 * @return	Nothing
 */
void TIMER32_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1))
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);

	mill_cnt++;

	/* 100ms Process */
	if((mill_cnt % 100) == 0) {
		Board_LED_Toggle(0);
		Board_LED_Toggle(1);
	}

	/* Second Process */
	if((mill_cnt % 1000) == 0) {
		mill_cnt = 0;
	
		/* TFTP Process */
		tftp_timeout_handler();
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
