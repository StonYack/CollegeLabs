/*
 * myUDPServer.c
 *
 * Created: 5/7/2020 6:31:46 PM
 *  Author: Ston
 */ 

// Includes 
// Standard Includes 
#include <stdint.h>
#include <stdio.h> 
#include <ctype.h>

// FreeRTOS Includes 
#include "FreeRTOS.h" 
#include "task.h" 
#include "semphr.h"

// FreeRTOS+TCP Include 
#include "FreeRTOS_UDP_IP.h" 
#include "FreeRTOS_Sockets.h"

// My Headers
 #include "myUDPServer.h"
 
 static uint16_t uiMyUsedStackSize = 0;
 
 void StartMyUdpServerTasks( uint16_t usStackSize, UBaseType_t uxPriority ) 
 { 
	 xTaskCreate(MyUDPServerTask, "MyUDPServer", usStackSize, NULL, uxPriority + 1, NULL);	// Creates a task
	 
	 uiMyUsedStackSize = usStackSize; 	// Sets up Stack size once the task is done
}

void MyUDPServerTask(void * pvParameters)
{	// Initialization
	struct freertos_sockaddr xClient, xAddress;
	Socket_t xListeningSocket;
	uint8_t Buffer[ipconfigUDP_MSS];	
	int32_t Returned;
	// Sets up the socket
	xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	configASSERT( xListeningSocket != FREERTOS_INVALID_SOCKET );
	//Sets up socket address
	xAddress.sin_port = FreeRTOS_htons( MY_UDP_PORT_NUMBER );
	xAddress.sin_family = FREERTOS_AF_INET;
	//Binds the socket to the address
	FreeRTOS_bind( xListeningSocket, &xAddress, sizeof( xAddress ) );
	
	while(1)	
	{	// Sets buffer to zero
		memset( Buffer, 0x00, sizeof( Buffer ) );
		//Receives the data from the client
		Returned = FreeRTOS_recvfrom(xListeningSocket, Buffer, sizeof(Buffer), 0, &xClient, sizeof(xClient));
		if(Returned > 0)
		{
			FreeRTOS_printf(("Data was: %s \n", Buffer));	// Outputs the data for the user
		}
	}
}