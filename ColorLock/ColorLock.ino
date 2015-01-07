#include <Adafruit_NeoPixel.h>

const int PIN_BUTTON_UP=4;
const int PIN_BUTTON_IN=3;
const int PIN_LED=5;
const int PIN_RELAY=13;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, PIN_LED, NEO_GRB + NEO_KHZ800);

const int COLORS_SIZE = 8;
const uint32_t BLACK = led.Color(0,0,0);
const uint32_t RED = led.Color(255,0,0);
const uint32_t ORANGE = led.Color(255,64,0);
const uint32_t YELLOW = led.Color(255,128,0);
const uint32_t GREEN = led.Color(0,255,0);
const uint32_t BLUE = led.Color(0,0,255);
const uint32_t VIOLET = led.Color(128,0,255);
const uint32_t WHITE = led.Color(255,255,255);
const uint32_t* COLORS = new uint32_t[COLORS_SIZE]{BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET, WHITE};

const int CODE_SIZE = 3;
const int* CODE = new int[CODE_SIZE]{1, 7, 5};//RED, WHITE, BLUE

int colorIndex = 0;
boolean canRead = false;
int codeIndex = 0;

void setup() {
  led.begin();
  led.setBrightness(64);
  led.setPixelColor(0,COLORS[0]);
  led.show();

  pinMode(PIN_BUTTON_UP, INPUT);
  pinMode(PIN_BUTTON_IN, INPUT);

  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, LOW);

  Serial.begin(9600);
}

void loop() {
  
  int buttonUpState = digitalRead(PIN_BUTTON_UP);
  int buttonInState = digitalRead(PIN_BUTTON_IN);
  
  if(canRead && buttonUpState == LOW) {
    Serial.println("UP");
    changeColor();
  } else if(canRead && buttonInState == LOW) {
    Serial.println("IN");
    checkCode();
  }
 
  if(buttonUpState == HIGH && buttonInState == HIGH) {
    canRead = true;
  } else {
    canRead = false;
  }
  
  delay(1);
}

void changeColor() {
  colorIndex=(colorIndex + 1) % COLORS_SIZE;
  led.setPixelColor(0,COLORS[colorIndex]);
  led.show();
}

void checkCode() {
  Serial.print("colorIndex: ");
  Serial.println(colorIndex);
  Serial.print("codeIndex: ");
  Serial.println(codeIndex);
  Serial.print("CODE[codeIndex]: ");
  Serial.println(CODE[codeIndex]);
  
  if(colorIndex==CODE[codeIndex]) {
    Serial.println("Match");
    codeIndex++;
    if(codeIndex==CODE_SIZE) {
      unlock();
      reset();
    }
  } else {
    Serial.println("No match.  Reset.");
    codeIndex = 0;
  }
}

void unlock() {
  Serial.println("Unlocking!");
  digitalWrite(PIN_RELAY, HIGH);
  for(int i=0; i<25; i++) {
    led.setPixelColor(0,GREEN);
    led.show();
    delay(100);
    led.setPixelColor(0,BLACK);
    led.show();
    delay(100);
  }
  digitalWrite(PIN_RELAY, LOW);
  led.setPixelColor(0,RED);
  led.show();
  delay(1000);
  led.setPixelColor(0,BLACK);
  led.show();
}

void reset() {
  Serial.println("Reset");
  codeIndex = 0;
  colorIndex = 0;
  led.setPixelColor(0,COLORS[colorIndex]);
  led.show();
}
