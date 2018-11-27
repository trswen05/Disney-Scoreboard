#include <Adafruit_NeoPixel.h>

const int HOMEA = 2; //identify pin
const int HOMEB = 3; //identify pin
const int AWAYA = 4; //identify pin
const int AWAYB = 5; //identify pin
const int RESCORE = 6; //identify pin
const int PERIODA = 7; //identify pin
const int PERIODB = 8; //identify pin
const int PERIODC = 9; //identify pin
const int PERIODD = 10; //identify pin
#define PIXEL_PIN 11 //identify pin
#define NUMPIXELS  60

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_RGB + NEO_KHZ800);

int buttonState;
int previous = HIGH;
int val = 0;

uint32_t colorlist[] = {pixels.Color(255, 0, 0), pixels.Color(0, 0, 255)};
const int characterOffset = 15;
int score[2] = {0, 0};

int charLayout[10][15] = {
  {1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}, //0
  {1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1}, //1
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1}, //2
  {1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //3
  {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1}, //4
  {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1}, //5
  {1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, //6
  {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1}, //7
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1} //9
}
;
void setup() {
  pinMode(HOMEA, INPUT);
  pinMode(HOMEB, INPUT);
  pinMode(AWAYA, INPUT);
  pinMode(AWAYB, INPUT);
  pinMode(RESCORE, INPUT);
  pinMode(PERIODA, INPUT);
  pinMode(PERIODB, INPUT);
  pinMode(PERIODC, INPUT);
  pinMode(PERIODD, INPUT);
  pinMode(PIXEL_PIN, OUTPUT);
  digitalWrite(HOMEA, HIGH);
  digitalWrite(HOMEB, HIGH);
  digitalWrite(AWAYA, HIGH);
  digitalWrite(AWAYB, HIGH);
  digitalWrite(RESCORE, HIGH);

  pixels.begin();
  pixels.clear();
  pixels.show();

  setNumber(0, 00);
  setNumber(1, 00);
}

void loop() {

  { buttonState = digitalRead(HOMEB);


    if (buttonState == LOW && previous == HIGH ) {
      setScore(0, 1);
      delay(500);
      previous == buttonState;
    }

    { buttonState = digitalRead(HOMEA);

      if (buttonState == LOW && previous == HIGH ) {
        setScore(0, -1);
        delay(500);
        previous == buttonState;
      }
    }

    { buttonState = digitalRead(AWAYA);

      if (buttonState == LOW && previous == HIGH) {
        setScore(1, -1);
        delay(500);
        previous == buttonState;
      }
    }
    { buttonState = digitalRead(AWAYB);

      if (buttonState == LOW && previous == HIGH) {
        setScore(1, 1);
        delay(500);
        previous == buttonState;
      }
    }

    { buttonState = digitalRead(RESCORE);
      if (buttonState == LOW && previous == HIGH) {
        setScore(0, -100);
        setScore(1, -100);
        delay(500);
        previous == buttonState;
      }
    }

  }
}




void setNumber(int side, int displayNum) {
  int sideOffset = side * characterOffset * 2;
  int ones = (displayNum % 10);
  int tens = ((displayNum / 10) % 10);
  for (int k = 0; k < 15; k++) {
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
