//Master

#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD Pin Configuration
#define LCD_RS 3
#define LCD_EN 2
#define LCD_D4 A2
#define LCD_D5 A3
#define LCD_D6 9
#define LCD_D7 8

// Start Button Pin
#define START_BUTTON_PIN 4

// Servo Motor Pin
#define SERVO_PIN 7

// I2C Address for Slave
#define SLAVE_ADDRESS 8

// Game State
int player1Score = 0;
int player2Score = 0;
int activePlayer = 1;
int currentRound = 0;
int totalRounds = 5;
bool gameStarted = false;

// Function Prototypes
void displayStartupAnimation();
void displayScores();
void moveServoForRound();
void resetServo();
void startGame();
void sendCommand(char command, int value);
void playRound();
void endGame();

// Initialize LCD and Servo
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Servo servo;

void setup() {
  lcd.begin(16, 2);
  displayStartupAnimation();

  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  servo.attach(SERVO_PIN);
  servo.write(0); // Reset servo to the initial position

  Wire.begin(); // Start I2C communication
  Serial.begin(9600);
  Serial.println("Master Initialized");
}

void displayStartupAnimation() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");

  lcd.setCursor(0, 1);
  for (int i = 0; i < 16; i++) {
    lcd.print("-");
    delay(100);
  }

  delay(500);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Welcome to");
  lcd.setCursor(4, 1);
  lcd.print("the Game!");
  delay(1500);
  lcd.clear();
}

void displayScores() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("P1: ");
  lcd.print(player1Score);
  lcd.setCursor(8, 0);
  lcd.print("P2: ");
  lcd.print(player2Score);
}

void moveServoForRound() {
  int position = map(currentRound, 0, totalRounds, 0, 180);
  servo.write(position);
  delay(500); // Allow servo to move
}

void resetServo() {
  servo.write(0); // Reset servo to initial position
  delay(500);
}

void startGame() {
  lcd.clear();
  lcd.print("Game Started!");
  delay(1000);
  lcd.clear();
  displayScores();
  gameStarted = true;
  Serial.println("Game Started");
}

void sendCommand(char command, int value) {
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(command);
  Wire.write(value);
  Wire.endTransmission();
  Serial.print("Sent Command: ");
  Serial.print(command);
  Serial.print(" with Value: ");
  Serial.println(value);
}

void playRound() {
  sendCommand('P', activePlayer);
  int colorCode = random(1, 4);
  sendCommand('C', colorCode);
  delay(100);

  unsigned long startTime = millis();
  bool correctPress = false;

  while (!correctPress && (millis() - startTime < 3000)) {
    Wire.requestFrom(SLAVE_ADDRESS, 2);
    if (Wire.available() >= 2) {
      int points = Wire.read();
      int correctness = Wire.read();
      Serial.print("Received Points: ");
      Serial.print(points);
      Serial.print(", Correctness: ");
      Serial.println(correctness);
      if (correctness == 1) {
        correctPress = true;
        if (activePlayer == 1) {
          player1Score += points;
        } else {
          player2Score += points;
        }
      }
    }
  }

  displayScores();
  moveServoForRound();
  activePlayer = (activePlayer == 1) ? 2 : 1;
  currentRound++;

  if (currentRound >= totalRounds) {
    endGame();
  }
}

void endGame() {
  lcd.clear();
  lcd.print("Game Over!");
  delay(1000);
  lcd.clear();
  if (player1Score > player2Score) {
    lcd.print("Winner: P1");
  } else if (player2Score > player1Score) {
    lcd.print("Winner: P2");
  } else {
    lcd.print("It's a Tie!");
  }
  delay(3000);

  // Send a command to turn off all RGB LEDs
  sendCommand('R', 0);

  gameStarted = false;
  currentRound = 0;
  player1Score = 0;
  player2Score = 0;
  resetServo();
  lcd.clear();
  lcd.print("Welcome to Game");
}

void loop() {
  if (!gameStarted) {
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      delay(100); // Debounce
      if (digitalRead(START_BUTTON_PIN) == LOW) {
        startGame();
      }
    }
  } else {
    playRound();
  }
}
