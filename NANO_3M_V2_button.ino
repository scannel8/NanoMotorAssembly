#include <Stepper.h>

// === Constants ===
const int STEPS_PER_REV = 2048;
const float ROTATION_FRACTION = 0.75;
const int STEPS_PER_MOVE = (int)(STEPS_PER_REV * ROTATION_FRACTION); // = 1536 steps
const int RPM = 30;

// === Button (SW) Pins ===
const int BTN1 = A0;
const int BTN2 = A2;
const int BTN3 = A4;

// === Motor Setup ===
Stepper stepper1(STEPS_PER_REV, 2, 3, 4, 5);
Stepper stepper2(STEPS_PER_REV, 6, 7, 8, 9);
Stepper stepper3(STEPS_PER_REV, 10, 11, 12, 13);

// === State Variables ===
bool dir1 = true;      // true = CW, false = CCW
bool dir2 = true;
bool dir3 = true;

bool lastPressed1 = false;
bool lastPressed2 = false;
bool lastPressed3 = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Stepper Button Control Mode");

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);

  stepper1.setSpeed(RPM);
  stepper2.setSpeed(RPM);
  stepper3.setSpeed(RPM);
}

void loop() {
  checkButtonAndMove(BTN1, stepper1, dir1, lastPressed1, "Motor 1");
  checkButtonAndMove(BTN2, stepper2, dir2, lastPressed2, "Motor 2");
  checkButtonAndMove(BTN3, stepper3, dir3, lastPressed3, "Motor 3");
}

void checkButtonAndMove(int pin, Stepper &motor, bool &dir, bool &lastPressed, const char* label) {
  bool currentPressed = digitalRead(pin) == LOW; // LOW = pressed

  if (currentPressed && !lastPressed) {
    Serial.print(label); Serial.print(" button pressed → ");
    Serial.println(dir ? "CW" : "CCW");

    if (dir) {
      motor.step(STEPS_PER_MOVE); // CW
    } else {
      motor.step(-STEPS_PER_MOVE); // CCW
    }

    dir = !dir; // Flip direction for next press
  }

  lastPressed = currentPressed;
}
