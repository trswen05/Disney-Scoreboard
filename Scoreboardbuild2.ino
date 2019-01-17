#include <Adafruit_NeoPixel.h>
#include <FastLED.h>

const int HOMEA = 6; //home minus 1
const int HOMEB = 5; //home plus 1
const int AWAYA = 3; //away minus 1
const int AWAYB = 2; //away plus 1
const int RESCORE = 4; //reset score
#define BONUS_PIN  7  //bonus led loop
#define PIXEL_PIN  9 //neopixel data pin
#define NUMPIXELS  112 //number of pixels in score
#define BONPIXELS 4 //number of pizels in bonus

#define ONE_LED 49 // Game 1 Data
#define TWO_LED 50 // Game 2 Data
#define THREE_LED 51 // Game 3 Data
#define FOUR_LED 52 // Game 4 Data
#define FIVE_LED 53 // Game 5 Data

unsigned long previousTime;
unsigned long previousGame;
unsigned long currentGame;
unsigned long currentTime;
unsigned long bonusTime = 10000;
unsigned long gameTime = 7000;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_RGB + NEO_KHZ800);
CRGB bonus[BONPIXELS];

int buttonState;
int previous = HIGH;
int val = 0;

int brightness = 100;
int off = 0;

int timer = 7000;

uint32_t colorlist[] = {pixels.Color(0, 0, 255), pixels.Color(255, 0, 0)};

const int characterOffset = 28;
int score[2] = {0, 0};

int charLayout[10][28] = {
  {1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1}, //0
  {1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1}, //1
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1}, //2
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1}, //3
  {1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //4
  {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1}, //5
  {1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1}, //6
  {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1}, //7
  {1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1}, //9
};



void setup() {
  delay(2000);
  FastLED.addLeds<WS2811, BONUS_PIN, RGB>(bonus, BONPIXELS);


  pinMode(HOMEA, INPUT);
  pinMode(HOMEB, INPUT);
  pinMode(AWAYA, INPUT);
  pinMode(AWAYB, INPUT);
  pinMode(RESCORE, INPUT);
  pinMode(PIXEL_PIN, OUTPUT);
  digitalWrite(HOMEA, HIGH);
  digitalWrite(HOMEB, HIGH);
  digitalWrite(AWAYA, HIGH);
  digitalWrite(AWAYB, HIGH);
  digitalWrite(RESCORE, HIGH);

  pixels.begin();
  pixels.clear();
  pixels.show();

  for (int thisPin = 49; thisPin < 54; thisPin++) {
    pinMode(thisPin, OUTPUT);
  }

  setNumber(0, 00);
  setNumber(1, 00);
}

void loop() {

//the following listens for each button to be pressed. If a button is pressed, it adds/subtracts 1
//or resets the score.

  { buttonState = digitalRead(HOMEB);


    if (buttonState == LOW && previous == HIGH ) {
      setScore(0, 1);
      delay(250);
      previous == buttonState;
    }
  }

  { buttonState = digitalRead(HOMEA);

    if (buttonState == LOW && previous == HIGH ) {
      setScore(0, -1);
      delay(250);
      previous == buttonState;
    }
  }

  { buttonState = digitalRead(AWAYA);

    if (buttonState == LOW && previous == HIGH) {
      setScore(1, -1);
      delay(250);
      previous == buttonState;
    }
  }
  { buttonState = digitalRead(AWAYB);

    if (buttonState == LOW && previous == HIGH) {
      setScore(1, 1);
      delay(250);
      previous == buttonState;
    }
  }

  { buttonState = digitalRead(RESCORE);
    if (buttonState == LOW && previous == HIGH) {
      setScore(0, -100);
      setScore(1, -100);
      delay(250);
      previous == buttonState;
    }
  }

//the following cycles through the bonus lights one at a time.
  int current = (millis() - previousTime) / 1000;
  if (current < 10) {
    bonus[0] = CRGB(255, 255, 255);
    bonus[1] = CRGB(255, 255, 255);
    bonus[2] = CRGB::Black;
    bonus[3] = CRGB::Black;
    FastLED.show();
  }
  else
  {
    bonus[2] = CRGB(255, 255, 255);
    bonus[3] = CRGB(255, 255, 255);
    bonus[0] = CRGB::Black;
    bonus[1] = CRGB::Black;
    FastLED.show();
  }

  if (current >= 20) {
    previousTime = millis();
  }

//this following portion cycles throught the Game lights one at a time

  int game = (millis() - previousGame) / 1000;

  if (game < 7) {
    digitalWrite(ONE_LED, brightness);
  }
  else {
    digitalWrite(ONE_LED, off);
  }

  if (game >= 7 && game < 14) {
    digitalWrite(TWO_LED, brightness);
  }
  else {
    digitalWrite(TWO_LED, off);
  }

  if (game >= 14 && game < 21) {

    digitalWrite(THREE_LED, brightness);
  }
  else {
    digitalWrite(THREE_LED, off);
  }

  if (game >= 21 && game < 28) {

    digitalWrite(FOUR_LED, brightness);
  }
  else {
    digitalWrite(FOUR_LED, off);
  }

  if (game >= 28 && game < 35) {

    digitalWrite(FIVE_LED, brightness);
  }
  else {
    digitalWrite(FIVE_LED, off);
  }

  if (game >= 35) {
    previousGame = millis();
  }

}


void setNumber(int side, int displayNum) {
  int sideOffset = side * characterOffset * 2;
  int ones = (displayNum % 10);
  int tens = ((displayNum / 10) % 10);
  for (int k = 0; k < 28; k++) {
    toggleLocation(side, k + sideOffset, charLayout[tens][k]);
    toggleLocation(side, k + characterOffset + sideOffset, charLayout[ones][k]);
  }
  pixels.show();
}

void toggleLocation(int side, int loc, int ledState) {
  if (ledState == 0) {

    pixels.setPixelColor(loc, pixels.Color(0, 0, 0));
  } else {
    pixels.setPixelColor(loc, colorlist[side]);
  }
}

void setScore(int team, int val) {
  int teamScore = score[team];
  if (val == 0) {
    teamScore = 0;
  } else {
    teamScore += val;
    if (teamScore < 0) {
      teamScore = 0;

    }
  }
  score[team] = teamScore;
  setNumber(team, teamScore);
}
