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

#define OFFSET_NUM 10
#define STEP_ACC_TRIGGER 2.5

u8g_t u8g;

double getAcc(int addr);
void drawSteps(uint16_t steps);
void drawString(char * string);
void drawAccData(double x, double y, double z);
void getAccOffset(double *ox, double *oy, double *oz);

int main(void) {
	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE);
	u8g_SetFont(&u8g, u8g_font_fub11);
	double combinedAcc = 0;
	double accX = 0.0, accY = 0.0, accZ = 0.0;
	double xOffset = 0.0, yOffset = 0.0, zOffset = 0.0;
	uint16_t steps = 0;
	getAccOffset(&xOffset, &yOffset, &zOffset);
	sei();
	DDRB |= (1 << PB0);
	/*	 Init MPU 6050	*/
	mpu6050_init();
	_delay_ms(50);
	drawString("Idle");
	while(1) {
		accX = getAcc(X) - xOffset; 
		accY = getAcc(Y) - yOffset;
		accZ = getAcc(Z) - zOffset;
		combinedAcc = (accX + accY + accZ);
		//drawAccData(accX, accY, accZ);
		while(1){
			drawAccData(accX, accY, accZ);
		}
		if(combinedAcc > STEP_ACC_TRIGGER){
			steps++;
			PORTB= 0b00000001;
			if(!(steps % 10)){
				drawSteps(steps);
				_delay_ms(10);
				drawString("Idle");
			}
			_delay_ms(10);
			PORTB= 0b00000000;
		}
		//drawSteps(xOffset);
		_delay_ms(5);
	}
}

double getAcc(int addr){
	int16_t ret = 0;
	uint8_t buffer[2]; //För att hålla två bytes med i2c_readAck
	i2c_start(MPU6050_ADDR | I2C_WRITE);
	i2c_write(addr);
	_delay_us(10);
	//read data
	i2c_start(MPU6050_ADDR | I2C_READ);
	buffer[0] = i2c_readAck();
	buffer[1] = i2c_readNak();
	i2c_stop();
	//ret = (((int16_t)buffer[0]) << 8) | buffer[1];
	ret = fabs((((int16_t)buffer[0]) << 8) | buffer[1]);
	//return fabs(ret) / 1000;
	return (double)(ret)/MPU6050_AGAIN; //Konverterar till g?
}

void drawSteps(uint16_t steps){
	char counterString[6] = "\0";
	itoa(steps, counterString, 10);
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, 16, counterString);
	}while(u8g_NextPage(&u8g));
}

void drawString(char * string){
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, 16*2, string);
	}while(u8g_NextPage(&u8g));
}

void drawAccData(double x, double y, double z){
	char ax[5] = "\0";
	char ay[5] = "\0";
	char az[5] = "\0";
	dtostrf(x,1,3,ax);
	dtostrf(y,1,3,ay);
	dtostrf(z,1,3,az);
	//itoa(x, ax, 10);
	//itoa(y, ay, 10);
	//itoa(z, az, 10);
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, 16, ax);
		u8g_DrawStr(&u8g, 2, 16*2 + 1, ay);
		u8g_DrawStr(&u8g, 2, 16*3 + 2, az);
	}while(u8g_NextPage(&u8g));
}

void getAccOffset(double *ox, double *oy, double *oz){
	double accX = 0, accY = 0, accZ = 0;
	drawString("Calibrating...");
	for(int i = 0; i < OFFSET_NUM; i++){
		accX += getAcc(X);
		accY += getAcc(Y); 
		accZ += getAcc(Z); 
		_delay_ms(10);
	}
	*ox = (double)(accX / OFFSET_NUM);
	*oy = (double)(accY / OFFSET_NUM);
	*oz = (double)(accZ / OFFSET_NUM);
	drawString("Calib. done");
	_delay_ms(500);
}
