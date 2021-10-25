/*************************************************************
 * Author:          Troy Scevers
 * Filename:		myCommands.c
 * Date Created:	4/16/2018
 * Modifications:   
 *
 * Lab/Assignment:	UDP CLI Commands
 *
 * Overview:
 *  
 ************************************************************/
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS+CLI includes. */
#include "FreeRTOS_CLI.h"

/* FreeRTOS+TCP includes, just to make the stats available to the CLI
   commands. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/* My Includes */
#include "myCommands.h"
#include "myUDPServer.h"
#include "myGlobals.h"

// Command Definitions
static const CLI_Command_Definition_t xNetUp =
{
    "net-status", /* The command string to type. */
    "\r\nnet-status:\r\n Calls FreeRTOS_isNetworkUp()\r\n",
    prvNetStatusCommand, /* The function to run. */
    0 /* No parameters are expected. */
};
static const CLI_Command_Definition_t xDNSLookup =
{
    "dns-lookup",
    "\r\ndns-lookup <host-name>:\r\n For Example dns-lookup www.example.com\r\n",
    prvDnsLookupCommand,
    1
};

static const CLI_Command_Definition_t xGetMAC =
{
	"MAC-get",
	"\r\nMAC-get:\r\n Calls FreeRTOS_GetMACAddress()\r\n",
	prvGetMacCommand,
	0
};

static const CLI_Command_Definition_t xGetIP =
{
	"IP-get",
	"\r\nIP-get:\r\n Calls FreeRTOS_GetIPAddress()\r\n",
	prvGetIPAddress,
	0
};

static const CLI_Command_Definition_t xGetGateway =
{
	"Gateway-get",
	"\r\nGateway-get:\r\n Calls FreeRTOS_GetGatewayAddress()\r\n",
	prvGetGatewayAddress,
	0
};

static const CLI_Command_Definition_t xGetDNS =
{
	"DNS-get",
	"\r\nDNS-get:\r\n Calls FreeRTOS_GetDNSServerAddress()\r\n",
	prvGetDNSServerAddress,
	0
};

static const CLI_Command_Definition_t xGetNetmask =
{
	"Netmask-get",
	"\r\nNetmask-get:\r\n Calls FreeRTOS_GetNetmask()\r\n",
	prvGetNetmask,
	0
};

static const CLI_Command_Definition_t xOutputARP =
{
	"Output-ARP-Request",
	"\r\nOutput-ARP-Request <Device-IP-Address>:\r\n Calls FreeRTOS_OutputARPRequest()\r\n",
	prvOutputARPRequest,
	1
};

static const CLI_Command_Definition_t xSendUDPMessage =	// Sets up UDP message command
{
	"udp-send",
	"\r\nudp-send <host> <message> \r\nExample: udp-send \"Hello\"\r\n",
	prvSendUDPMessageCommand,
	1
};

static const CLI_Command_Definition_t xPrintName =
{ 
	"print-name", 
	"\r\nprint-name\r\nExample: print-name\r\n",
	 prvPrintNameCommand, 
	 0 
};


//Functions
void vRegisterMyCommands(void)
{
    FreeRTOS_CLIRegisterCommand( &xNetUp );
    FreeRTOS_CLIRegisterCommand( &xDNSLookup );
	FreeRTOS_CLIRegisterCommand( &xGetMAC );
	FreeRTOS_CLIRegisterCommand( &xGetIP );
	FreeRTOS_CLIRegisterCommand( &xGetGateway );
	FreeRTOS_CLIRegisterCommand( &xGetDNS );
	FreeRTOS_CLIRegisterCommand( &xGetNetmask );
	FreeRTOS_CLIRegisterCommand( &xOutputARP );
	FreeRTOS_CLIRegisterCommand( &xSendUDPMessage );	// registers command
	FreeRTOS_CLIRegisterCommand( &xPrintName );
}

BaseType_t prvNetStatusCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    BaseType_t xReturnValue = pdFALSE;

    // Get rid of compiler warnings about unused variables
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    // Start with an empty string
    pcWriteBuffer[0] = '\0';

    xReturnValue = FreeRTOS_IsNetworkUp();

    if(xReturnValue == pdFALSE)
    {
        sprintf(pcWriteBuffer, "%s", "The Network is Currently down\r\n");
    }
    else
    {
        sprintf(pcWriteBuffer, "%s", "The Network is Currently Up\r\n");
    }

    return pdFALSE;
}

