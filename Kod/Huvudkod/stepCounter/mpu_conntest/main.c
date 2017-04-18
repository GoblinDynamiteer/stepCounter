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
#include <math.h> //fabs()
#include <stdio.h> //sprintf()

#include "i2chw/i2cmaster.h" //I2C master library using hardware TWI interface
#include "mpu6050/mpu6050.h" //MPU6050 lib 0x02
#include "u8g/u8g.h" 		//Universal 8bit Graphics Library

/* Addresses in MPU-9250 for getting acceleration data	*/
#define X 0x3B
#define Y 0x3D
#define Z 0x3F

/*	 Step trigger treshold 	*/
#define STEP_ACC_TRIGGER 2.3

/*	 Amount of values taken from MPU to set idle accelleration	*/
#define SET_IDLE_LOOP 20

/*	 For display	*/
#define DISPLAY_LINE_HEIGHT 16 //Pixel height for one row on display.
#define DISPLAY_MIDDLE (DISPLAY_LINE_HEIGHT * 2 + 2) //Middle(ish) of display
#define DISPLAY_SLEEP_DELAY 3000 //Value for triggering display sleep mode
u8g_t u8g; //u8g library struct, for holding display data

uint16_t steps = 0; //Step counter
double accIdle = 0; //Acceleration idle value
double accCombined = 0; //Combined XYZ acceleration value
double accX = 0.0, accY = 0.0, accZ = 0.0;
int displaySleeping = 0; //"bool" for display sleep mode, set to 1 if sleeping
int displaySleepTimer = 0; //Timer/ticker for setting display in sleep mode

double getAcc(int addr);
void drawSteps(uint16_t steps);
void drawString(char * string, int line);
void setAccIdle();
void toggleDisplaySleep(void);
double getAccXYZ(void);

int main(void) {
	/*	 Init MPU-9250 - MPU6050 lib*/
	sei();
	mpu6050_init();
	_delay_ms(50);

	/*	 Init display - u8g lib	*/
	u8g_InitI2C(&u8g, &u8g_dev_ssd1306_128x64_i2c, U8G_I2C_OPT_NONE);
	u8g_SetFont(&u8g, u8g_font_fub14);

	_delay_ms(50);
	/*	 Display start-up text on display	*/
	drawString("STEPCOUNTER!", DISPLAY_MIDDLE);
	_delay_ms(1000);

	/*	 Determine and set MPU idle accelerometer value.	*/
	setAccIdle();
	_delay_ms(50);

	/*  Main loop	*/
	while(1) {
		accCombined = getAccXYZ(); // get combined XYZ acceleration value
		if(fabs(accCombined - accIdle) > STEP_ACC_TRIGGER){
			/*	 Wake display if sleeping	*/
			if(displaySleeping){
				toggleDisplaySleep();
			}
			/*	 Show step count on display	*/
			drawSteps(steps++);
			_delay_ms(50);
		}
		_delay_ms(10);
		/*	 Set display to sleep if timer is reached	*/
		if(displaySleepTimer++ > DISPLAY_SLEEP_DELAY){
			displaySleepTimer = 0;
			if(!displaySleeping){
				drawString("Sleeping", DISPLAY_MIDDLE);
				_delay_ms(1000);
				toggleDisplaySleep();
			}
		}
	}
}

/*	 Get added value of acceleration for X, Y and Z	*/
double getAccXYZ(void){
	accX = getAcc(X);
	accY = getAcc(Y);
	accZ = getAcc(Z);
	return (accX + accY + accZ);
}

/*	 Get acceleration data, input is address for X, Y or Z	*/
double getAcc(int addr){
	int16_t ret = 0; // return value
	uint8_t buffer[2];
	/*  Read data from MPU Adress	*/
	i2c_start(MPU6050_ADDR | I2C_WRITE);
	i2c_write(addr);
	_delay_us(10);
	i2c_start(MPU6050_ADDR | I2C_READ);
	buffer[0] = i2c_readAck();
	buffer[1] = i2c_readNak();
	i2c_stop();
	/*	 	*/
	ret = fabs((((int16_t)buffer[0]) << 8) | buffer[1]);
	return (double)(ret)/MPU6050_AGAIN;
}

/*	 Draw step count to display	*/
void drawSteps(uint16_t steps){
	char counterString[20] = "\0";
	sprintf(counterString, "Steps: %i", steps);
	drawString(counterString, DISPLAY_MIDDLE);
}

/*	 Draw text string to display	*/
void drawString(char * string, int line){
	u8g_FirstPage(&u8g);
	do{
		u8g_DrawStr(&u8g, 2, line, string);
	}while(u8g_NextPage(&u8g));
}

/* Determine acceleration idle value	*/
void setAccIdle(){
	for(int i = 0; i < SET_IDLE_LOOP; i++){
		 accIdle += getAccXYZ();
		 drawString("Calibrating..", DISPLAY_MIDDLE);
		 _delay_ms(100);
	}
	accIdle /= SET_IDLE_LOOP;
	drawString("Idle..", DISPLAY_MIDDLE);
}

/*	 Toggle display sleep  mode on/off	*/
void toggleDisplaySleep(void){
	if(displaySleeping){
		u8g_SleepOff(&u8g);
		displaySleeping = 0;
	}
	else{
		u8g_SleepOn(&u8g);
		displaySleeping = 1;
	}
}
