/*
 * OLEDLib.c
 *
 * Created: 2017-04-01 18:01:01
 * Author : kampe_000
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "i2chw/i2cmaster.h"

#define SSD1306_I2C_ADDRESS (0b01111000) //0111100 med SA0 = 0, sida 20 i datablad
#define SSD1306_WRITE (SSD1306_I2C_ADDRESS + 0)

void blinkLed(){
	while(1){
		PORTB = 0xff; /*	 Tänd LED på alla PORTB 	*/
		_delay_ms(300);
		PORTB = 0x00; /*	 Tänd LED på alla PORTB 	*/
		_delay_ms(300);
	}
}

int main(void)
{
	DDRB  = 0xff;
	PORTB = 0x00;
	
	i2c_init();
	unsigned char i2cStatus = i2c_start(SSD1306_WRITE);
		
	if(i2cStatus){
		blinkLed();
	}
	i2c_write(0xAE);
 
 i2c_write(0x00 | 0x0);            // low col = 0
 i2c_write(0x10 | 0x0);           // hi col = 0
 i2c_write(0x40 | 0x0);            // line #0
 
 i2c_write(0x81);                   // Set Contrast 0x81
 i2c_write(0xCF);
 // flips display
 i2c_write(0xA1);                    // Segremap - 0xA1
 i2c_write(0xC8);                    // COMSCAN DEC 0xC8 C0
 i2c_write(0xA6);                    // Normal Display 0xA6 (Invert A7)
 
 i2c_write(0xA4);                // DISPLAY ALL ON RESUME - 0xA4
 i2c_write(0xA8);                    // Set Multiplex 0xA8
 i2c_write(0x3F);                    // 1/64 Duty Cycle
 
 i2c_write(0xD3);                    // Set Display Offset 0xD3
 i2c_write(0x0);                     // no offset
 
 i2c_write(0xD5);                    // Set Display Clk Div 0xD5
 i2c_write(0x80);                    // Recommneded resistor ratio 0x80
 
 i2c_write(0xD9);                  // Set Precharge 0xd9
 i2c_write(0xF1);
 
 i2c_write(0xDA);                    // Set COM Pins0xDA
 i2c_write(0x12);
 
 i2c_write(0xDB);                 // Set VCOM Detect - 0xDB
 i2c_write(0x40);
 
 i2c_write(0x20);                    // Set Memory Addressing Mode
 i2c_write(0x00);                    // 0x00 - Horizontal
 
 i2c_write(0x40 | 0x0);              // Set start line at line 0 - 0x40
 
 i2c_write(0x8D);                    // Charge Pump -0x8D
 i2c_write(0x14);
 
 
 i2c_write(0xA4);              //--turn on all pixels - A5. Regular mode A4
 i2c_write(0xAF);                //--turn on oled panel - AF
	
    while (1) 
    {
    }
}

