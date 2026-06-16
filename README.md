# Smart Automated Dustbin Using Arduino

This project implements a touchless smart dust bin using Arduino, ultrasonic sensors, a servo motor, an LCD, and a buzzer to automate lid opening and monitor bin fill level.

---

## About the Project

The Smart Dust Bin is designed to open its lid automatically when a user’s hand or object is detected near the bin and to display how much free space is left inside the bin. 
It uses one ultrasonic sensor to detect a nearby user and another ultrasonic sensor to measure the distance to the trash level, calculating the percentage of free space and showing it on a 16x2 LCD along with simple emoji-style feedback. 

This is an embedded systems mini-project suitable for academic demonstration and practical automation applications, promoting hygiene by avoiding direct contact with the dust bin lid. 

---

## Hardware Components

- Arduino board (e.g., Arduino Uno)  
- Ultrasonic Sensor 1 (for user detection)  
- Ultrasonic Sensor 2 (for bin level detection)  
- Servo motor (for lid control)  
- 16x2 LCD display (LiquidCrystal)  
- Buzzer (connected to analog pin A5 in the code)  
- Connecting wires and breadboard  
- Power supply (USB or external 5V)  

The pin mapping follows a typical smart dustbin setup with ultrasonic sensors, servo, and LCD connected to Arduino digital pins. [web:20][web:22][web:25]

---

## Pin Configuration

From the code:

- LCD:
  - `rs` → pin 8  
  - `en` → pin 9  
  - `d4` → pin 10  
  - `d5` → pin 11  
  - `d6` → pin 12  
  - `d7` → pin 13  

- Ultrasonic Sensor 1 (for user detection):
  - `TRIGGER_PIN_1` → pin 2  
  - `ECHO_PIN_1` → pin 3  

- Ultrasonic Sensor 2 (for bin level):
  - `TRIGGER_PIN_2` → pin 5  
  - `ECHO_PIN_2` → pin 4  

- Servo:
  - Servo signal → pin 6 (attached in code)  

- Buzzer:
  - `buz` → pin A5  

---

## Features

- **Automatic lid opening**  
  Uses Ultrasonic Sensor 1 to detect an object within 30 cm, and opens the lid using a servo motor. [web:16][web:20][web:22][web:25]

- **Automatic lid closing**  
  When the user moves away, the lid closes automatically and a short buzzer beep is triggered to indicate lid actions.

- **Bin fill level monitoring**  
  Ultrasonic Sensor 2 measures the distance from the sensor to the trash level and computes the free space percentage based on a configurable maximum bin height (`maxBinHeight`). 

- **LCD status display**  
  The 16x2 LCD shows:
  - Current lid status: `OPEN` or `CLOSED`  
  - Free space percentage in the bin  
  - Emoji-like feedback:
    - `:)` when more than 50% free  
    - `:|` when between 20% and 50% free  
    - `:(` when less than 20% free  

- **Bin full alert**  
  When free space falls to 10% or less, the system:
  - Activates the buzzer  
  - Displays `BIN FULL!` and `EMPTY PLEASE` on the LCD  
  - Prints a “BIN FULL ALERT” message on the Serial Monitor  

- **Calibration / stability logic**  
  After any lid movement, the system waits for a stabilization delay (`LID_STABILIZE_DELAY`) before reading the level sensor to avoid false readings due to motion. 

- **Serial debugging**  
  Distance readings from both sensors and free space percentage are printed to the Serial Monitor for debugging.

---
<p align="center">
  <img src="https://github.com/user-attachments/assets/7089f13f-24dc-469d-ae85-9516eb6b6ead"
       alt="Smart Automated Dustbin"
       width="500">
</p>
---

## How the Code Works

1. **Initialization (`setup`)**  
   - Initializes the LCD, servo, buzzer, and ultrasonic sensor pins.  
   - Sets the lid to the closed position (`myservo.write(0)` and `isOpen = false`).  
   - Displays a welcome message “WELCOME SMART DUST BIN” on the LCD for 5 seconds.

2. **Distance measurement (`readSensor`)**  
   - Sends a 10 µs trigger pulse to the ultrasonic sensor.  
   - Uses `pulseIn` to measure the echo pulse duration with a timeout of 30 ms.  
   - Converts the duration to distance in centimeters.  
   - Returns `-1` if the reading is invalid or out of range (less than 2 cm or greater than 200 cm).

3. **User detection and lid control**  
   - Reads distance from Sensor 1.  
   - If a valid reading is less than 30 cm and the lid is closed:
     - Opens the lid (`myservo.write(140)`), sets `isOpen = true`, updates `lastLidActionTime`, and gives a short buzzer beep.  
   - When the user is no longer near and the lid was open:
     - Closes the lid (`myservo.write(0)`), sets `isOpen = false`, updates `lastLidActionTime`, and gives another beep.

4. **Bin level measurement and free space calculation**  
   - After the stabilization delay from the last lid action, reads distance from Sensor 2.  
   - If valid, computes:
     \[
     \text{freePercentage} = \frac{\text{distance\_2} \times 100}{\text{maxBinHeight}}
     \]
     and caps it at 100.  
   - Displays this percentage and the corresponding emoji on the LCD.

5. **Bin full detection**  
   - If:
     - Enough time has passed since the last lid action,
     - Sensor 2 reading is valid, and
     - `freePercentage <= 10`,  

     then it:
     - Activates the buzzer,  
     - Displays “BIN FULL!” and “EMPTY PLEASE” on the LCD,  
     - Logs `*** BIN FULL ALERT ***` over Serial,  
     - Adds small delays to avoid rapid retriggering.

6. **Loop delay**  
   - A `delay(500)` at the end to reduce flickering and constant triggering.

---

## Getting Started

### Prerequisites

- Arduino IDE installed. [web:20][web:22]  
- Correct board selected (e.g., Arduino Uno) and COM port configured in the Arduino IDE. [web:22][web:23]  
- All hardware components wired according to the pin configuration section.

### Installation and Upload

1. Connect your Arduino board to your PC via USB.  
2. Open the Arduino IDE.  
3. Create a new sketch and paste the provided code.  
4. Go to:
   - **Tools → Board** → select your board (e.g., Arduino Uno).  
   - **Tools → Port** → select the correct COM port.  
5. Click the **Upload** button to flash the code to the Arduino. 

---

## Usage

- Power up the Arduino and dust bin circuit.  
- On startup, the LCD will show the welcome message and then switch to real-time status.  
- Bring your hand or an object close to the front sensor:
  - The lid should automatically open.
  - You will hear a short beep.  
- Move your hand away:
  - The lid closes automatically and beeps again.  
- As the bin fills, observe the “Free %” and emoji on the LCD.  
- When the bin is almost full (≤ 10% free), the buzzer will sound and the LCD will display the “BIN FULL!” message.

---

## Future Improvements

- Add an IoT module (e.g., ESP8266) to send bin status to a cloud dashboard or mobile app. 
- Add a real-time clock (RTC) to log when the bin becomes full.  
- Use a battery-powered or solar-powered version for outdoor usage.  
- Implement a more advanced filtering or averaging of sensor readings to reduce noise.

---

## Author

- Name: Goutham  
- Role: Undergraduate engineering student / embedded systems enthusiast  
- Project Type: Academic mini-project / embedded systems project  
