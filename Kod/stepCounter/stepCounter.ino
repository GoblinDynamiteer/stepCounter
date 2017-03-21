// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include <Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
#define STEPDELAY 300
int16_t AcX, AcY, AcZ, combined, stepDelay = STEPDELAY;
unsigned long timer = 0;
int steps = 0, offset = 8100;

int16_t readAccX(void);
int16_t readAccY(void);
int16_t readAccZ(void);


void setup(){ // SDA / SCL for Arduino UNO: A4 / A5
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  combined = ((readAccX()+readAccY()+readAccZ()) / 3) - offset;
  Serial.println(combined);
  if(((combined <= -7000) || (combined >= 7000)) &&
  ((millis() - timer) > STEPDELAY)) {
      steps++;
      Serial.println(steps);
      timer = millis();
  }
}

int16_t readAccX(void){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);
    return Wire.read()<<8|Wire.read();
}

int16_t readAccY(void){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3D);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);
    return Wire.read()<<8|Wire.read();
}

int16_t readAccZ(void){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3F);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,2,true);
    return Wire.read()<<8|Wire.read();
}
