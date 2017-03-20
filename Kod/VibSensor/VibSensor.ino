/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

void setup(){
    Serial.begin(9600);
}

void loop(){
    float vibrationData =
      (float)analogRead(VIB_SENSOR_PIN) * 0.5;
    Serial.println(vibrationData);
    delay(10);
}
