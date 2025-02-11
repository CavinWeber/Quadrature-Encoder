#include "Encoder.h"
#include <driver/gpio.h>

RotaryEncoder::RotaryEncoder(int A, int B)
    : APin(A), BPin(B), encoderValue(0), AOld(false), BOld(false), interrupt(false), APinReg(32 - APin - 1), BPinReg(32 - BPin - 1) {}
  
void RotaryEncoder::begin() {
  pinMode(APin, INPUT_PULLUP);
  pinMode(BPin, INPUT_PULLUP);
  AOld = digitalRead(APin);
  BOld = digitalRead(BPin);
}

int RotaryEncoder::read()
{
  bool ANew = digitalRead(APin);
  bool BNew = digitalRead(BPin);

  if (ANew == AOld && BNew == BOld){  // Check to see if old value and new value are the same
    return encoderValue;
  }

  byte AOldBOld = AOld << 1 | BOld; // Create bit couple for old values
  byte ANewBNew = ANew << 1 | BNew; // Create bit couple for new values
  byte ATestBTest = (ANewBNew >> 1) | ((!BNew) << 1);

  if (AOldBOld ^ ATestBTest) {
    encoderValue++; // Right turn
  } else {
    encoderValue--; // Left turn
  }

  AOld = ANew;
  BOld = BNew;
  
  return encoderValue;
}

int RotaryEncoder::fastRead()
{
  bool ANew = *gpio_in_reg >> APinReg & 1;
  bool BNew = *gpio_in_reg >> BPinReg & 1;

  if (ANew == AOld && BNew == BOld){  // Check to see if old value and new value are the same
    return encoderValue;
  }

  byte AOldBOld = AOld << 1 | BOld; // Create bit couple for old values
  byte ANewBNew = ANew << 1 | BNew; // Create bit couple for new values
  byte ATestBTest = (ANewBNew >> 1) | ((!BNew) << 1);

  if (AOldBOld ^ ATestBTest) {
    encoderValue++; // Right turn
  } else {
    encoderValue--; // Left turn
  }

  AOld = ANew;
  BOld = BNew;
  
  return encoderValue;
}