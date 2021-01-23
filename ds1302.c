/*
 * ds1302.c
 *
 *  Created on: 4/10/2019
 *      Author: Jesús
 */

#include "ds1302.h"

//**********************************************************************************************************************************************************
// Structure that simplifies the conversion from BCD to binary.
struct {
	uint8_t NIB_L:4;
	uint8_t NIB_H:4;
}CONV;

//**********************************************************************************************************************************************************
// Initializes the RST and SCLK pins in OUT mode.
void DS1302_Init_Pins(void)
{
	DS1302_RST;
	DS1302_SCLK;
}

//**********************************************************************************************************************************************************
// Starts the communication with the RTC.
void DS1302_Start(void)
{
	DS1302_RST_L;
	DS1302_SCLK_L;
	DS1302_RST_H;
}

//**********************************************************************************************************************************************************
// Ends communication with the RTC.
void DS1302_Stop(void)
{
	DS1302_RST_L;
}

//**********************************************************************************************************************************************************
// Read a byte. The bytes should be read on the falling edge.
uint8_t DS1302_Read_Byte(void)
{
	uint8_t i,byte_data = 0;
    DS1302_IO_RD_MODE;

    for(i = 8; i > 0; i--)
    {
        DS1302_SCLK_H;
        DS1302_SCLK_L;
        byte_data>>=1;

        if(DS1302_IO_RD)
        {
            byte_data|=0x80;
        }
    }
    return(byte_data);
}

//**********************************************************************************************************************************************************
// Writes a byte. The bytes should be written on the rising edge.
void DS1302_Write_Byte(uint8_t byte_data)
{
    uint8_t i;
    DS1302_IO_WR_MODE;

    for(i = 8; i > 0; i--)
    {
        if(byte_data&0x01)
        {
            DS1302_IO_WR_H;
        }
        else
        {
            DS1302_IO_WR_L;
        }
        DS1302_SCLK_L;
        DS1302_SCLK_H;
        byte_data>>=1;
    }
}

//**********************************************************************************************************************************************************
// Set the date and time individually.
// The order should be seconds, minutes, hours, dayofweek, dayofmonth, month, year (All in BCD).
void DS1302_Set_Time(uint8_t *buf)
{
    buf[0] &= 0x7f;
    buf[1] &= 0x7f;
    buf[2] &= 0x7f;
    buf[3] &= 0x3f;
    buf[4] &= 0x1f;
    buf[5] &= 0x07;
    buf[6] &= 0xff;

	DS1302_Start();
	DS1302_Write_Byte(DS1302_CONTROL);
	DS1302_Write_Byte(0x00);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_SECONDS);
	DS1302_Write_Byte(buf[0]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_MINUTES);
	DS1302_Write_Byte(buf[1]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_HOURS);
	DS1302_Write_Byte(buf[2]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_DATE);
	DS1302_Write_Byte(buf[3]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_MONTH);
	DS1302_Write_Byte(buf[4]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_DAY);
	DS1302_Write_Byte(buf[5]);
	DS1302_Stop();
	DS1302_Start();
	DS1302_Write_Byte(DS1302_YEAR);
	DS1302_Write_Byte(buf[6]);
	DS1302_Stop();
}

//**********************************************************************************************************************************************************
// Read the time from the RTC registers in burst mode.
void DS1302_Read_Time(uint8_t *buf)
{
	uint8_t i;
	DS1302_Start();
    DS1302_Write_Byte(DS1302_CLOCK_BURST_READ);

    for(i = 7; i > 0 ; i--)
    {
        buf[7u - i]=DS1302_Read_Byte();
    }

    DS1302_Stop();

    for(i = 7; i > 0 ; i--)
    	buf[7u - i] = DS1302_BCD_to_Bin(buf[7u - i]);
}

//**********************************************************************************************************************************************************
// Writes the RAM in burst mode.
void DS1302_Write_RAM(uint8_t *buf,uint8_t len)
{
    unsigned int i;

	DS1302_Start();
    DS1302_Write_Byte(DS1302_CONTROL);
    DS1302_Write_Byte(0x00);
    DS1302_Stop();

	DS1302_Start();
    DS1302_Write_Byte(DS1302_RAM_BURST_WRITE);

    for(i = len; i > 0 ; i--)
    {
       DS1302_Write_Byte(buf[len - i]);
    }

    DS1302_Start();
    DS1302_Write_Byte(DS1302_CONTROL);
    DS1302_Write_Byte(0x80);
    DS1302_Stop();
}

//**********************************************************************************************************************************************************
// Read the RAM in burst mode.
void DS1302_Read_RAM(uint8_t *buf,uint8_t len)
{
    unsigned int i;
	DS1302_Start();
    DS1302_Write_Byte(DS1302_RAM_BURST_READ);

    for(i = len; i > 0 ; i--)
    {
        buf[len - i]=DS1302_Read_Byte();
    }

    DS1302_Stop();
}

//**********************************************************************************************************************************************************
// Converts from BCD to binary
uint8_t DS1302_BCD_to_Bin(uint8_t val)
{
   CONV.NIB_H = val/16;
   CONV.NIB_L = val%16;
   return(CONV.NIB_H*10+CONV.NIB_L);
}

//**********************************************************************************************************************************************************
// Shows the Date and Hour.
void DS1302_Show_Date(uint8_t *buf)
{
    uint16_t aux;
    aux=(buf[0])/10;
    showChar(aux+48,pos5);
    aux=(buf[0])%10;
    showChar(aux+48,pos6);

    aux=(buf[1])/10;
    showChar(aux+48,pos3);
    aux=(buf[1])%10;
    showChar(aux+48,pos4);
    LCDMEM[pos4+1] |= 0x04;     // ':'

    aux=(buf[2])/10;
    showChar(aux+48,pos1);
    aux=(buf[2])%10;
    showChar(aux+48,pos2);
    LCDMEM[pos2+1] |= 0x04;     // ':'
}
