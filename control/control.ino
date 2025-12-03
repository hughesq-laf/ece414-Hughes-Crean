#include <SPI.h>
#include <LoRa.h>
#include <Servo.h>
#include "Computation.h"
#include "hcsr04.h"

// ---------------- LoRa Pins ----------------
#define LORA_CS   5
#define LORA_RST  19
#define LORA_IRQ  2

// ---------------- Motor Pins ---------------
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define ENA 6 //PWM Pins for motor.
#define ENB 5

// ---------------- Servo --------------------
#define SERVO_PIN 18
Servo steering;

// ---------------- LED ----------------------
#define LED_PIN 16

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  steering.attach(SERVO_PIN);
  steering.write(90);

  // SPI pin remap
  SPI.setSCK(6);
  SPI.setTX(7);
  SPI.setRX(4);

  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);

  Serial.println("Starting LoRa…");

  if (!LoRa.begin(915E6)) {
    Serial.println("LoRa init failed!");
    while (1);
  }

  Serial.println("LoRa init success!");
}

void loop() {

  // ----- heartbeat -----
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  delay(100);

  // ----- receive packets -----
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = LoRa.readString();
    Serial.print("RX: ");
    Serial.println(received);

    // Format: "F,120"
    char movement = received.charAt(0);
    int commaIndex = received.indexOf(',');
    int angle = received.substring(commaIndex + 1).toInt();

    // Steering
    steering.write(angle);
    updateHCSR04();
    // Movement
    if (movement == 'F') {
      int speedPWM = computeSpeedPWM(float distanceCm);
      analogWrite(ENA, speedPWM);
      analogWrite(ENB, speedPWM);
      digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    } else if (movement == 'B') {
      digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    } else {
      digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
    }
  }
}
