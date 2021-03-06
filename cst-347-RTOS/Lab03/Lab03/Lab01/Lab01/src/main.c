//system includes
#include <asf.h>

// FreeRTOS Includes
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <queue.h>
#include <semphr.h>


// My Includes
#include "Uartdrv.h"
#include "mytasks.h"
#include "OITExpansionBoard.h"
#include "ledDriver.h"
#include "swDriver.h"

// Defines
#if( BOARD == SAM4E_XPLAINED_PRO )		// Used to place the heap
#define mainHEAP_START			_estack
#define mainRAM_LENGTH			0x00020000	/* 128 KB of internal SRAM. */
#define mainRAM_START			0x20000000	/* at 0x20000000 */
/* The SAM4E Xplained board has 2 banks of external SRAM, each one 512KB. */
#define mainEXTERNAL_SRAM_SIZE	( 512ul * 1024ul )
#else
#error Define memory regions here.
#endif /* SAM4E_XPLAINED_PRO */

// Function Prototypes
static void prvMiscInitialisation( void );
static void prvInitialiseHeap( void );
void vApplicationMallocFailedHook( void );
//void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );

// Used to place the heap
extern char _estack;

TaskHandle_t xMain1 = NULL;
TaskHandle_t xMain2 = NULL;
TaskHandle_t xMain3 = NULL;
QueueHandle_t xQueue1;// = xQueueCreate(4, sizeof(char[50]));
QueueHandle_t xQueue2;// = xQueueCreate(4, sizeof(char[50]));
QueueHandle_t xQueue3;// = xQueueCreate(4, sizeof(char[50]));
QueueHandle_t xUARTQueue;



int main (void)
{
	// Initialize The Board
	prvMiscInitialisation();
	
	xQueue1 = xQueueCreate(5, sizeof(enum ACommand));
	xQueue2 = xQueueCreate(5, sizeof(enum ACommand));
	xQueue3 = xQueueCreate(5, sizeof(enum ACommand));
	xUARTQueue = xQueueCreate(5, sizeof(char[50]));	
	
	xTaskCreate(ledTask,				// Function Called by task
				"LED Task",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)1,						// Any Parameters Passed to Task
				1,							// Task Priority
				NULL);						// Place to store Task Handle
	
	xTaskCreate(ledTask,				// Function Called by task
				"LED Task",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)2,						// Any Parameters Passed to Task
				1,							// Task Priority
				NULL);						// Place to store Task Handle

	xTaskCreate(ledTask,				// Function Called by task
				"LED Task",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)3,						// Any Parameters Passed to Task
				1,							// Task Priority
				NULL);						// Place to store Task Handle
	
	xTaskCreate(taskSystemControl,				// Function Called by task
				"LED Control",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)1,						// Any Parameters Passed to Task
				1,							// Task Priority
				&xMain1);						// Place to store Task Handle
	
	xTaskCreate(taskSystemControl,				// Function Called by task
				"LED Control",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)2,						// Any Parameters Passed to Task
				1,							// Task Priority
				&xMain2);						// Place to store Task Handle
	
	xTaskCreate(taskSystemControl,				// Function Called by task
				"LED Control",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				(void*)3,						// Any Parameters Passed to Task
				1,							// Task Priority
				&xMain3);						// Place to store Task Handle
				
	xTaskCreate(UARTTask,				// Function Called by task
				"UART Control",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				NULL,						// Any Parameters Passed to Task
				1,							// Task Priority
				NULL);						// Place to store Task Handle
				
	xTaskCreate(HeartbeatTask,				// Function Called by task
				"Heartbeat Task",			// Task Name
				configMINIMAL_STACK_SIZE,	// Task Stack Size
				NULL,						// Any Parameters Passed to Task
				1,							// Task Priority
				NULL);						// Place to store Task Handle
				
	
				
	vTaskSuspend(xMain2);
	vTaskSuspend(xMain3);
				
	// Start The Scheduler
	vTaskStartScheduler();
	while(true)
	{ }
}

static void prvInitialiseHeap( )
{
	uint32_t ulHeapSize;
	uint8_t *pucHeapStart, *pucHeapEnd;

	pucHeapStart = ( uint8_t * ) ( ( ( ( uint32_t ) &mainHEAP_START ) + 7 ) & ~0x07ul );
	pucHeapEnd = ( uint8_t * ) ( mainRAM_START + mainRAM_LENGTH );
	ulHeapSize = ( uint32_t ) ( ( uint32_t ) pucHeapEnd - ( uint32_t ) &mainHEAP_START );
	ulHeapSize &= ~0x07ul;
	ulHeapSize -= 1024;
	HeapRegion_t xHeapRegions[] = {
		{ ( unsigned char *) pucHeapStart, ulHeapSize },
		{ ( unsigned char *) SRAM_BASE_ADDRESS, mainEXTERNAL_SRAM_SIZE },
		{ ( unsigned char *) SRAM_BASE_ADDRESS_2ND, mainEXTERNAL_SRAM_SIZE },
		{ NULL, 0 }
	};

	vPortDefineHeapRegions( xHeapRegions );
}
static void prvMiscInitialisation( void )
{
	/* Initialize the SAM system */
	sysclk_init();
	board_init();
	prvInitialiseHeap();
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	OITExpansionBoardInit();
	initializeSWDriver();
	initializeLEDDriver();
	initUART(EDBG_UART);
}

void vAssertCalled( const char *pcFile, uint32_t ulLine )
{
	volatile uint32_t ulBlockVariable = 0UL;

	/* Setting ulBlockVariable to a non-zero value in the debugger will allow
	this function to be exited. */
	taskDISABLE_INTERRUPTS();
	{
		while( ulBlockVariable == 0UL )
		{
			__asm volatile( "NOP" );
		}
	}
	taskENABLE_INTERRUPTS();
}

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.? pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.? The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	vAssertCalled( __FILE__, __LINE__ );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.? This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();

	for( ;; );
}