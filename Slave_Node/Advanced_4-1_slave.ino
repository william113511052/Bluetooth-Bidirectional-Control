#include <SoftwareSerial.h>

// RX = Pin 10 (接 HC-05 TXD), TX = Pin 11 (接 HC-05 RXD)
SoftwareSerial BTSerial(10, 11);

const int potPin = A0;  // 可變電阻接 A0
const int ledPin = 13;  // LED 接 Pin 13

int lastSentSpeed = -1;
unsigned long lastSendTime = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  BTSerial.begin(9600); // 一般通訊模式鮑率為 9600
  Serial.println("Slave Node Ready!");
}

void loop() {
  // 【接收端：Master -> Slave】接收 A 的按鈕指令 ('1' 或 '0') 控制 LED 亮滅
  if (BTSerial.available() > 0) {
    String cmd = BTSerial.readStringUntil('\n');
    cmd.trim();
    if (cmd == "1") {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    } 
    else if (cmd == "0") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
  }

  // 【發送端：Slave -> Master】每 100ms 讀取可變電阻，若有變化則傳送對應轉速 (0~255)
  if (millis() - lastSendTime >= 100) {
    int potValue = analogRead(potPin); // 0 ~ 1023
    int motorSpeed = map(potValue, 0, 1023, 0, 255);

    // 過濾微小雜訊：只有當速度變化超過 2 時才發送，避免塞爆藍牙緩衝區
    if (abs(motorSpeed - lastSentSpeed) > 2) {
      BTSerial.println(motorSpeed);
      
      Serial.print("Sent Motor Speed = ");
      Serial.println(motorSpeed);
      
      lastSentSpeed = motorSpeed;
    }
    lastSendTime = millis();
  }
}
