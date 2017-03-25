
#define VIBRATION_TRIGGER 20
#define STEPDELAY 300
const int VIB_SENSOR_PIN = A0;

int steps = 0;
unsigned long timer = 0;

void setup(){
    Serial.begin(9600);
}

void loop(){
    float vibrationData = (float)analogRead(VIB_SENSOR_PIN) * 0.2;

    if((vibrationData >= VIBRATION_TRIGGER) &&
        (millis() - timer) > STEPDELAY) {
            steps++;
            Serial.println(vibrationData);
            Serial.println(steps);
            timer = millis();
    }
}