BaseType_t prvDnsLookupCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
    uint32_t ulIPAddress;
    int8_t cBuffer[ 16 ];
    char * pcParameter;
    BaseType_t lParameterStringLength;
    
    // Get rid of compiler warnings about unused variables
    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    // Start with an empty string.
    pcWriteBuffer[0] = '\0';

    // Get the parameter
    pcParameter = (char *) FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParameterStringLength);

    if( pcParameter != NULL )
    {
        // Add Null Terminator to the string
        pcParameter[lParameterStringLength] = '\0';

        // Call get host by name to do dns lookup
        ulIPAddress = FreeRTOS_gethostbyname(pcParameter);

        if(ulIPAddress != 0)
        {
            FreeRTOS_inet_ntoa(ulIPAddress, (char *) cBuffer);

            sprintf(pcWriteBuffer, "%s is at IP Address %s\r\n", pcParameter, cBuffer);
        }
        else
        {
            sprintf(pcWriteBuffer, "%s", "DNS Lookup Failed\r\n");
        }
    }
    else
    {
        sprintf(pcWriteBuffer, "%s", "DNS Failed: Could not retrieve parameter\r\n");
    }

    return pdFALSE;
}

BaseType_t prvGetMacCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	 
	 // Get rid of compiler warnings about unused variables
	 ( void ) pcCommandString;
	 ( void ) xWriteBufferLen;
     configASSERT( pcWriteBuffer );	
	 
	 pcWriteBuffer[0] = '\0';	//Initialize buffer
	 
	 const uint8_t * MAC = FreeRTOS_GetMACAddress();	// Receives MAC address
	 
	 if(MAC != NULL)	// if MAC = pdFALSE something went wrong
	 {
		 sprintf(pcWriteBuffer, "MAC Address: %02x:%02x:%02x:%02x:%02x:%02x", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);	// outputs MAC address
	 }
	 else sprintf(pcWriteBuffer,"Receiving MAC address failed");
	
	return pdFALSE;	
}

BaseType_t prvGetIPAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
     // Get rid of compiler warnings about unused variables
	 ( void ) pcCommandString;
	 ( void ) xWriteBufferLen;
     configASSERT( pcWriteBuffer );	
	 
	 pcWriteBuffer[0] = '\0';	//Initialize buffer
	
	uint8_t IP[16] = {'\0'};
	uint32_t  address = FreeRTOS_GetIPAddress(); // Receives IP address
	FreeRTOS_inet_ntoa(address,(char *) IP);	//Translate to correct format
	
	if(IP != NULL)	// if IP = pdFALSE something went wrong
	{
		sprintf(pcWriteBuffer,"IP Address: %s", IP);	// Outputs IP address
	}
	else sprintf(pcWriteBuffer,"Receiving IP address failed");
	
	return pdFALSE;
}

BaseType_t prvGetGatewayAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	 // Get rid of compiler warnings about unused variables
	 ( void ) pcCommandString;
	 ( void ) xWriteBufferLen;
     configASSERT( pcWriteBuffer );	
	 
	 pcWriteBuffer[0] = '\0';	//Initialize buffer
	
	uint8_t Gateway[16] = {'\0'};	// Initialization of the value to receive Gateway
	uint32_t address = FreeRTOS_GetGatewayAddress();	// Receives Gateway address
	FreeRTOS_inet_ntoa(address, (char*) Gateway);	//Translate to correct format
	
	if(Gateway != NULL)	// if Gateway = pdFALSE something went wrong
	{
		sprintf(pcWriteBuffer, "Gateway Address: %s", Gateway);	// Outputs Gateway address
	}
	else sprintf(pcWriteBuffer,"Receiving Gateway address failed");
	
	return pdFALSE;
}

BaseType_t prvGetDNSServerAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	// Get rid of compiler warnings about unused variables
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
		
	pcWriteBuffer[0] = '\0';	//Initialize buffer
	
	uint8_t DNS[16] = {'\0'};	// Initialization of the value to receive DNS Server
	uint32_t address = FreeRTOS_GetDNSServerAddress();	// Receives DNS Server address
	FreeRTOS_inet_ntoa(address, (char*) DNS);	//Translate to correct format
	
	if(DNS != NULL)	// if DNS Server = pdFALSE something went wrong
	{
		sprintf(pcWriteBuffer, "DNS Server Address: %s", DNS);	// Outputs DNS Server address
	}
	else sprintf(pcWriteBuffer,"Receiving DNS Server address failed");
	
	return pdFALSE;
}

