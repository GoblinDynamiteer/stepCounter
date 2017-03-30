#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "i2chw/i2cmaster.h"
#include <util/delay.h>
#include <math.h>

#include "mpu6050/mpu6050.h"
#include "u8g/u8g.h"

#define X 0x3B //Adress f�r accelerationdata X
#define Y 0x3D //Adress f�r accelerationdata Y
#define Z 0x3F //Adress f�r accelerationdata Z

u8g_t u8g;

double getAcc(int addr);
void drawSteps(uint16_t steps);

int main(void) {
	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE);


	sei();
	/*	 Init MPU 6050	*/
	mpu6050_init();
	_delay_ms(50);

	uint16_t steps = 0;

	while(1) {
		double combinedAcc = fabs(getAcc(X)) + fabs(getAcc(Y)) + fabs(getAcc(Z));
		if(combinedAcc > 2.0){
			steps++;
			u8g_FirstPage(&u8g);
			do{
				drawSteps(steps);
			}while(u8g_NextPage(&u8g));
		}
		//u8g_Delay(100);
		steps++;
		_delay_ms(1000);
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

void drawSteps(uint16_t steps){
	char csteps[4] = "";
	itoa(steps, csteps, 10);
	u8g_SetFont(&u8g, u8g_font_fub14);
	u8g_DrawStr(&u8g, 2, 16, csteps);
}
