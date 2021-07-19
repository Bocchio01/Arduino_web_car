//www.elegoo.com
//2016.12.09
#include <Servo.h>

Servo myser;

long int pos;
float po1;
int re;
int value;

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

void setup() {
  pinMode(5, OUTPUT);
  myser.attach(9);
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  myser.write(0);
}

void loop() {
  //Serial.print(analogRead(Y_pin));
  //Serial.print(" ");
  //Serial.println(analogRead(Y_pin));
  re = analogRead(Y_pin);
  pos = re*16;
  po1 = pos/102;
  Serial.println(po1);
  pos = po1;
  value =analogRead(X_pin)/4;
  //Serial.println(pos);
  myser.write(pos);
  analogWrite(5, value);
    Serial.println(value);
}
