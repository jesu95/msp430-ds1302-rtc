#include "ds1302.h"

void main(void) {

	uint8_t bytes[7]={0x00,0x00,0x17,0x06,0x23,0x01,0x21}; // data in BCD code.

	WDT_A_hold(WDT_A_BASE);
    DS1302_Init_Pins();
    Init_LCD();
    PMM_unlockLPM5();
    DS1302_Set_Time(bytes);

    while(1){
    	DS1302_Read_Time(bytes);
    	DS1302_Show_Date(bytes);
    }
}
