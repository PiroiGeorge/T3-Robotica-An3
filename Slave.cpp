//Slave


#include <Wire.h>

// RGB LED Pins for Player 1
#define RGB1_RED 7
#define RGB1_GREEN 6
#define RGB1_BLUE 5

// RGB LED Pins for Player 2
#define RGB2_RED 11
#define RGB2_GREEN 12
#define RGB2_BLUE 13

// Individual LEDs for Player 1
#define LED1_RED 4
#define LED2_GREEN 3
#define LED3_BLUE 2

// Individual LEDs for Player 2
#define LED4_RED 9
#define LED5_GREEN 8
#define LED6_BLUE 10

// Player 1 Buttons (Multiplexed)
#define BUTTON_PLAYER1 A0

// Player 2 Buttons (Multiplexed)
#define BUTTON_PLAYER2 A1

int activePlayer = 0;
int colorCode = 0;
bool waitingForInput = false;
unsigned long reactionStartTime = 0;

// Function to set RGB LED colors
void setRGB(int red, int green, int blue, int player) {
  if (player == 1) {
    digitalWrite(RGB1_RED, red);
    digitalWrite(RGB1_GREEN, green);
    digitalWrite(RGB1_BLUE, blue);
  } else if (player == 2) {
    digitalWrite(RGB2_RED, red);
    digitalWrite(RGB2_GREEN, green);
    digitalWrite(RGB2_BLUE, blue);
  }
}

// Function to control individual LEDs
void setIndividualLED(int red, int green, int blue, int player) {
  if (player == 1) {
    digitalWrite(LED1_RED, red);
    digitalWrite(LED2_GREEN, green);
    digitalWrite(LED3_BLUE, blue);
  } else if (player == 2) {
    digitalWrite(LED4_RED, red);
    digitalWrite(LED5_GREEN, green);
    digitalWrite(LED6_BLUE, blue);
  }
}

// Function to reset all RGB LEDs
void resetRGB() {
  setRGB(LOW, LOW, LOW, 1);
  setRGB(LOW, LOW, LOW, 2);
}

// Function to reset all individual LEDs
void resetIndividualLEDs() {
  setIndividualLED(LOW, LOW, LOW, 1);
  setIndividualLED(LOW, LOW, LOW, 2);
}

// Function to read button press from an analog pin
int readButton(int analogPin) {
  int value = analogRead(analogPin);
  Serial.print("Analog Read on Pin ");
  Serial.print(analogPin);
  Serial.print(": ");
  Serial.println(value);

  if (value > 850) return 3; // Blue button
  if (value > 400 && value < 600) return 2; // Green button
  if (value > 70 && value < 120) return 1;  // Red button
  return 0;                  // No button pressed
}

// Handle commands received from the Master
void receiveEvent(int numBytes) {
  while (Wire.available() >= 2) {
    char command = Wire.read();
    int value = Wire.read();

    if (command == 'P') {
      resetRGB(); // Turn off any active RGB LEDs when switching players
      resetIndividualLEDs();
      activePlayer = value;
      Serial.print("Active Player: ");
      Serial.println(activePlayer);
    } else if (command == 'C') {
      colorCode = value;
      reactionStartTime = millis();
      waitingForInput = true;
      Serial.print("Color Code: ");
      Serial.println(colorCode);

      // Set the RGB LED color for the active player
      if (colorCode == 1) setRGB(HIGH, LOW, LOW, activePlayer);  // Red
      if (colorCode == 2) setRGB(LOW, HIGH, LOW, activePlayer);  // Green
      if (colorCode == 3) setRGB(LOW, LOW, HIGH, activePlayer);  // Blue
    } else if (command == 'R') {
      resetRGB(); // Turn off all RGB LEDs
      resetIndividualLEDs();
      Serial.println("Resetting RGB and Individual LEDs");
    }
  }
}

// Send response back to Master
void sendResponse() {
  int points = 0;
  int correctness = 0;

  if (waitingForInput) {
    int pressedButton = (activePlayer == 1) ? readButton(BUTTON_PLAYER1) : readButton(BUTTON_PLAYER2);

    Serial.print("Pressed Button: ");
    Serial.println(pressedButton);

    // Light up individual LED corresponding to the pressed button
    if (pressedButton == 1) setIndividualLED(HIGH, LOW, LOW, activePlayer);  // Red
    if (pressedButton == 3) setIndividualLED(LOW, HIGH, LOW, activePlayer);  // Green
    if (pressedButton == 2) setIndividualLED(LOW, LOW, HIGH, activePlayer);  // Blue

    if (pressedButton == colorCode) {
      correctness = 1;
      points = max(0, 1000 - (millis() - reactionStartTime) / 10);
      waitingForInput = false;
    }
  }

  Wire.write(points);
  Wire.write(correctness);

  if (!waitingForInput) {
    resetRGB();
    resetIndividualLEDs();
  }
}

void setup() {
  Serial.begin(9600);

  // Setup RGB LEDs for both players
  pinMode(RGB1_RED, OUTPUT);
  pinMode(RGB1_GREEN, OUTPUT);
  pinMode(RGB1_BLUE, OUTPUT);

  pinMode(RGB2_RED, OUTPUT);
  pinMode(RGB2_GREEN, OUTPUT);
  pinMode(RGB2_BLUE, OUTPUT);

  // Setup individual LEDs for both players
  pinMode(LED1_RED, OUTPUT);
  pinMode(LED2_GREEN, OUTPUT);
  pinMode(LED3_BLUE, OUTPUT);

  pinMode(LED4_RED, OUTPUT);
  pinMode(LED5_GREEN, OUTPUT);
  pinMode(LED6_BLUE, OUTPUT);

  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendResponse);

  Serial.println("Slave Initialized");
}

void loop() {
  // Logic handled by I2C events
}
