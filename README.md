# 🗑️ Smart Automated Dustbin Using Arduino

> A touchless smart dustbin that automatically opens its lid, monitors bin fill level, and provides real-time status feedback using Arduino and ultrasonic sensors.

---

## 📌 About the Project

The **Smart Automated Dustbin** is an embedded systems project designed to provide **touchless waste disposal** and **automatic bin-level monitoring**.

The system uses:

* **Ultrasonic Sensor 1** → Detects a user's hand/object near the dustbin.
* **Servo Motor** → Automatically opens and closes the lid.
* **Ultrasonic Sensor 2** → Measures the distance to the trash level.
* **16x2 LCD** → Displays lid status, free-space percentage, and feedback.
* **Buzzer** → Provides audible alerts for lid actions and a nearly full bin.

The system calculates the **percentage of free space** remaining inside the bin and displays it on the LCD using simple emoji-style indicators.

This project demonstrates practical applications of **Arduino, sensors, actuator control, LCD interfacing, and embedded automation** while promoting better hygiene through touchless operation.

---

## ✨ Features

### 🤖 Automatic Lid Opening

* Ultrasonic Sensor 1 detects an object within **30 cm**.
* Servo motor rotates the lid to the open position.
* A short buzzer beep indicates the lid action.

### 🔒 Automatic Lid Closing

* When the detected object moves away, the lid automatically closes.
* A short buzzer beep confirms the closing action.

### 📊 Bin Fill-Level Monitoring

* Ultrasonic Sensor 2 measures the distance between the sensor and the trash.
* The system calculates the remaining free space.
* Free-space percentage is displayed on the LCD.

### 🖥️ LCD Status Display

The 16x2 LCD displays:

| Free Space    | Feedback |
| ------------- | -------- |
| **> 50%**     | `:)`     |
| **20% – 50%** | `:\|`    |
| **< 20%**     | `:(`     |

The LCD also displays the current lid status:

* `OPEN`
* `CLOSED`

### 🚨 Bin Full Alert

When free space reaches **10% or less**:

* Buzzer is activated.
* LCD displays `BIN FULL!`
* LCD displays `EMPTY PLEASE`
* Serial Monitor prints `*** BIN FULL ALERT ***`

### ⚙️ Sensor Stability Logic

After lid movement, the system waits for `LID_STABILIZE_DELAY` before measuring the bin level.

This prevents inaccurate readings caused by temporary movement or vibration.

### 🖥️ Serial Debugging

The Serial Monitor displays:

* Sensor 1 distance
* Sensor 2 distance
* Free-space percentage
* Bin-full alerts

---

## 🧰 Hardware Components

| Component                         | Purpose                    |
| --------------------------------- | -------------------------- |
| Arduino board (e.g., Arduino Uno) | Main controller            |
| Ultrasonic Sensor 1               | User/object detection      |
| Ultrasonic Sensor 2               | Bin fill-level measurement |
| Servo Motor                       | Lid control                |
| 16x2 LCD                          | Status display             |
| Buzzer                            | Audible alerts             |
| Connecting wires                  | Circuit connections        |
| Breadboard                        | Prototyping                |
| USB / External 5V supply          | Power                      |

---

## 🔌 Pin Configuration

### LCD — `LiquidCrystal`

| LCD Pin | Arduino Pin |
| ------- | ----------: |
| `rs`    |           8 |
| `en`    |           9 |
| `d4`    |          10 |
| `d5`    |          11 |
| `d6`    |          12 |
| `d7`    |          13 |

### Ultrasonic Sensor 1 — User Detection

| Pin             | Arduino Pin |
| --------------- | ----------: |
| `TRIGGER_PIN_1` |           2 |
| `ECHO_PIN_1`    |           3 |

### Ultrasonic Sensor 2 — Bin Level

| Pin             | Arduino Pin |
| --------------- | ----------: |
| `TRIGGER_PIN_2` |           5 |
| `ECHO_PIN_2`    |           4 |

### Servo Motor

| Connection   | Arduino Pin |
| ------------ | ----------: |
| Servo Signal |           6 |

### Buzzer

| Connection | Arduino Pin |
| ---------- | ----------: |
| `buz`      |          A5 |

---

## 🖼️ Project Preview

<p align="center">
  <img
    src="https://github.com/user-attachments/assets/a8a1a5d8-bf04-4d9a-a38a-88a011fe007b"
    alt="Smart Automated Dustbin"
    width="450"
  />
</p>

---

## ⚙️ How It Works

### 1. Initialization — `setup()`

During startup, the system:

* Initializes the LCD.
* Initializes the servo.
* Initializes the buzzer.
* Configures ultrasonic sensor pins.
* Sets the lid to the closed position:

```cpp
myservo.write(0);
isOpen = false;
```

