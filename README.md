<h1>stepCounter</h1>
<h2>Pocket Pedometer</h2>  

---


**Student project for course "Programming of embedded systems"**   
*Mjukvaruutvecklare inbyggda system*  
*Mölk Utbildning, Mjärdevi - Linköping*  

Members  
[Simon Karlsson](https://www.linkedin.com/in/simon-karlsson-54509aa9/)  
[Johan Kämpe](https://www.linkedin.com/in/johankampe/)  
[Dennis Bunne](https://www.linkedin.com/in/dennis-bunne-259a9783/)  

---

**Description:**  
Simple pedometer built with an Atmel Atmega328p MCU and 3-axis accelerometer. Steps are displayed on a small display.

---


*Goals:*
- Pocket/shoe-wearable
- Bluetooth / phone-app
- Reset / on-off / screen-on switches
- Modularization (I2C devices)
- Screen sleep mode
- Long battery life

---

Current Parts:
- Atmel Atmega328p
- 28 pin MCU socket
- MPU-9250 Gyro / Accelerometer / Compass Unit
- Grove connectors for I2C-modules
- SSD1306 Display
- Resistors for I2C Line Pull up
- LiPo 18650 Battery
- 18650 Battery holder
- Mouse housing for prototype

---

**Photos:**  
MPU-9250 Accelerometer data test with Arduino Uno  
![MPU-9250](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-03-20_arduino_sensors.jpg)

Soldered board with connected I2C modules (Display and Accelerometer)
![Prototype](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-04-05_prototoype.jpg)

Backside of board and disconnected I2C modules
![Prototype](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-04-05_prototoype_modules.jpg)

![Prototype](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-04-05_prototoype_modules_batt.jpg)

Housing made of cannibalized mouse, tape, glue.
![Prototype](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-04-05_prototoype_housing_batt.jpg)

Housing front
![Prototype](https://raw.githubusercontent.com/GoblinDynamiteer/stepCounter/master/Dokumentation/Bilder/Foto/2017-04-05_prototoype_housing_screen.jpg)
