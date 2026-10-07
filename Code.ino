#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define TRIGGER_PIN_1 2
#define ECHO_PIN_1 3    
#define TRIGGER_PIN_2 5  
#define ECHO_PIN_2 4  

Servo myservo;
int buz = A5;

bool isOpen = false;
bool wasNear = false;
long maxBinHeight = 24; // Total height from sensor to bottom
unsigned long lastLidActionTime = 0;
const unsigned long LID_STABILIZE_DELAY = 1500; // Wait after lid movement

void setup() {
  Serial.begin(9600);
  myservo.attach(6);
  pinMode(buz, OUTPUT);
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);

  myservo.write(0);
  isOpen = false;

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME");
  lcd.setCursor(0, 1);
  lcd.print("SMART DUST BIN");
  delay(5000); // Changed from 2000 to 5000 (5 seconds)
  lcd.clear();
}

long readSensor(int triggerPin, int echoPin) {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) {
    return -1; // Error reading
  }
  
  long distance = duration * 0.034 / 2;
  if (distance < 2 || distance > 200) {
    return -1; 
  }
  
  return distance;
}

void loop() {
  long distance_1 = readSensor(TRIGGER_PIN_1, ECHO_PIN_1);
  
  long distance_2 = -1;
  int freePercentage = 0;
  
  if (millis() - lastLidActionTime > LID_STABILIZE_DELAY) {
    distance_2 = readSensor(TRIGGER_PIN_2, ECHO_PIN_2);
    
    if (distance_2 != -1) {
      freePercentage = (distance_2 * 100) / maxBinHeight;
      if (freePercentage > 100) freePercentage = 100;
    }
  }
  
  // Display
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("Lid:");
  if (isOpen) {
    lcd.print("OPEN  ");
  } else {
    lcd.print("CLOSED");
  }
  
  lcd.setCursor(0, 1); 
  if (millis() - lastLidActionTime <= LID_STABILIZE_DELAY) {
    lcd.print("Calibrating...  ");
  } else if (distance_2 == -1) {
    lcd.print("Sensor Error   ");
  } else {
    lcd.print("Free:");
    lcd.print(freePercentage);
    lcd.print("% ");
    
    // Adjust emojis for free percentage (happy when more free space)
    if (freePercentage > 50) {
      lcd.print(":)"); // Happy when more than 50% free
    } else if (freePercentage > 20) {
      lcd.print(":|"); // Neutral when 20-50% free
    } else {
      lcd.print(":("); // Sad when less than 20% free
    }
  }

  // Debug to Serial Monitor
  Serial.print("Sensor1: ");
  Serial.print(distance_1);
  Serial.print("cm, Sensor2: ");
  Serial.print(distance_2);
  Serial.print("cm, Free: ");
  Serial.print(freePercentage);
  Serial.println("%");

  // Lid control - only if we got a valid reading
  if (distance_1 != -1 && distance_1 < 30) {
    if (!isOpen) {
      myservo.write(140); // Open lid
      isOpen = true;
      lastLidActionTime = millis();
      digitalWrite(buz, HIGH);
      delay(100);
      digitalWrite(buz, LOW);
      Serial.println("Lid OPENED");
    }
    wasNear = true;
  } else {
    if (isOpen && wasNear) {
      myservo.write(0); // Close lid
      isOpen = false;
      lastLidActionTime = millis();
      digitalWrite(buz, HIGH);
      delay(100);
      digitalWrite(buz, LOW);
      Serial.println("Lid CLOSED");
    }
    wasNear = false;
  }

]  if (millis() - lastLidActionTime > LID_STABILIZE_DELAY && 
      distance_2 != -1 && 
      freePercentage <= 10) {
    
    digitalWrite(buz, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BIN FULL!");
    lcd.setCursor(0, 1);
    lcd.print("EMPTY PLEASE");
    Serial.println("*** BIN FULL ALERT ***");
    delay(1000);
    digitalWrite(buz, LOW);
    delay(500); // Additional delay to prevent rapid retriggering
  }

  delay(500);
}
