/*
 * ledDriver.h
 *
 * Created: 1/12/2021 3:01:10 PM
 *  Author: Ston
 */ 


#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_
#include <asf.h>
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>

void initializeLEDDriver(void);
uint8_t readLED(uint8_t uiLedNum);
void setLED(uint8_t uiLedNum, uint8_t uiLEDValue);
uint8_t toggleLED(uint8_t uiLedNum);


#endif /* LEDDRIVER_H_ */