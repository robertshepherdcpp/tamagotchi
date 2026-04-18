/*
summary of game logic:
plan for the different components
button:
1. Feed luosifen = feed (hunger)
2. Climb mountain = play (happiness, and decrease energy)
3. Nap = sleep (energy increased, intelligence increased by 1)
4. study = NEW ONE (increase intelligence by 10)

also a new stat called intelligence

for the rotary encoder - after a random amount of time on the main screen, say 5 seconds, can be taken to a wild card,
where a random number is calculated from 1 to 20, if they get the number (they can select it using the buttons)
- if they get the number, the number bellow, the number or the number above then the score is doubled!
- otherwise the score is decreased by 15% (*0.85)


*/

// buttons
#define BTN1 0
#define BTN2 1
#define BTN3 2
#define BTN4 21

//OLED display
#define SDA_PIN 22
#define SCL_PIN 23

// buzzer
#define BZR 18

// rotary encoder
#define RE_A 20
#define RE_B 19

// includes, and setting up other stuff
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BTN_LEFT D0    // EAT LUOSIFEN
#define BTN_MIDDLE D1  // CLIMB A MOUNTAIN
#define BTN_RIGHT D2   // NAP
#define BTN_DOWN D3    // STUDY

#define BUZZER_PIN D10

// im not sure if this is correct, but did the same for buttons, so probably the same.
#define RotaryEncoderA D9
#define RotaryEncoderB D8

struct Pet {
  int hunger;         // 0 to 100 (0 = starving, 100 = full)
  int happiness;      // 0 to 100 (0 = miserable, 100 = ecstatic)
  int energy;         // 0 to 100 (0 = exhausted, 100 = fully rested)
  unsigned long age;  // total seconds the pet has been alive
  int intelligence;   // 0 to 100 (0 = dumb, 100 = mega brain)
};

// globals

Pet pet{};

int prev_CLK_state;  // checking if has changed, when using the rotary encoder
int count;           // measured of the rotary encoder. 1-20 (360 degrees and 18 degree increments)
int lastValue{};

enum Screen {
  SCREEN_MAIN,
  SCREEN_FEED,     // eat luosifen
  SCREEN_PLAY,     // climb mountain
  SCREEN_SLEEP,    // nap
  SCREEN_STUDY,    // study
  SCREEN_WILDCARD  // pick a random number between 1 and 4 if right .... (this is done if the user stays on the main screen for 10 seconds)
};

Screen currentScreen = SCREEN_MAIN;

void setup() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_MIDDLE, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);

  // TODO: also do for the rotary encoder, but probably wont be the same, do some research....
  pinMode(RotaryEncoderA, INPUT);  // seems like these dont need a pullup, but idk...
  pinMode(RotaryEncoderB, INPUT);

  prev_CLK_state = digitalRead(RotaryEncoderA);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("LUNA Init...");
  display.display();
  delay(1000);

  pet.hunger = 80;
  pet.happiness = 80;
  pet.energy = 80;
  pet.age = 0;
}

unsigned long lastUpdate = 0;

void updatePet() {
  if (millis() - lastUpdate > 5000) {  // every 5 seconds
    pet.hunger--;
    pet.happiness--;
    pet.energy--;

    if (pet.hunger < 0) pet.hunger = 0;
    if (pet.happiness < 0) pet.happiness = 0;
    if (pet.energy < 0) pet.energy = 0;

    pet.age += 5;
    lastUpdate = millis();
  }
}

unsigned long lastButtonPress = 0;

void checkButtons() {
  if (millis() - lastButtonPress < 200) return;  // debounce: ignore presses within 200ms

  if (digitalRead(BTN_LEFT) == LOW) {
    currentScreen = SCREEN_FEED;
    tone(BUZZER_PIN, 1000, 50);
    lastButtonPress = millis();
  } else if (digitalRead(BTN_MIDDLE) == LOW) {
    currentScreen = SCREEN_PLAY;
    tone(BUZZER_PIN, 1200, 50);
    lastButtonPress = millis();
  } else if (digitalRead(BTN_RIGHT) == LOW) {
    currentScreen = SCREEN_SLEEP;
    tone(BUZZER_PIN, 800, 50);
    lastButtonPress = millis();
  } else if (digitalRead(BTN_DOWN) == LOW) {  //i.e. its clicked
    currentScreen = SCREEN_WILDCARD;
    tone(BUZZER_PIN, 1400, 75);
    lastButtonPress = millis();
  }
}

void handleScreenLogic() {
  switch (currentScreen) {

    case SCREEN_FEED:
      pet.hunger += 10;
      pet.energy += 5;
      if (pet.energy > 100) pet.energy = 100;
      if (pet.hunger > 100) pet.hunger = 100;
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_PLAY:
      pet.happiness += 10;
      pet.energy -= 5;
      if (pet.happiness > 100) pet.happiness = 100;
      if (pet.energy < 0) pet.energy = 0;
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_SLEEP:
      pet.energy += 15;
      pet.intelligence += 10;
      if (pet.intelligence > 100) pet.intelligence = 100;
      if (pet.energy > 100) pet.energy = 100;
      currentScreen = SCREEN_MAIN;
      break;

    case SCREEN_WILDCARD:
    {
      // here we are going to check for the rotary encoder getting moved
      // CLK = A
      // DT = B

      // to check if there has been any change.
      lastValue = count;

      auto currentStateCLK = digitalRead(RotaryEncoderA);

      auto start = millis();

      while (millis() - start < 3000) {
        if (currentStateCLK != prev_CLK_state) {
          // Read the state of DT to determine rotation direction
          if (digitalRead(RotaryEncoderB) != currentStateCLK) {
            if (count == 19) {
              count = -1;  // which then gets incremented to 0.
            }
            count++;  // Clockwise rotation
          } else {
            if (count == 0) {
              count = 20;  // which then gets decremented to 19.
            }
            count--;  // Counterclockwise rotation
          }
        }
        prev_CLK_state = currentStateCLK;
      }

      // now we have got the count, we need to find a random number.
      int num = int(random(0, 20));
      // check if it is round about.
      if (count == (num - 1) || count == (num) || count == (num + 1)) {
        pet.happiness *= 2;
        if (pet.happiness > 100) {
          pet.happiness = 100;
        }
      } else {
        // they didnt get it, decrease by 15%
        pet.happiness = int(pet.happiness * 0.85);
      }
      currentScreen = SCREEN_MAIN;
    }
    break;

    case SCREEN_MAIN:
      break;  // do nothing, just display stats
  }
}

