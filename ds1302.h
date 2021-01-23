/*
 * ds1302.h
 *
 *  Created on: 4/10/2019
 *      Author: Jesús
 */

#ifndef DS1302_H_
#define DS1302_H_

#include "driverlib.h"
#include "hal_LCD.h"

//**********************************************************************************************************************************************************
// Commands to handle the RTC.
#define DS1302_SECONDS           0x80
#define DS1302_MINUTES           0x82
#define DS1302_HOURS             0x84
#define DS1302_DATE              0x86
#define DS1302_MONTH             0x88
#define DS1302_DAY               0x8A
#define DS1302_YEAR              0x8C
#define DS1302_CONTROL			 0x8E
#define DS1302_TRICKLE			 0x90
#define DS1302_CLOCK_BURST_WRITE 0xBE
#define DS1302_CLOCK_BURST_READ  0xBF
#define DS1302_RAMSTART          0xC0
#define DS1302_RAMEND            0xFC
#define DS1302_RAM_BURST_WRITE   0xFE
#define DS1302_RAM_BURST_READ    0xFF

//**********************************************************************************************************************************************************
// Defines to configure pins.
#define DS1302_RST				{ P1DIR |= BIT5; P1REN &= ~BIT5;}
#define DS1302_RST_H     		P1OUT |= BIT5
#define DS1302_RST_L     		P1OUT &= ~BIT5

#define DS1302_SCLK				{ P1DIR |= BIT3; P1REN &= ~BIT3;}
#define DS1302_SCLK_H     		P1OUT |= BIT3
#define DS1302_SCLK_L     		P1OUT &= ~BIT3

#define DS1302_IO_WR_H      	P1OUT |= BIT4
#define DS1302_IO_WR_L      	P1OUT &= ~BIT4
#define DS1302_IO_RD      		P1IN  &  BIT4
#define DS1302_IO_WR_MODE   	{ P1DIR |= BIT4; P1REN &= ~BIT4;}
#define DS1302_IO_RD_MODE   	{ P1DIR &= ~BIT4; P1REN &= ~BIT4;}

//**********************************************************************************************************************************************************
// Function prototypes.
void DS1302_Init_Pins(void);
void DS1302_Start(void);
void DS1302_Stop(void);
uint8_t DS1302_Read_Byte(void);
void DS1302_Write_Byte(uint8_t);
void DS1302_Set_Time(uint8_t *);
void DS1302_Read_Time(uint8_t *);
void DS1302_Write_RAM(uint8_t *,uint8_t);
void DS1302_Read_RAM(uint8_t *,uint8_t);
uint8_t DS1302_BCD_to_Bin(uint8_t);
void DS1302_Show_Date(uint8_t*);

#endif /* DS1302_H_ */
