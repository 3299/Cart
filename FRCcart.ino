#include <Servo.h> // Includes the servo library for comunicating with the lift motor
#include <NewPing.h> // Includes a library for comunicating with the ultrasonic sensor

int limitSwitch1 = 50;
int limitSwitch2 = 51;
boolean up = false; // assuming we start with the lift down
boolean middle = false; // if lift is inbetween up and down positions
boolean held = false;

//Ultrasonic sensor setup
#define TRIGGER_PIN  52
#define ECHO_PIN     4
#define MAX_DISTANCE 10
long interval = 50;
long previousMillis = 0;
long sonarMillis = 0;
int distance;
int savedDistance;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

Servo liftMotor;

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // make the switch an input:
  pinMode(limitSwitch1, INPUT);
  pinMode(limitSwitch2, INPUT);
  // give the switch some power
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  
  // attach the lift motor to PWM pin 7
  liftMotor.attach(7);
}

// the loop routine runs over and over again forever:
void loop() {  
  /*if (digitalRead(limitSwitch1) == LOW) {
    up = true;
  }
  else {
    up = false;
  }*/
  
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    unsigned int uS = sonar.ping();
    distance = uS / US_ROUNDTRIP_CM; //Convert ping time to distance
  }
  
  if (distance < 8 && distance > 0) {
    Serial.println(distance);
    savedDistance = distance;
    if (currentMillis - sonarMillis > 4000) {
      float average = savedDistance + distance;
      average = average / 2;
      average = map(average, 1, 9, 1, 2);
      //Serial.println(average);
      if (average < 2) {
        held = true;
      }
      else {
        held = false;
      }
    }
  }
  else {
    held = false;
  }
      
  
  if (held == true) {
    Serial.println(digitalRead(limitSwitch1));
    if (digitalRead(limitSwitch1) == LOW) {
      liftMotor.write(130);
    }
    else if (digitalRead(limitSwitch1) == HIGH) {
      liftMotor.write(60);
    }
  }
  else {
    liftMotor.write(95);
  }
}


