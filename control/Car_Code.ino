#include <Servo.h>
#include "Computation.h"
#include "hcsr04.h"

// -------- Pin Definitions --------
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define SERVO_PIN 18  // Steering servo pin
#define LED_PIN 16    // Status LED
#define ENA 26 //PWM Pins for motor.

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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Servo
  steering.attach(SERVO_PIN);
  
  // LED
  pinMode(LED_PIN, OUTPUT);
  
  // HC-12 UART
  Serial1.setTX(0);  // GP0 -> HC-12 RX
  Serial1.setRX(1);  // GP1 <- HC-12 TX
  Serial1.begin(9600);
  
  // Debug serial
  Serial.begin(115200);
  
  // Initialize motors off
  stopMotors();
  steering.write(90);  // center servo
  
  Serial.println("Pico car system initialized.");
  Serial.println("HC-12 ready - waiting for commands...");
  initHCSR04();
  // Startup blink
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

// -------- Motor Control --------
void driveForward(float D) {
  int speedPWM = computeSpeedPWM(D);
  analogWrite(ENA, speedPWM);
  //Serial.print("spped: ");
  //Serial.println(speedPWM);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void driveBackward() {
  // Backward: IN1=LOW, IN2=HIGH (right motor)
  //           IN3=LOW, IN4=HIGH (left motor)
  analogWrite(ENA, 200);
  
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
  updateHCSR04();
  float dist = returnDist();
  while(dist == 0.0){
    updateHCSR04();
    float dist = returnDist();
  }
  // Listen for commands from HC-12
  if (Serial1.available()) {
    String cmd = Serial1.readStringUntil('\n');
    cmd.trim();
    
    // Debug print
    Serial.print("Received: ");
    Serial.println(cmd);
    
    // Blink LED to show reception
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    
    // Process command
    if (cmd == "F") {
      
      driveForward(dist);
      Serial.println("Action: Forward");
      Serial.print("Distance: ");
      Serial.println(dist);
    }
    else if (cmd == "R") {  // R for Reverse
      driveBackward();
      Serial.println("Action: Backward");
    }
    else if (cmd == "S") {
      stopMotors();
      Serial.println("Action: Stop");
    }
    else {
      // Interpret as steering angle (0-180)
      int angle = cmd.toInt();
      
      if (angle >= 0 && angle <= 180) {
        steering.write(angle);
        Serial.print("Steering: ");
        Serial.println(angle);
      } else {
        Serial.print("Invalid command: ");
        Serial.println(cmd);
      }
    }
  }
  
  // Optional: Heartbeat blink every 2 seconds
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 2000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastHeartbeat = millis();
  }
}
