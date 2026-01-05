/*
 * bl_jump.c
 *
 */

#include "main.h"
#include "bl_jump.h"
#include "flash_layout.h"
#include "app_header.h"

#define APP_MAGIC 0xABCDEFAB

typedef void (*pFunction)(void);

void JumpToApplication(void)
{
	uint32_t appStack;
	uint32_t appResetHandler;
	pFunction appEntry;

	/*The application expects a clean startup state
	 * Before making the jump
	 *
	 * 1.The bootloader disables all active interrupts
	 * this ensures that no bootloader interrupts will trigger during application run
	 *
	 * 2.The bootloader stops the SysTick timer completely
	 * SysTick may have been configured by the bootloader*/

	/*1.Disable interrupts*/
	__disable_irq();

	/*2.Stop SysTick*/
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	/*First 2 elements of vector table according to cortex-m
	 * stack pointer, reset handler
	 *
	 * These 2 are set before the C runtime initialization in the assembly startup file usually
	 * 1.Set the initial SP
	 * 2.Set the initial PC == Reset_Handler
	 * */

	/*Read the application stack pointer*/
	appStack = *(volatile uint32_t*)APP_START_ADDR;

	/*Read reset handler address*/
	appResetHandler = *(volatile uint32_t*)(APP_START_ADDR + 4);
	appEntry = (pFunction)appResetHandler;

	/*1.Set the initial SP
	 * Set the main stack pointer*/
	__set_MSP(appStack);

	/*2.Set the initial PC == Reset_Handler
	 * Jump to application reset handler*/
	appEntry();
}

int bootloader_is_app_valid(void)
{
	uint32_t HEADER_ADDR = APP_HEADER_START;
	const app_header_t *app_header = (const app_header_t *)HEADER_ADDR;

	/*1.Check magic number*/
	if (app_header->magic != APP_MAGIC)
	{
		return 1;
	}

	/*2.Reset handler sanity check*/
	reset_handler = *(uint32_t*)(APP_START_ADDR + 4);
	if ((reset_handler & 0xFF000000) != 0x08000000)
	{
		return 4;
	}

	return 0; // VALID
}
