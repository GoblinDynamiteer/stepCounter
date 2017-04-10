/*	 stepCounter

	Project for course "Programmering av inbyggda system"
		- Mjukvaruutvecklare inbyggda system

	Get steps with MPU-9250 acceleration data
	Display step count to display SSD 1306

	Members:
	Simon Karlsson
	Dennis Bunne
	Johan Kämpe
	*/


#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>
#include <math.h>

#include "i2chw/i2cmaster.h"
#include "mpu6050/mpu6050.h" //MPU lib
#include "u8g/u8g.h" //OLED display lib

/* Adresses in MPU-9250 for getting acceleration data	*/
#define X 0x3B
#define Y 0x3D
#define Z 0x3F

#define STEP_ACC_TRIGGER 2.3

u8g_t u8g;
uint16_t steps = 0;
double accIdle = 0;
double accCombined = 0;
double accX = 0.0, accY = 0.0, accZ = 0.0;

double getAcc(int addr);
void drawSteps(uint16_t steps);
void drawString(char * string);
void setAccIdle();
double getAccXYZ(void);

int main(void) {
	/*	 Init MPU	*/
	sei();
	mpu6050_init();
	_delay_ms(50);

	/*	 Init display	*/
	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE);
	u8g_SetFont(&u8g, u8g_font_fub14);

	_delay_ms(50);
	drawString("STEPCOUNTER!");

	setAccIdle();
	_delay_ms(50);

	DDRB |= (1 << PB0);

	while(1) {
		accCombined = getAccXYZ();
		if(!accCombined){
			PORTB= 0b00000001;
			_delay_ms(100);
			PORTB= 0b00000000;
			_delay_ms(100);
		}
		if(fabs(accCombined - accIdle) > STEP_ACC_TRIGGER){
			steps++;
			PORTB= 0b00000001;
			drawSteps(steps);
			_delay_ms(50);
			PORTB= 0b00000000;
		}
		_delay_ms(10);
	}
}

/*	 Get added value of acceleration for X, Y and Z	*/
double getAccXYZ(void){
	accX = getAcc(X);
	accY = getAcc(Y);
	accZ = getAcc(Z);
	return (accX + accY + accZ);
}

/*	 Get acceleration data, input is adress for X, Y or Z	*/
double getAcc(int addr){
	int16_t ret = 0;
	uint8_t buffer[2];
	i2c_start(MPU6050_ADDR | I2C_WRITE);
	i2c_write(addr);
	_delay_us(10);
	i2c_start(MPU6050_ADDR | I2C_READ);
	buffer[0] = i2c_readAck();
	buffer[1] = i2c_readNak();
	i2c_stop();
	ret = fabs((((int16_t)buffer[0]) << 8) | buffer[1]);
	return (double)(ret)/MPU6050_AGAIN;
}

/*	 Draw step count to display	*/
void drawSteps(uint16_t steps){
	char counterString[6] = "\0";
	itoa(steps, counterString, 10);
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, 16, counterString);
	}while(u8g_NextPage(&u8g));
}

/*	 Draw text string to display	*/
void drawString(char * string){
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, 16*2, string);
	}while(u8g_NextPage(&u8g));
}

/* Determine acceleration idle value	*/
void setAccIdle(){
	for(int i = 0; i < 10; i++){
		 accIdle += getAccXYZ();
		 drawString("setAccIdle");
		 _delay_ms(100);
	}
	accIdle /= 10;
}
