/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

#include "common.h"
#include "ConfigData.h"
#include "ConfigMessage.h"
#include "eepromHandler.h"
#include "atcmd.h"
#include "timerHandler.h"
#include "sspHandler.h"
#include "uartHandler.h"
#include "DHCP/dhcp.h"
#include "DNS/dns.h"
#include "S2E.h"
#include "dhcp_cb.h"

#include "mqtt_interface.h"
#include "MQTTClient.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Transmit and receive buffers */
static int vector_in_ram[52] __attribute__((section ("vtable")));

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
uint8_t g_send_buf[WORK_BUF_SIZE] = {'\0',};
uint8_t g_recv_buf[WORK_BUF_SIZE] = {'\0',};

uint8_t run_dns = 1;
uint8_t op_mode;

Network mqttNetwork;
MQTTClient mqttClient;
MQTTPacket_connectData mqttConnectData = MQTTPacket_connectData_initializer;
/*****************************************************************************
 * Private functions
 ****************************************************************************/
static void vector_relocation_ram(void)
{
	int i;
	volatile uint32_t *p = (volatile uint32_t *)APP_BASE;

	/* Vector Table Relocation */
	__disable_irq();

	for(i = 0 ; i < 52 ; i++) {
		vector_in_ram[i] = *(p + i);
	}
	Chip_SYSCTL_Map(REMAP_USER_RAM_MODE);

	__enable_irq();
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/
int main(void) 
{
	S2E_Packet *value = get_S2E_Packet_pointer();
	uint8_t dns_server_ip[4];

	vector_relocation_ram();
	SystemCoreClockUpdate();
	Board_Init();

	Board_LED_Set(0, false);
	Board_LED_Set(1, false);

	load_S2E_Packet_from_eeprom();
	//set_S2E_Packet_to_factory_value();
	//save_S2E_Packet_to_eeprom();

	UART_Init();
	SPI_Init();
	W5500_Init();
	timer_Init();
	
	Mac_Conf();
	DHCP_init(SOCK_DHCP, g_send_buf);
	reg_dhcp_cbfunc(w5500_dhcp_assign, w5500_dhcp_assign, w5500_dhcp_conflict);

	/* Initialize Network Information */
	if(value->options.dhcp_use) {		// DHCP
		uint32_t ret;
		uint8_t dhcp_retry = 0;

		//printf("Start DHCP...\r\n");
		while(1) {
			ret = DHCP_run();

			if(ret == DHCP_IP_LEASED)
				break;
			else if(ret == DHCP_FAILED)
				dhcp_retry++;

			if(dhcp_retry > 3) {
				Net_Conf();
				break;
			}
			do_udp_config(SOCK_CONFIG);
		}
	} else 								// Static
		Net_Conf();	


	DNS_init(SOCK_DNS, g_send_buf);
	if(value->options.dns_use) {
		uint8_t dns_retry = 0;

		memcpy(dns_server_ip, value->options.dns_server_ip, sizeof(dns_server_ip));

		while(1) {
			if(DNS_run(dns_server_ip, (uint8_t *)value->options.dns_domain_name, value->network_info[0].remote_ip) == 1)
				break;
			else
				dns_retry++;

			if(dns_retry > 3) {
				break;
			}

			do_udp_config(SOCK_CONFIG);

			if(value->options.dhcp_use)
				DHCP_run();
		}
	}

	NewNetwork(&mqttNetwork, SOCK_MQTT);
	MQTTClientInit(&mqttClient, &mqttNetwork, 1000, g_send_buf, WORK_BUF_SIZE, g_recv_buf, WORK_BUF_SIZE);

	atc_init();

	op_mode = OP_DATA;
	while (1) {
		if(op_mode == OP_COMMAND) {			// Command Mode
			atc_run();
			sockwatch_run();
		} else {							// DATA Mode
			s2e_run(SOCK_DATA);
		}
		
		do_udp_config(SOCK_CONFIG);

		if(value->options.dhcp_use)
			DHCP_run();

		if(value->options.dns_use && run_dns == 1) {
			memcpy(dns_server_ip, value->options.dns_server_ip, sizeof(dns_server_ip));

			if(DNS_run(dns_server_ip, (uint8_t *)value->options.dns_domain_name, value->network_info[0].remote_ip) == 1) {
				run_dns = 0;
			}
		}
	}

	/* DeInitialize SPI peripheral */
	Chip_SSP_DeInit(LPC_SSP0);

    return 0 ;
}
