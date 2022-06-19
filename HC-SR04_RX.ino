#include <CircularBuffer.h>

#define echoPin 3
#define trigPin 2

CircularBuffer<int,20> buffer;

long duration;
int distance;
int currentState;
int MA;
int triggerThreshold = 25;

long startTime;
long deltaTime;

byte received = 0b00000000;
short currentPos = 0;

int state = 1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  startTime = millis();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;
  
  Serial.print(distance); Serial.print(";");
  buffer.push(distance);   
  
 
  
  MA = 0;
  for (int i = 0; i < buffer.size(); i++) {
    if(buffer[i] > triggerThreshold) {
      MA += buffer[i];
    }
    else {
       MA += 0;
    }
  
  }
  MA = MA / buffer.size();

    
  Serial.print(MA); Serial.print(";");

  if (MA <= 32) {
    currentState = 0;
  }
  else {
    currentState = 1;
  }

  Serial.print(currentState); Serial.print(";");

  deltaTime = millis();
  if (deltaTime - startTime > 2*1000) {
    startTime = millis();
    setState(currentState);
   
  }
  Serial.println("");
  delay(10);
}


void setState(int cState) {
  if (state == 1) { // Idle
    if (cState == 1) { state = 1; }
    else if (cState == 0) { state = 3; } // Start state
  }
  else if (state == 3) { // Data recv
    if (currentPos < 8) {
      bitWrite(received, currentPos, cState);
      currentPos++;
    }
    else if (currentPos == 8) {
      // Check parity bit
      Serial.write(received); Serial.print(";");
      currentPos++;
    }
    else if (currentPos > 8) {
      currentPos = 0;
      received = 0b00000000;
      if (cState == 0) { state = 4; }
      else if (cState == 1) { state = 1; }
    }
  }
  else if (state == 4) { // Malformed packet
    if (cState == 1) { state = 1; }
  }
}
