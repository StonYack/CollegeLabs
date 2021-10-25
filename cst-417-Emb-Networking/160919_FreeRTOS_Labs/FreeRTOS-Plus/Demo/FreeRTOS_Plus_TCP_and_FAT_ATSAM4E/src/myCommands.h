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
#ifndef MYCOMMANDS_H_
#define MYCOMMANDS_H_


void vRegisterMyCommands(void);

// Define Command Functions
BaseType_t prvNetStatusCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvDnsLookupCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvArpReqCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvPingRequestCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

BaseType_t prvGetMacCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvGetIPAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvGetGatewayAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvGetDNSServerAddress(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvGetNetmask(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);
BaseType_t prvOutputARPRequest(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

BaseType_t prvSendUDPMessageCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

BaseType_t prvPrintNameCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

#endif /* MYCOMMANDS_H_ */