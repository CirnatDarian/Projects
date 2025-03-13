#define BLYNK_TEMPLATE_ID "TMPL4nBI67qGr"
#define BLYNK_TEMPLATE_NAME "MobileRoboticArm"
#define BLYNK_AUTH_TOKEN "Gfb7ppuE396n-MLUpdoA33JSSaMbV_wc"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// Setează autentificarea Blynk
char auth[] = BLYNK_AUTH_TOKEN;

// Configurare Wi-Fi
char ssid[] = "Tenda_99D320";
char pass[] = "roadtell277";

// Definire pini pentru L298N
const int IN1 = 15;
const int IN2 = 2;
const int IN3 = 0;
const int IN4 = 4;

// Definire Servomotoare
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

// Pini pentru servomotoare
const int servoPins[] = {34, 35, 32, 33, 25, 26};

// Definire pini pentru senzorul ultrasonic HC-SR04
const int trigPin = 27;
const int echoPin = 14;

// Funcție pentru masurarea distantei
long measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  servo1.attach(34);
  servo2.attach(35);
  servo3.attach(32);
  servo4.attach(33);
  servo5.attach(25);
  servo6.attach(26);
}

// Controlul motoarelor
BLYNK_WRITE(V1) {
  int value = param.asInt();
  digitalWrite(IN1, value);
  digitalWrite(IN2, !value);
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  digitalWrite(IN1, !value);
  digitalWrite(IN2, value);
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  digitalWrite(IN3, value);
  digitalWrite(IN4, !value);
}

BLYNK_WRITE(V4) {
  int value = param.asInt();
  digitalWrite(IN3, !value);
  digitalWrite(IN4, value);
}

// Controlul Servomotoarelor 360
//180
BLYNK_WRITE(V5) {
  int value = param.asInt();
  servo1.write(30); // Rotire la stânga
}

BLYNK_WRITE(V6) {
  int value = param.asInt();
    servo1.write(20); // Rotire la dreapta
}

//360
BLYNK_WRITE(V7) {
  int value = param.asInt();
  if (value) {
    servo2.write(servo2.read()+60); // Rotire la stânga
  }
}

BLYNK_WRITE(V8) {
  int value = param.asInt();
  if (value) {
    servo2.write(servo2.read() - 90); // Rotire la dreapta
  }
}

//180
BLYNK_WRITE(V9) {
  int value = param.asInt();
  if (value) {
    servo3.write(servo3.read()+60); // Rotire la stânga
  }
}

BLYNK_WRITE(V10) {
  int value = param.asInt();
  if (value) {
    servo3.write(servo3.read()-60); // Rotire la dreapta
  }
}

//180
BLYNK_WRITE(V11) {
  int value = param.asInt();
  if (value) {
    servo4.write(servo4.read()+45); // Rotire la stânga
  }
}

BLYNK_WRITE(V12) {
  int value = param.asInt();
  if (value) {
    servo4.write(servo4.read() - 45); // Rotire la dreapta
  }
}

//180
BLYNK_WRITE(V13) {
  int value = param.asInt();
  if (value) {
    servo5.write(servo5.read()+60); // Rotire la stânga
  }
}

BLYNK_WRITE(V14) {
  int value = param.asInt();
  if (value) {
    servo5.write(servo4.read()-60); // Rotire la dreapta
  }
}

// Controlul Servomotorului cu Blynk (radial gauge)
BLYNK_WRITE(V15) {
  int value = param.asInt();  
  servo6.write(value);     
}

// Actualizarea distanței măsurate de senzorul ultrasonic
void updateDistance() {
  long distance = measureDistance();
  Blynk.virtualWrite(V16, distance);  // Trimite distanta la Blynk
}

void loop() {
  Blynk.run();
  updateDistance();  
  delay(1000);
}