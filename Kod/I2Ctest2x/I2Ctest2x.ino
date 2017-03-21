// MPU-6050 Short Example Sketch
// By Arduino User JohnChi
// August 17, 2014
// Public Domain
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
const int OLED_addr=0x3C; //Ic2 OLED display
#define STEPDELAY 300
int16_t AcX, AcY, AcZ, combined, stepDelay = STEPDELAY;
unsigned long timer = 0;
int steps = 0, offset = 8100;
void setup(){ // SDA / SCL for Arduino UNO: A4 / A5
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  combined = ((AcX+AcY+AcZ) / 3) - offset;
  if(((combined <= -7000) || (combined >= 7000)) &&
  ((millis() - timer) > STEPDELAY)) {
      steps++;
      Serial.println(steps);
      timer = millis();
  }
  delay(1000);
    Wire.beginTransmission(OLED_addr);
    Wire.write(0xAE);
    Wire.endTransmission(true);
  delay(1000);
    Wire.beginTransmission(OLED_addr);
    Wire.write(0xAF);
    Wire.endTransmission(true);
}
