/*
 * mpu_test.c
 *
 * Created: 2017-03-20 14:05:09
 * Author : Dennis Bunne, Johan Kämpe och Simon Karlsson
 */ 

#define F_CPU 1600000
#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
	DDRB = (1<<PB3);    //OUTPUT
	//0b 0000 1000
	
	while(1)
	{
		PORTB |= (1<<PB3);
		_delay_ms(300);
		PORTB &=~ (1<<PB3);
		_delay_ms(300);
	}
}