const unsigned char PROGMEM petHappy[] = {
  0b00000000, 0b00000000,
  0b00000001, 0b11110000,
  0b00000011, 0b00011000,
  0b00000110, 0b00001000,
  0b00001100, 0b00001000,
  0b00011001, 0b00100100,
  0b00010000, 0b00000100,
  0b00110000, 0b00000100,
  0b00100010, 0b00100100,
  0b00100011, 0b01100100,
  0b00100011, 0b11000100,
  0b00110000, 0b00001100,
  0b00011000, 0b00001000,
  0b00001100, 0b00011000,
  0b00000111, 0b11110000,
  0b00000000, 0b00000000
};

// Usage:
// display.drawBitmap(x, y, sprite, 16, 16, SSD1306_WHITE);

// Example: Sad face (any stat below 30)
const unsigned char PROGMEM petSad[] = {
  0b00000000, 0b00000000,
  0b00000001, 0b11110000,
  0b00000011, 0b00011000,
  0b00000110, 0b00001000,
  0b00001100, 0b00001000,
  0b00011001, 0b00100100,
  0b00010000, 0b00000100,
  0b00110000, 0b00000100,
  0b00100011, 0b11000100,
  0b00100110, 0b01100100,
  0b00100100, 0b00100100,
  0b00110000, 0b00001100,
  0b00011000, 0b00001000,
  0b00001100, 0b00011000,
  0b00000111, 0b11110000,
  0b00000000, 0b00000000
};

// Usage:
// display.drawBitmap(x, y, sprite, 16, 16, SSD1306_WHITE);

const unsigned char PROGMEM petNeutral[] = {
  0b00000000, 0b00000000,
  0b00000001, 0b11110000,
  0b00000011, 0b00011000,
  0b00000110, 0b00001000,
  0b00001100, 0b00001000,
  0b00011001, 0b00100100,
  0b00010000, 0b00000100,
  0b00110000, 0b00000100,
  0b00100000, 0b00000100,
  0b00100111, 0b11100100,
  0b00100000, 0b00000100,
  0b00110000, 0b00001100,
  0b00011000, 0b00001000,
  0b00001100, 0b00011000,
  0b00000111, 0b11110000,
  0b00000000, 0b00000000
};

//pet sleep
const unsigned char PROGMEM sprite[] = {
  0b00000000, 0b00000000,
  0b00000001, 0b11110000,
  0b00000011, 0b00011000,
  0b00000110, 0b00001000,
  0b00001100, 0b00001000,
  0b00011000, 0b00000100,
  0b00010000, 0b00000100,
  0b00110000, 0b00000100,
  0b00100000, 0b00000100,
  0b00100011, 0b11100100,
  0b00100000, 0b00000100,
  0b00110000, 0b00001100,
  0b00011000, 0b00001000,
  0b00001100, 0b00011000,
  0b00000111, 0b11110000,
  0b00000000, 0b00000000
};

// Usage:
// display.drawBitmap(x, y, sprite, 16, 16, SSD1306_WHITE);

void render() {
  display.clearDisplay();

  // Choose the right sprite based on pet stats
  const unsigned char* sprite;
  if (pet.hunger < 30 || pet.happiness < 30 || pet.energy < 30) {
    sprite = petSad;
  } else if (pet.hunger > 50 && pet.happiness > 50 && pet.energy > 50) {
    sprite = petHappy;
  } else {
    sprite = petNeutral;
  }

  // Draw the pet sprite (centered horizontally, near the top)
  display.drawBitmap(56, 2, sprite, 16, 16, SSD1306_WHITE);

  // Draw stat bars below the pet
  display.setTextSize(1);

  display.setCursor(0, 24);
  display.print("HUN ");
  drawBar(24, 24, pet.hunger);

  display.setCursor(0, 30);
  display.print("HAP ");
  drawBar(24, 30, pet.happiness);

  display.setCursor(0, 36);
  display.print("ENG ");
  drawBar(24, 36, pet.energy);

  display.setCursor(0, 42);
  display.print("INTG ");
  drawBar(24, 42, pet.intelligence);

  // should we print out the age?
  Serial.println(pet.age);  // just print it out to the console for now.

  // Button labels at the bottom
  display.setCursor(0, 56);
  display.println("[LSF] [CLMB] [SJ] [WLDCRD]");

  display.display();
}

// Draws a stat bar: empty rectangle with a filled portion based on value (0 to 100)
void drawBar(int x, int y, int value) {
  int barWidth = 100;
  int barHeight = 6;
  int fillWidth = map(value, 0, 100, 0, barWidth);

  display.drawRect(x, y, barWidth, barHeight, SSD1306_WHITE);   // outline
  display.fillRect(x, y, fillWidth, barHeight, SSD1306_WHITE);  // filled portion
}

void loop() {
  checkButtons();       // 1. Read input
  updatePet();          // 2. Update state over time
  handleScreenLogic();  // 3. Process actions
  render();             // 4. Display results
  delay(100);           // 5. Short pause
}
