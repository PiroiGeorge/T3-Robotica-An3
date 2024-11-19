T3-Robotica-An3


Description:
This project is a two-player reflex-based competitive game, designed to test and improve players' reaction speeds. Each player competes to achieve the highest score by pressing the button corresponding to the color displayed on their RGB LED. The game progresses through multiple rounds, and the player with the highest score at the end wins.

The project is implemented using two Arduino Uno boards, with one acting as the Master and the other as the Slave. The boards communicate using the SPI protocol, dividing responsibilities such as controlling LEDs, buttons, and game state management.

🛠️ Components:

Master Board:
1x LCD Display (16x2) with 50kΩ potentiometer for contrast adjustment
1x Servo Motor (indicates game progress)
1x Button (dedicated for starting the game)
1x Potetiometer
1x 10K-ohm Resistors
1x 100 ohm Resistors
Jumper Wires

Slave Board:
6x Individual LEDs (2 groups of 3 with distinct colors for each player)
2x RGB LEDs (1 for each player)
6x Buttons (3 for each player)
Multiplexed Resistors for button inputs
Breadboard
9x 100 ohm Resistors
3x 220 ohm Resistors
2x 1K-ohm Resistors
4x 10K-ohm Resistors
2x 100K-ohm Resistors
Jumper Wires



📖 Project Functionality:
1. Initialization
The LCD displays a welcome message with an animation during initialization.
The Servo motor starts in the 0-degree position.
2. Game Start
Pressing the start button triggers the game to begin, resetting all scores and displaying initial scores on the LCD.
3. Gameplay Rounds
Player Turn: Each player alternates turns as the active participant.
RGB LED Display: The active player's RGB LED lights up in a random color (Red, Green, or Blue).
Player Input: The player must press the button corresponding to the color displayed on the RGB LED:
Correct Input: Points awarded based on reaction time.
Incorrect Input: No points are awarded.
Individual LEDs: A matching individual LED lights up to indicate the pressed button.
Time Limit: Players have 3 seconds to respond; otherwise, no points are awarded.
Score Updates: Scores are updated on the LCD after every round.
4. Progress Tracking
The servo motor rotates incrementally after each round, indicating progress. A full rotation (180°) signifies the end of the game.
5. End of Game
After all rounds, the game announces the winner (or a tie) on the LCD.
The RGB LEDs and individual LEDs reset, and the servo motor returns to 0°.
The game returns to the idle state, displaying the welcome message.



⚙️ Circuit Schematic:
![Copy of Copy of Mighty Wluff (1)](https://github.com/user-attachments/assets/396bb140-a17a-46d0-bb26-932f4dbdebd3)


Simulation Picture:
![image](https://github.com/user-attachments/assets/4780f9eb-4b94-4dd4-8fb2-f3c1bb2baa89)

📹 Demo Video:

https://github.com/user-attachments/assets/dc9cc42e-d242-4dd7-84c2-4dc266290f25




