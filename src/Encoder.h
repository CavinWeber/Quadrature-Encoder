#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class RotaryEncoder{
    public:
    RotaryEncoder(int APin, int BPin);
    void begin();
    int read();
    int readInterrupt();
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
    // static RotaryEncoder* instance; // Static instance to be used by the interrupt routine
};


#endif