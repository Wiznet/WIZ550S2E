
#ifndef __FLASH_H__
#define __FLASH_H__

#define APP_BASE			(0x6000)

#define SECTOR_SIZE			(4096)
#define MAX_SECTOR_NUM		(24)
#define FLASH_BASE_ADDR		(0)
#define TICKRATE_HZ 		(10)    /* 10 ticks per second */

uint32_t addr_to_sector(uint8_t *addr);
uint32_t read_part_id();
uint32_t write_flash(uint8_t *addr, uint8_t *data, uint32_t data_len);
uint32_t erase_flash_sector(uint32_t start_sector, uint32_t last_sector);

#endif
