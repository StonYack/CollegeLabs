/*
 * myUDPServer.h
 *
 * Created: 5/7/2020 6:31:12 PM
 *  Author: Ston
 */ 


#ifndef MYUDPSERVER_H_ 
#define MYUDPSERVER_H_
#include "FreeRTOS_IP.h"

#define MY_UDP_PORT_NUMBER 9930 
#define ipconfigUDP_MSS (ipconfigNETWORK_MTU - ipSIZE_OF_IPv4_HEADER - ipSIZE_OF_UDP_HEADER)

void StartMyUdpServerTasks( uint16_t usStackSize, UBaseType_t uxPriority ); 
void MyUDPServerTask(void * pvParameters);

#endif /* MYUDPSERVER_H_ */