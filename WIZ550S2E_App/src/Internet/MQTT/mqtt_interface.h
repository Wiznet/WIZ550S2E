//*****************************************************************************
//! \file mqtt_interface.h
//! \brief Paho MQTT to WIZnet Chip interface Header file.
//! \details The process of porting an interface to use paho MQTT.
//! \version 1.0.0
//! \date 2016/12/06
//! \par  Revision history
//!       <2016/12/06> 1st Release
//!
//! \author Peter Bang & Justin Kim
//! \copyright
//!
//! Copyright (c)  2016, WIZnet Co., LTD.
//! All rights reserved.
//!
//! Redistribution and use in source and binary forms, with or without
//! modification, are permitted provided that the following conditions
//! are met:
//!
//!     * Redistributions of source code must retain the above copyright
//! notice, this list of conditions and the following disclaimer.
//!     * Redistributions in binary form must reproduce the above copyright
//! notice, this list of conditions and the following disclaimer in the
//! documentation and/or other materials provided with the distribution.
//!     * Neither the name of the <ORGANIZATION> nor the names of its
//! contributors may be used to endorse or promote products derived
//! from this software without specific prior written permission.
//!
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//! ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//! LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//! CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//! SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//! INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//! CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//! ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
//! THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************


#ifndef __MQTT_INTERFACE_H_
#define __MQTT_INTERFACE_H_

/*
 * @brief MQTT MilliTimer handler
 * @note MUST BE register to your system 1m Tick timer handler
 */
void MilliTimer_Handler(void);

/*
 * @brief Timer structure
 */
typedef struct Timer Timer;
struct Timer {
	unsigned long systick_period;
	unsigned long end_time;
};

/*
 * @brief Network structure
 */
typedef struct Network Network;
struct Network
{
	int my_socket;
	int (*mqttread) (Network*, unsigned char*, int, int);
	int (*mqttwrite) (Network*, unsigned char*, int, int);
	void (*disconnect) (Network*);
};

/*
 * @brief Timer function
 */
void TimerInit(Timer*);
char TimerIsExpired(Timer*);
void TimerCountdownMS(Timer*, unsigned int);
void TimerCountdown(Timer*, unsigned int);
int TimerLeftMS(Timer*);

/*
 * @brief Network interface porting
 */
int w5x00_read(Network*, unsigned char*, int);
int w5x00_write(Network*, unsigned char*, int);
void w5x00_disconnect(Network*);
void NewNetwork(Network*, int);
int InitNetwork(Network*, unsigned int, unsigned char);
int ConnectNetwork(Network*, unsigned char*, unsigned int);

#endif //__MQTT_INTERFACE_H_
