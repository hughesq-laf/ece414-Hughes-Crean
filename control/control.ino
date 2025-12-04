#include <Servo.h>
#include "Computation.h"
#include "hcsr04.h"

// -------- Pin Definitions --------
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 6 //PWM Pins for motor.
#define ENB 5

#define SERVO_PIN 18  // Steering servo pin
#define LED_PIN 16    // Status LED

// -------- Objects --------
Servo steering;

// -------- Setup --------
void setup() {
  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Servo
  steering.attach(SERVO_PIN);

  // LED
  pinMode(LED_PIN, OUTPUT);

  // Bluetooth UART (Bluefruit LE UART Friend)
  Serial1.begin(9600);   // UART to BLE module
  Serial.begin(9600);    // USB debug

  // Initialize motors off
  stopMotors();
  steering.write(90);  // center servo

  Serial.println("Pico car system initialized.");
  initHCSR04();
  int speedPWM = 0;
}

// -------- Motor Control --------
void driveForward() {
  int speedPWM = computeSpeedPWM(distanceCm);
  analogWrite(ENA, speedPWM);
  analogWrite(ENB, speedPWM);
  Serial.print("Speed: ");
  Serial.println(speedPWM);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void driveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// -------- Main Loop --------
void loop() {
  
  // Blink the LED for heartbeat
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);
  delay(200);

  // Listen for commands from the Bluefruit UART
  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    updateHCSR04();
    if (cmd == "F") {
      driveForward();
      Serial.println("Forward");
    }
    else if (cmd == "B") {
      driveBackward();
      Serial.println("Backward");
    }
    else if (cmd == "S") {
      stopMotors();
      Serial.println("Stop");
    }
    else {
      // Interpret as steering angle
      int angle = cmd.toInt();

      if (angle >= 0 && angle <= 180) {
        steering.write(angle);
        Serial.print("Steering angle: ");
        Serial.println(angle);
      } else {
        Serial.print("Invalid angle: ");
        Serial.println(cmd);
      }
    }
  }
}
