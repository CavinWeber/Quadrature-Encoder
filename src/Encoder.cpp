#include "Encoder.h"
#include <driver/gpio.h>

QuadratureEncoder::QuadratureEncoder(int A, int B)
    : APin(A), BPin(B), encoderValue(0), AOld(false), BOld(false), interrupt(false), APinReg(32 - APin - 1), BPinReg(32 - BPin - 1) {}
  
void QuadratureEncoder::begin() {
  pinMode(APin, INPUT_PULLUP);
  pinMode(BPin, INPUT_PULLUP);
  AOld = digitalRead(APin);
  BOld = digitalRead(BPin);
}

int QuadratureEncoder::read()
/**
 * Standard read of a quadrature encoder using standard digitalRead().
 * Slower than direct write, but much more reliable in Arduino.
 */
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

void QuadratureEncoder::read_ISR()
/**
 * Standard read of a quadrature encoder using standard digitalRead().
 * Designed for use in an ISR based on the "alter" state of the A and B pins.
 */
{
  bool ANew = digitalRead(APin);
  bool BNew = digitalRead(BPin);

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
}

int QuadratureEncoder::fastRead()
/**
 * This type of read is designed for fast-as-possible value reading using
 * port manipulation. This is likely only valid on a Teensy 4.0
 * microcontroller.
 */
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

void QuadratureEncoder::readNoBranch_ISR()
/**
 * Standard read of a quadrature encoder using standard digitalRead().
 * Uses additional bitmath to avoid branching altogether.
 * Proof of concept. Does not return or modify any variables.
 */
{
  bool ANew = digitalRead(APin);
  bool BNew = digitalRead(BPin);

  byte AOldBOld = AOld << 1 | BOld; // Create bit couple for old values
  byte ANewBNew = ANew << 1 | BNew; // Create bit couple for new values
  byte ATestBTest = (ANewBNew >> 1) | ((!BNew) << 1);

  encoderValue += ((int(AOldBOld ^ ATestBTest) * 2) - 1); // Full compare and encoderValue manipulation operation.

  AOld = ANew;
  BOld = BNew;
}