#include <Servo.h> // Includes the servo library for comunicating with the lift motor
#include <NewPing.h> // Includes a library for comunicating with the ultrasonic sensor

int inputSwitch = 50; //Switch is on digital pin 22
boolean up = false; // assuming we start with the lift down
boolean held = false;

//Ultrasonic sensor setup
#define TRIGGER_PIN  52
#define ECHO_PIN     4
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

Servo liftMotor;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // make the switch an input:
  pinMode(inputSwitch, INPUT);
  // give the switch some power
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  
  // attach the lift motor to PWM pin 7
  liftMotor.attach(7);
}

// the loop routine runs over and over again forever:
void loop() {
  delay(50); //Wait 50ms between pings
  Serial.println(digitalRead(inputSwitch));
  
  if (digitalRead(inputSwitch) == LOW) {
    up = true;
  }
  else {
    up = false;
  }
  
  delay(50); //Wait 50ms between pings
  unsigned int uS = sonar.ping();
  int distance = uS / US_ROUNDTRIP_CM; //Convert ping time to distance
  
  if (distance < 6 && distance > 0) {
    delay(1500);
    int distance2 = sonar.ping() / US_ROUNDTRIP_CM;
    int difference = distance2 - distance;
    if (difference < 3) {
      held = true;
      Serial.println("Held");
    }
    else {
      held = false;
    }
  }
  else {
    held = false;
  }
  
  if (held == true) {
    if (up == true) {
      liftMotor.write(130);
    }
    else {
      liftMotor.write(60);
    }
  }
  else {
    liftMotor.write(95);
  }
}


