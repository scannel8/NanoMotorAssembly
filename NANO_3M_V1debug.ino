#include <Stepper.h>

// === Constants ===
const int STEPS_PER_REV = 2048;               
const float ROTATION_FRACTION = 0.75;
const int STEPS_PER_MOVE = (int)(STEPS_PER_REV * ROTATION_FRACTION); // = 1536 steps
const int RPM = 30;                            // 0.5 rotations per second

// === Joystick Pins ===
const int JOY1_X = A0;
const int JOY2_X = A2;
const int JOY3_X = A4;

// === Deadzone Settings ===
const int DEADZONE = 100;
const int CENTER = 512;

// === Motor Setup ===
Stepper stepper1(STEPS_PER_REV, 2, 3, 4, 5);
Stepper stepper2(STEPS_PER_REV, 6, 7, 8, 9);
Stepper stepper3(STEPS_PER_REV, 10, 11, 12, 13);

// === Trigger State ===
bool triggered1 = false;
bool triggered2 = false;
bool triggered3 = false;

// === Step Counters ===
long stepsMotor1 = 0;
long stepsMotor2 = 0;
long stepsMotor3 = 0;

void setup() {
  Serial.begin(9600);  // Optional: use 115200 for faster output
  Serial.println("Stepper Motor Debugging with Timestamps Started...");

  stepper1.setSpeed(RPM);
  stepper2.setSpeed(RPM);
  stepper3.setSpeed(RPM);
}

void loop() {
  handleMotor(JOY1_X, stepper1, triggered1, stepsMotor1, "Motor 1");
  handleMotor(JOY2_X, stepper2, triggered2, stepsMotor2, "Motor 2");
  handleMotor(JOY3_X, stepper3, triggered3, stepsMotor3, "Motor 3");

  delay(100); // Prevents excessive serial spam
}

void handleMotor(int joyPin, Stepper &motor, bool &triggered, long &stepCount, const char* label) {
  int xVal = analogRead(joyPin);
  unsigned long t = millis();

  Serial.print("["); Serial.print(t); Serial.print(" ms] ");
  Serial.print(label); Serial.print(" joystick X: "); Serial.println(xVal);

  if (!triggered) {
    if (xVal < CENTER - DEADZONE) {
      Serial.print("["); Serial.print(t); Serial.print(" ms] ");
      Serial.print(label); Serial.println(" → Clockwise (0.75 turns)");
      motor.step(STEPS_PER_MOVE);
      stepCount += STEPS_PER_MOVE;
      Serial.print("["); Serial.print(t); Serial.print(" ms] ");
      Serial.print(label); Serial.print(" → Total steps: "); Serial.println(stepCount);
      triggered = true;
    } else if (xVal > CENTER + DEADZONE) {
      Serial.print("["); Serial.print(t); Serial.print(" ms] ");
      Serial.print(label); Serial.println(" → Counterclockwise (0.75 turns)");
      motor.step(-STEPS_PER_MOVE);
      stepCount -= STEPS_PER_MOVE;
      Serial.print("["); Serial.print(t); Serial.print(" ms] ");
      Serial.print(label); Serial.print(" → Total steps: "); Serial.println(stepCount);
      triggered = true;
    }
  }

  if (xVal >= CENTER - DEADZONE && xVal <= CENTER + DEADZONE) {
    if (triggered) {
      Serial.print("["); Serial.print(t); Serial.print(" ms] ");
      Serial.print(label); Serial.println(" → Joystick returned to center. Trigger reset.");
    }
    triggered = false;
  }
}
