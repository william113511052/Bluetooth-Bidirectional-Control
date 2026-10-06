#include <SoftwareSerial.h>

// RX = Pin 10 (接 HC-05 TXD), TX = Pin 11 (接 HC-05 RXD)
SoftwareSerial BTSerial(10, 11);

const int buttonPin = 2; // 按鈕腳位
const int enPin = 9;     // L293D 1,2EN (PWM 轉速控制)
const int in1Pin = 4;    // L293D 1A
const int in2Pin = 5;    // L293D 2A

bool lastButtonState = LOW;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(enPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  // 設定馬達固定正轉方向，初始速度為 0
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enPin, 0);

  Serial.begin(9600);
  BTSerial.begin(9600); // 一般通訊模式鮑率為 9600
  Serial.println("Master Node Ready!");
}

void loop() {
  // 【發送端：Master -> Slave】偵測按鈕狀態改變時，傳送 '1' 或 '0' 控制 B 的 LED
  bool currentButtonState = digitalRead(buttonPin);
  if (currentButtonState != lastButtonState) {
    if (currentButtonState == HIGH) {
      BTSerial.println("1");
      Serial.println("Button Pressed -> Turn ON Slave LED");
    } else {
      BTSerial.println("0");
      Serial.println("Button Released -> Turn OFF Slave LED");
    }
    delay(30); // 防彈跳
  }
  lastButtonState = currentButtonState;

  // 【接收端：Slave -> Master】讀取 B 傳來的可變電阻轉速值 (0~255) 控制馬達
  if (BTSerial.available() > 0) {
    String data = BTSerial.readStringUntil('\n');
    data.trim();
    if (data.length() > 0) {
      int motorSpeed = data.toInt();
      motorSpeed = constrain(motorSpeed, 0, 255); // 限制在合法 PWM 範圍
      
      analogWrite(enPin, motorSpeed);
      
      Serial.print("Motor Speed = ");
      Serial.println(motorSpeed);
    }
  }
}
