
/**
 * @file flash.c
 * @brief Flash Interface Source File.
 * @version 0.1.0
 * @author Sang-sik Kim
 */

/* Includes -----------------------------------------------------*/
#include <string.h>

#include "board.h"
#include "common.h"
#include "flashHandler.h"
#include "TFTP/tftp.h"

/* define -------------------------------------------------------*/

/* typedef ------------------------------------------------------*/

/* Global Variable ----------------------------------------------*/

/* Extern Variable ----------------------------------------------*/

/* Extern Functions ---------------------------------------------*/

/* static function define ---------------------------------------*/


/* Functions ----------------------------------------------------*/
uint32_t read_part_id()
{
	uint32_t command[5], result[4];

	command[0] = IAP_REPID_CMD;
	iap_entry(command, result);

	return result[1];
}

uint32_t addr_to_sector(uint8_t *addr)
{
	return ((uint32_t)addr / SECTOR_SIZE);
}

uint32_t write_flash(uint8_t *addr, uint8_t *data, uint32_t data_len)
{
	uint32_t command[5], result[4];
	uint32_t start_sector, last_sector;

	if((uint32_t)addr & 0xff) {
		//DBG_PRINT(ERROR_DBG, "[%s] flash addr must be 256byte boundary : %x\r\n", __func__, addr);
		return -1;
	}
	
	if((uint32_t)data & 0x3) {
		//DBG_PRINT(ERROR_DBG, "[%s] ram addr must be word boundary : %x\r\n", __func__, data);
		return -1;
	}

	if((data_len != 256) && (data_len != 512) && (data_len != 1024) && (data_len !=4096)) {
		//DBG_PRINT(ERROR_DBG, "[%s] Data Length must be 256/512/1024/4096 byte : %x\r\n", __func__, addr);
		return -1;
	}

	start_sector = addr_to_sector(addr);
	last_sector = start_sector + (data_len / SECTOR_SIZE);
	//DBG_PRINT(DEBUG_DBG, "[%s] prepare start : %d ~ last : %d\r\n", __func__, start_sector, last_sector);

	__disable_irq();

	/* Prepare to write/erase the last sector */
	command[0] = IAP_PREWRRITE_CMD;
	command[1] = start_sector;
	command[2] = last_sector;
	iap_entry(command, result);

	/* Error Check */
	if(result[0] != IAP_CMD_SUCCESS) {
		//DBG_PRINT(ERROR_DBG, "[%s] IAP_PREWRRITE_CMD failed, return code is: %x\r\n", __func__, result[0]); 
		return -1;
	}

	/* Erase the last sector */
	command[0] = IAP_WRISECTOR_CMD;
	command[1] = (uint32_t)addr;
	command[2] = (uint32_t)data;
	command[3] = data_len;
	command[4] = SystemCoreClock / 1000;
	iap_entry(command, result);

	/* Error Check */
	if(result[0] != IAP_CMD_SUCCESS) {
		//DBG_PRINT(ERROR_DBG, "[%s] IAP_WRISECTOR_CMD failed, return code is: %x\r\n", __func__,  result[0]); 
		return -1;
	}
	//DBG_PRINT(DEBUG_DBG, "[%s] flash write addr : %x (data len : %d)\r\n", __func__, addr, data_len);

	__enable_irq();

	return 0;
}

uint32_t erase_flash_sector(uint32_t start_sector, uint32_t last_sector)
{
	uint32_t command[5], result[4];

	__disable_irq();

	/* Prepare to write/erase the last sector */
	command[0] = IAP_PREWRRITE_CMD;
	command[1] = start_sector;
	command[2] = last_sector;
	iap_entry(command, result);

	/* Error Check */
	if(result[0] != IAP_CMD_SUCCESS) {
		//DBG_PRINT(ERROR_DBG, "[%s] IAP_PREWRRITE_CMD failed, return code is: %x\r\n", __func__, result[0]); 
		return -1;
	}

	/* Erase the last sector */
	command[0] = IAP_ERSSECTOR_CMD;
	command[1] = start_sector;
	command[2] = last_sector;
	command[3] = SystemCoreClock / 1000;
	iap_entry(command, result);

	/* Error Check */
	if(result[0] != IAP_CMD_SUCCESS) {
		//DBG_PRINT(ERROR_DBG, "[%s] IAP_WRISECTOR_CMD failed, return code is: %x\r\n", __func__,  result[0]); 
		return -1;
	}
	//DBG_PRINT(DEBUG_DBG, "[%s] erase start sector : %d ~ last sector : %d\r\n", __func__, start_sector, last_sector);

	__enable_irq();

	return 0;
}

/*
 * TFTP dependency
 */
static uint8_t *g_write_point = (uint8_t *)APP_BASE;
void save_data(uint8_t *data, uint32_t data_len, uint16_t block_number)
{
	static uint32_t cnt = 0;

	if(block_number == 1) {
		erase_flash_sector(addr_to_sector((uint8_t *)APP_BASE), MAX_SECTOR_NUM - 1);	
	}

	//DBG_PRINT(INFO_DBG, "#");
	cnt += data_len;

	if(data_len < TFTP_BLK_SIZE) {
		memset(data + data_len, 0xff, TFTP_BLK_SIZE - data_len);
		write_flash(g_write_point, data, TFTP_BLK_SIZE);
		//DBG_PRINT(INFO_DBG, "\r\nwrite flash %d bytes\r\n", cnt);
		g_write_point = (uint8_t *)APP_BASE;
		cnt = 0;
	}
	else {
		write_flash(g_write_point, data, data_len);
		g_write_point += data_len;
	}
}
