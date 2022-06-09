#define echoPin 18
#define trigPin 5

long duration;
int distance;
unsigned long startTime;
unsigned long deltaTime;

/* byte packetData = 0b01010101; */
String msg;

void sendPulse() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;
  
  delay(10);
}

void sendZero() {
  Serial.print(".");
  long startTime = millis();
  long deltaTime = millis();
  while (deltaTime - startTime < 2*1000) {
    sendPulse();
    deltaTime = millis();
  }
}

void sendOne() {
  Serial.print("-");
  delay(2*1000);  
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  startTime = millis();
}

void sendFrame(byte packet) {
  sendOne();
  sendOne();
  sendZero();

  for (int i = 0; i < 8; i++) {
    if (bitRead(packet, i) == 0 ) {
      sendZero();
    }
    else {
      sendOne();
    }
  }
  sendOne(); // Parity bit
  sendOne();
  Serial.println();
}

void loop() {
  msg = Serial.readString();
  msg.trim();
  if (msg.length() > 0) {
    // Send character
    Serial.println(msg);
    for (int i = 0; i < msg.length(); i++) {
      byte b = (byte)((char) msg[i]);
      Serial.println(b);
      sendFrame(b);
      delay(1);
    }
  }  
}
