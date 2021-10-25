/*
 * myEchoServer.c
 *
 * Created: 4/30/2020 6:30:51 PM
 *  Author: Ston
 */ 

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "myEchoServer.h"

/* Stores the stack size passed into vStartSimpleTCPServerTasks() so it can be
reused when the server listening task creates tasks to handle connections. */
static uint16_t usUsedStackSize = 0;

void StartMyEchoTCPServerTasks( uint16_t usStackSize, UBaseType_t uxPriority )
{
	/* Create the TCP echo server. */
	xTaskCreate( MyEchoServerListeningTask, "ServerListener", usStackSize, NULL, uxPriority + 1, NULL );

	usUsedStackSize = usStackSize;
}

void MyEchoServerListeningTask( void *pvParameters )
{
	struct freertos_sockaddr xClient, xBindAddress;
	Socket_t xListeningSocket, xConnectedSocket;
	socklen_t xSize = sizeof( xClient );
	static const TickType_t xReceiveTimeOut = portMAX_DELAY;
	const BaseType_t xBacklog = 20;

	/* Just to prevent compiler warnings. */
	( void ) pvParameters;

	/* Attempt to open the socket. */
	xListeningSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP );
	configASSERT( xListeningSocket != FREERTOS_INVALID_SOCKET );

	/* Set a time out so accept() will just wait for a connection. */
	FreeRTOS_setsockopt( xListeningSocket, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );

	/* Bind the socket to the port that the client task will send to, then
	listen for incoming connections. */
	xBindAddress.sin_port = myTCPechoPORT_NUMBER;
	xBindAddress.sin_port = FreeRTOS_htons( xBindAddress.sin_port );
	FreeRTOS_bind( xListeningSocket, &xBindAddress, sizeof( xBindAddress ) );
	FreeRTOS_listen( xListeningSocket, xBacklog );

	for( ;; )
	{
		/* Wait for a client to connect. */
		xConnectedSocket = FreeRTOS_accept( xListeningSocket, &xClient, &xSize );
		configASSERT( xConnectedSocket != FREERTOS_INVALID_SOCKET );

		/* Spawn a task to handle the connection. */
		xTaskCreate( MyEchoServerInstance, "EchoServer", usUsedStackSize, ( void * ) xConnectedSocket, tskIDLE_PRIORITY, NULL );
	}
}

void MyEchoServerInstance( void *pvParameters )
{
	int32_t lBytes, lSent, lTotalSent;
	Socket_t xConnectedSocket;
	static const TickType_t xReceiveTimeOut = pdMS_TO_TICKS( 5000 );
	static const TickType_t xSendTimeOut = pdMS_TO_TICKS( 5000 );
	TickType_t xTimeOnShutdown;
	uint8_t *pucRxBuffer;

	xConnectedSocket = ( Socket_t ) pvParameters;

	/* Attempt to create the buffer used to receive the string to be echoed
	back.  This could be avoided using a zero copy interface that just returned
	the same buffer. */
	pucRxBuffer = ( uint8_t * ) pvPortMalloc( ipconfigTCP_MSS );

	if( pucRxBuffer != NULL )
	{
		FreeRTOS_setsockopt( xConnectedSocket, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof( xReceiveTimeOut ) );
		FreeRTOS_setsockopt( xConnectedSocket, 0, FREERTOS_SO_SNDTIMEO, &xSendTimeOut, sizeof( xReceiveTimeOut ) );

		for( ;; )
		{
			/* Zero out the receive array so there is NULL at the end of the string
			when it is printed out. */
			memset( pucRxBuffer, 0x00, ipconfigTCP_MSS );

			/* Receive data on the socket. */
			lBytes = FreeRTOS_recv( xConnectedSocket, pucRxBuffer, ipconfigTCP_MSS, 0 );
			
			if(strcmp(pucRxBuffer,"Ston\r\n") == 0)		// checks to see if the input is Ston
			{
				strcpy(pucRxBuffer,"Hello Ston\r\n");	// if it is, copies the greeting message to the buffer
				lBytes = strlen("Hello Ston\r\n");		// Sets the correct number of bytes in the buffer
			}
			else if(strcmp(pucRxBuffer,".\r\n") == 0)	// if the input is "." end the server
			{
				break;
			}
			else										//  else set all of the input to uppercase (if applicable)
			{
				for (int i = 0; i < (strlen(pucRxBuffer) + 1); i++)
				{
					pucRxBuffer[i] = toupper(pucRxBuffer[i]);
				}
			}
			
			/* If data was received, echo it back. */
			if( lBytes >= 0 )
			{
				lSent = 0;
				lTotalSent = 0;
				
				/* Call send() until all the data has been sent. */
				while( ( lSent >= 0 ) && ( lTotalSent < lBytes ) )
				{
					lSent = FreeRTOS_send( xConnectedSocket, pucRxBuffer, lBytes - lTotalSent, 0 );
					lTotalSent += lSent;
				}

				if( lSent < 0 )
				{
					/* Socket closed? */
					break;
				}
			}
			else
			{
				/* Socket closed? */
				break;
			}
		}
	}

	/* Initiate a shutdown in case it has not already been initiated. */
	FreeRTOS_shutdown( xConnectedSocket, FREERTOS_SHUT_RDWR );

	/* Wait for the shutdown to take effect, indicated by FreeRTOS_recv()
	returning an error. */
	xTimeOnShutdown = xTaskGetTickCount();
	do
	{
		if( FreeRTOS_recv( xConnectedSocket, pucRxBuffer, ipconfigTCP_MSS, 0 ) < 0 )
		{
			break;
		}
	} while( ( xTaskGetTickCount() - xTimeOnShutdown ) < myTCPechoSHUTDOWN_DELAY );

	/* Finished with the socket, buffer, the task. */
	vPortFree( pucRxBuffer );
	FreeRTOS_closesocket( xConnectedSocket );

	vTaskDelete( NULL );
	
}