# guessGame
🎯 guessGame
A simple number-guessing game implemented with Arduino Uno, featuring a keypad, I²C LCD display, real-time clock module (RTC), and a buzzer. The game logic is coded in C++ (Arduino IDE), and a companion Python script logs serial output to the console.

🔧 Hardware Requirements
You’ll need the following components:

Arduino Uno

4×4 keypad (e.g. Matrix Keypad)

I²C LCD (e.g. 16×2 or 20×4)

I²C RTC module (e.g. DS1307, DS3231)

Buzzer (active or passive)

Jumper wires

Breadboard or prototyping board

📂 Repository Contents
guessGame.ino – Main Arduino sketch

listen_serial.py – Python script to read and log serial output

listen_serial.pyproj – Project file (optional; for editors like VS Code)

README.md – This documentation

🚀 How It Works
Setup

Initializes the I²C LCD and RTC module

Generates a random number to be guessed

Game Flow

Player enters guesses via keypad

LCD shows feedback: “Higher”, “Lower”, or “Correct!”

Buzzer beeps on correct guess

RTC logs the time of correct guess

Serial Logging

After a correct guess, the Arduino sends date/time + guess to serial

listen_serial.py displays this info in real-time

🧩 Wiring Guide
Component	Connection
LCD SDA → A4 (SDA)	SDA
LCD SCL → A5 (SCL)	SCL
RTC SDA → A4 (SDA)	SDA (shared)
RTC SCL → A5 (SCL)	SCL (shared)
Keypad pins → Digital I/O (e.g. 2–9)	Row/Column pins
Buzzer → Digital Pin (e.g. 10)	GND (–) and PIN (+)

👉 Adjust pin assignments in the Arduino code if your setup differs.

⚙️ Quick Start
Install required Arduino libraries:

text
Kopyala
Düzenle
- LiquidCrystal_I2C
- RTClib (or DS1307/DS3231 library)
Connect hardware as per the wiring guide.

Flash guessGame.ino to your Arduino.

Run listen_serial.py (Python 3 required):

bash
Kopyala
Düzenle
pip install pyserial
python listen_serial.py
Play the game by entering guesses on the keypad.

🧠 Notes & Tips
Ensure the RTC module’s battery is fresh so date/time remains accurate.

Modify the random number range in the code for varying difficulty.

Customize buzzer behavior by editing frequency/duration in tone() calls.

🛠️ Potential Enhancements
Add a fixed number of attempts with a “game over” condition.

Log game history (e.g. attempts count, timestamps) to EEPROM or SD card.

Add a menu option to reset the RTC or change settings.

Implement different difficulty levels selectable via keypad.