BaseType_t prvGetNetmask(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	// Get rid of compiler warnings about unused variables
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	pcWriteBuffer[0] = '\0';	//Initialize buffer
	
	uint8_t Netmask[16] = {'\0'};	// Initialization of the value to receive Netmask
	uint32_t address = FreeRTOS_GetNetmask();	// Receives Netmask address
	FreeRTOS_inet_ntoa(address, (char*) Netmask);	//Translate to correct format
	
	if(Netmask != NULL)	// if Netmask = pdFALSE something went wrong
	{
		sprintf(pcWriteBuffer, "Netmask: %s", Netmask);	// Outputs Netmask address
	}
	else printf(pcWriteBuffer,"Receiving Netmask Failed");
	
	return pdFALSE;
}

BaseType_t prvOutputARPRequest(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	// Get rid of compiler warnings about unused variables
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	pcWriteBuffer[0] = '\0';	//Initialize buffer
	
	char * pcParameter = NULL;
	BaseType_t lParameterStringLength;
	
	pcParameter = (char *) FreeRTOS_CLIGetParameter(pcCommandString, 1, &lParameterStringLength);
	
	if(pcParameter != NULL)
	{
		uint32_t address = FreeRTOS_inet_addr(pcParameter);// Receives IP address
		
		if(address != 0)// if IP = pdFALSE something went wrong
		{
			FreeRTOS_OutputARPRequest(address);	//	Sends ARP request
			sprintf(pcWriteBuffer,"ARP Request Sent!");
		}
		else sprintf(pcWriteBuffer,"Receiving IP address failed");
	}
	
	return pdFALSE;
}

BaseType_t prvSendUDPMessageCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString)
{
	// Get rid of compiler warnings about unused variables
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	// Initializes the variables
	struct freertos_sockaddr xSendUDPMessageAddress;
	struct freertos_sockaddr xLocalAddress; 
	
	Socket_t xSocket;
	int32_t Returned;
	BaseType_t StringLen1;
	BaseType_t StringLen2;
	
	// Gets the Parameters
	uint8_t * Buffer = (char *) FreeRTOS_CLIGetParameter(pcCommandString, 1, &StringLen2);
	Buffer [StringLen2] = 0x00;
	
	// Sets the socket
	xSocket = FreeRTOS_socket( FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP );
	//Sets up the socket address
	xSendUDPMessageAddress.sin_addr = FreeRTOS_inet_addr(UDP_IP); // Sets the IP
	xSendUDPMessageAddress.sin_port = FreeRTOS_htons(atoi(UDP_Port)); // Converts the port to legible value to be used then sets port
	xSendUDPMessageAddress.sin_family = FREERTOS_AF_INET;
	
	xLocalAddress.sin_port = FreeRTOS_htons(atoi(UDP_Port));	// uses the set port on the local address
	
	configASSERT( xSocket  != FREERTOS_INVALID_SOCKET);   // checks to see if the socket was created properly
	
	// Binds the socket and then sends the data to the specified location
	FreeRTOS_bind( xSocket, &xLocalAddress, sizeof( xLocalAddress ) );
	Returned = FreeRTOS_sendto(xSocket, Buffer, strlen(Buffer), 0, &xSendUDPMessageAddress, sizeof(xSendUDPMessageAddress));
	if(Returned > 0)
	{
		sprintf(pcWriteBuffer,"Sent!");	// If the data is sent, notify the user
	}
	FreeRTOS_closesocket(xSocket);	// Closes the socket
	Returned = 0;	// Set the socket back to zero
	
	return pdFALSE;
}

BaseType_t prvPrintNameCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString) 
{ 
	BaseType_t xReturnValue = pdFALSE;
	// Get rid of compiler warnings about unused variables 
	( void ) pcCommandString; 
	( void ) xWriteBufferLen; 
	configASSERT( pcWriteBuffer );
	// Start with an empty string 
	pcWriteBuffer[0] = '\0';
	sprintf(pcWriteBuffer, "Name is: %s\r\n", myName);
	
	return xReturnValue; 
}