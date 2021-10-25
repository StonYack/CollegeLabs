/*
 * myEchoServer.h
 *
 * Created: 4/30/2020 6:30:24 PM
 *  Author: Ston
 */ 


#ifndef MYECHOSERVER_H_
#define MYECHOSERVER_H_

#define myTCPechoSHUTDOWN_DELAY ( pdMS_TO_TICKS( 5000 ) ) 
#define myTCPechoPORT_NUMBER 8080

void StartMyEchoTCPServerTasks( uint16_t usStackSize, UBaseType_t uxPriority ); 
void MyEchoServerListeningTask( void *pvParameters ); 
void MyEchoServerInstance( void *pvParameters );

#endif /* MYECHOSERVER_H_ */