// Motor Driver Pins
const int ENA = 5; 
const int IN1 = 4;
const int IN2 = 3;
const int IN3 = 2;
const int IN4 = 7;
const int ENB = 6;

// 5-Channel IR Sensor Pins
const int S1 = A0; // Far Left
const int S2 = A1; // Mid Left
const int S3 = A2; // Center
const int S4 = A3; // Mid Right
const int S5 = A4; // Far Right

// Base Speed Settings (PWM values: 0 to 255)
const int baseSpeed = 150;
const int turnSpeedFast = 200;
const int turnSpeedSlow = 50;

void setup() {
  // Initialize Motor Control Pins
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize Sensor Pins
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
}

void loop() {
  // Read digital state of sensors (assuming Black line gives HIGH, White gives LOW)
  // If your sensor logic is inverted, flip HIGH and LOW in the conditions below.
  int s1_val = digitalRead(S1);
  int s2_val = digitalRead(S2);
  int s3_val = digitalRead(S3);
  int s4_val = digitalRead(S4);
  int s5_val = digitalRead(S5);

  // Case 1: Center sensor is on line -> Move Straight
  if ((s3_val == HIGH) && (s2_val == LOW) && (s4_val == LOW)) {
    moveForward(baseSpeed, baseSpeed);
  }
  
  // Case 2: Slight Right Turn Needed (Line is under Mid Left Sensor)
  else if ((s2_val == HIGH) && (s1_val == LOW)) {
    moveForward(turnSpeedSlow, turnSpeedFast); // Slow down left motor, speed up right
  }
  
  // Case 3: Sharp Right Turn Needed (Line is under Far Left Sensor)
  else if (s1_val == HIGH) {
    spinLeft(turnSpeedFast); // Spin chassis in place to recover the line
  }
  
  // Case 4: Slight Left Turn Needed (Line is under Mid Right Sensor)
  else if ((s4_val == HIGH) && (s5_val == LOW)) {
    moveForward(turnSpeedFast, turnSpeedSlow); // Speed up left motor, slow down right
  }
  
  // Case 5: Sharp Left Turn Needed (Line is under Far Right Sensor)
  else if (s5_val == HIGH) {
    spinRight(turnSpeedFast); // Spin chassis in place to recover the line
  }
  
  // Case 6: Out of bounds / Intersection logic
  else if (s1_val == LOW && s2_val == LOW && s3_val == LOW && s4_val == LOW && s5_val == LOW) {
    // Optional: Stop or maintain last state if track is lost
    stopMotors();
  }
}

// Helper Functions for Motion Control
void moveForward(int speedLeft, int speedRight) {
  analogWrite(ENA, speedLeft);
  analogWrite(ENB, speedRight);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void spinLeft(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH); // Reverse left motor
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); // Forward right motor
}

void spinRight(int speed) {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Forward left motor
  digitalWrite(IN3, HIGH); // Reverse right motor
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}