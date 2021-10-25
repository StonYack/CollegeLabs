/*
 * swDriver.h
 *
 * Created: 1/12/2021 3:01:27 PM
 *  Author: Ston
 */ 


#ifndef SWDRIVER_H_
#define SWDRIVER_H_


enum ACommand {increase, decrease};

void initializeSWDriver(void);
uint8_t readSW(void);
void Button1(uint8_t Cur);
void Button2(uint8_t Cur);
void Button3(uint8_t Cur);


#endif /* SWDRIVER_H_ */