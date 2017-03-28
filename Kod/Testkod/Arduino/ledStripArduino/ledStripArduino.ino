#include <Arduino.h>
#include <Adafruit_DotStar.h>

/*   Led-strip   */
#define NUMPIXELS 6
#define NUMCOLORS 7
#define DATAPIN 4
#define CLOCKPIN 5
#define LED_MAX_STR 250
#define LED_BRIGHTNESS_MAX 250
#define LED_BRIGHTNESS_HUM_MIN 10
#define LED_BRIGHTNESS_HUM_MAX 20

/*   Vibration sensor  */
#define VIBRATION_TRIGGER 10
const int VIB_SENSOR_PIN = A0;

/*   Delay between hit triggers  */
#define HIT_DELAY 100

/*   "Game" works for 60 seconds  */
#define PLAY_TIME 60000


/*  Counts hits on racket   */
int hits = 0;

/*   Predefined colors  */
uint32_t ledColor[NUMCOLORS] = {
  0x00FF00, 0x0000FF, 0xFF0000, 0xA5FF00,
  0x00FFFF, 0xFF00FF, 0xCCFFCC
};

/*  Indexes for ledColor array   */
enum{RED, BLUE, GREEN, YELLOW, PURPLE, CYAN, WHITE};

/*  Timers   */
unsigned long timer = 0;
unsigned long startTime = 0;


/*   Init LED-strip as "strip"  */
Adafruit_DotStar strip = Adafruit_DotStar(
    NUMPIXELS,
    DATAPIN,
    CLOCKPIN,
    DOTSTAR_BRG
  );

void setup(){
  /* Start Led-strip  */
  strip.begin();
  strip.setBrightness(LED_BRIGHTNESS_MAX);
  for(int i = 0; i < NUMPIXELS; i++){
      strip.setPixelColor(i, ledColor[RED]);
  }
  strip.show();
}

void loop(){

}
