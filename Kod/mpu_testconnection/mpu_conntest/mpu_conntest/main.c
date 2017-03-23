#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "i2chw/i2cmaster.h"

#define F_CPU 1000000UL
#include <util/delay.h>
#include <math.h>  //include libm

#include "mpu6050/mpu6050.h"

//#define X MPU6050_RA_ACCEL_XOUT_H //Adress för accelerationdata X
//#define Y MPU6050_RA_ACCEL_YOUT_H //Adress för accelerationdata Y
//#define Z MPU6050_RA_ACCEL_ZOUT_H //Adress för accelerationdata Z

#define X 0x3B //Adress för accelerationdata X
#define Y 0x3D //Adress för accelerationdata Y
#define Z 0x3F //Adress för accelerationdata Z

double getAcc(int addr);


int main(void) {
	DDRB = PB0;
	
	//init interrupt
	sei();

	//init mpu6050
	mpu6050_init();
	_delay_ms(50);

	while(1) {
		/*	 Tänd LED 1 sek	*/
		double combinedAcc = fabs(getAcc(X)) + fabs(getAcc(Y)) + fabs(getAcc(Z));
		if(combinedAcc > 2.0){
			PORTB |= (1<<PB0);
			_delay_ms(1000);
			PORTB &= ~(1<<PB0);
		}
		_delay_ms(1);
	}
}

double getAcc(int addr){
	int16_t ret = 0;
	uint8_t buffer[2];
	i2c_start(MPU6050_ADDR | I2C_WRITE);
	i2c_write(addr);
	_delay_us(10);
	//read data
	i2c_start(MPU6050_ADDR | I2C_READ);
	buffer[0] = i2c_readAck();
	buffer[1] = i2c_readNak();
	i2c_stop();
	ret = (((int16_t)buffer[0]) << 8) | buffer[1];
	return (double)(ret)/MPU6050_AGAIN; //Konverterar till g?
}


