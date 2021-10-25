/*
 * mytasks.h
 *
 * Created: 1/4/2021 12:58:00 PM
 *  Author: Ston Yackamouih
 */ 


#ifndef MYTASKS_H_
#define MYTASKS_H_

void LEDTask(void * pvParameters);
void HeartbeatTask(void * pvParameters);
void TXTask(void * pvParameters);
void RXTask(void * pvParameters);

#endif /* MYTASKS_H_ */