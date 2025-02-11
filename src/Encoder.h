/**
 * This library is designed to do a fast-as-possible read of a quadrature encoder
 * using only bitwise math. On most processors, the quadrature read should only
 * take a handful of cycles with minimal branching.
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class QuadratureEncoder{
    public:
    QuadratureEncoder(int APin, int BPin);
    void begin();
    int read();
    void read_ISR();
    int readInterrupt();
    void readNoBranch_ISR();
    void enableInterrupts();
    int fastRead();

    private:
    int APin;
    int BPin;
    int encoderValue;
    bool AOld;
    bool BOld;
    volatile bool interrupt;
    void raiseInterruptFlag();
    static void IRAM_ATTR isrHandler();
    uint8_t APinReg;
    uint8_t BPinReg;
    volatile uint32_t *gpio_in_reg = (volatile uint32_t *)GPIO_IN_REG;
    uint32_t reg_value;
    // static QuadratureEncoder* instance; // Static instance to be used by the interrupt routine
};


#endif