* Displays:

```text
WELCOME SMART DUST BIN
```

for **5 seconds**.

---

### 2. Distance Measurement — `readSensor()`

The ultrasonic sensor:

1. Receives a **10 µs trigger pulse**.
2. Measures the returning echo using `pulseIn`.
3. Uses a **30 ms timeout**.
4. Converts echo duration into distance in centimeters.
5. Returns `-1` for invalid readings.

Valid distance range:

```text
2 cm ≤ distance ≤ 200 cm
```

---

### 3. User Detection & Lid Control

Sensor 1 continuously checks for a nearby object.

If:

```text
distance < 30 cm
```

and the lid is closed:

```cpp
myservo.write(140);
isOpen = true;
```

The system:

* Opens the lid.
* Updates `lastLidActionTime`.
* Produces a short buzzer beep.

When the user moves away:

```cpp
myservo.write(0);
isOpen = false;
```

The lid closes and another beep is generated.

---

### 4. Bin-Level Measurement

After the stabilization delay, Sensor 2 measures the distance to the trash.

The free-space percentage is calculated using:

$$
\text{freePercentage}
=
\frac{\text{distance\_2} \times 100}
{\text{maxBinHeight}}
$$

The result is limited to a maximum of **100%**.

---

### 5. Bin-Full Detection

The bin is considered nearly full when:

```text
freePercentage <= 10
```

provided that:

* The lid stabilization period has completed.
* Sensor 2 provides a valid reading.

The system then:

* Activates the buzzer.
* Displays `BIN FULL!`.
* Displays `EMPTY PLEASE`.
* Prints:

```text
*** BIN FULL ALERT ***
```

to the Serial Monitor.

---

### 6. Loop Delay

A:

```cpp
delay(500);
```

is used at the end of the loop to reduce:

* LCD flickering.
* Excessive sensor triggering.
* Rapid repeated alerts.

---

## 🚀 Getting Started

### Prerequisites

* Arduino IDE
* Compatible Arduino board such as **Arduino Uno**
* Correct COM port
* All hardware connected according to the pin configuration

### Installation & Upload

1. Connect the Arduino board to the PC using USB.

2. Open **Arduino IDE**.

3. Create a new sketch.

4. Paste the project code.

5. Select:

   **Tools → Board → Arduino Uno**

6. Select:

   **Tools → Port → COM Port**

7. Click **Upload**.

---

## 🧪 Usage

### Step 1 — Power On

Power the Arduino and dustbin circuit.

The LCD initially displays:

```text
WELCOME SMART DUST BIN
```

### Step 2 — Approach the Dustbin

Place your hand or an object within **30 cm** of the front ultrasonic sensor.

Expected behavior:

```text
Object detected
      ↓
Servo activates
      ↓
Lid opens
      ↓
Buzzer beeps
```

### Step 3 — Move Away

When the object moves away:

```text
Object absent
      ↓
Servo activates
      ↓
Lid closes
      ↓
Buzzer beeps
```

### Step 4 — Monitor Fill Level

As waste accumulates, Sensor 2 measures the remaining free space.

The LCD displays:

```text
Free: XX%
```

along with the corresponding feedback symbol.

### Step 5 — Bin Full Alert

When free space reaches **10% or less**:

```text
BIN FULL!
EMPTY PLEASE
```

The buzzer also provides an audible warning.

---

## 🔄 System Flow

```text
                 ┌───────────────────┐
                 │   Arduino UNO     │
                 └─────────┬─────────┘
                           │
              ┌────────────┴────────────┐
              │                         │
              ▼                         ▼
     Ultrasonic Sensor 1       Ultrasonic Sensor 2
       User Detection            Bin-Level Detection
              │                         │
              ▼                         ▼
        Servo Motor             Free-Space Calculation
              │                         │
              ▼                         ▼
         Lid Control              16x2 LCD Display
                                        │
                                        ▼
                                  Buzzer Alert
```

---

## 🔮 Future Improvements

* Add an **ESP8266/ESP32 IoT module** for remote bin monitoring.
* Send bin-level information to a **cloud dashboard or mobile application**.
* Add an **RTC module** to record when the bin becomes full.
* Develop a **battery-powered or solar-powered version**.
* Implement sensor **filtering and averaging** to reduce measurement noise.
* Add wireless notifications when the bin requires emptying.

---

## 👨‍💻 Author

**Goutham Vinjamuri**

**B.Tech – Electronics and Communication Engineering**
**Vellore Institute of Technology – Andhra Pradesh**

### 🔗 Connect

* **LinkedIn:** https://www.linkedin.com/in/goutham-vinjamuri-902787326/

---

⭐ **An Arduino-based embedded automation project for touchless waste disposal and real-time bin monitoring.**
