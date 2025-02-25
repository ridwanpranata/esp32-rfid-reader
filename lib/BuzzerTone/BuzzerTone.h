#ifndef BUZZERTONE_H
#define BUZZERTONE_H

#include <Arduino.h>

class BuzzerTone {
    public:
        BuzzerTone(uint8_t buzzerPin, uint8_t channel, uint8_t resolution, uint8_t volume);
        void begin(); // Inisialisasi hardware (pin, PWM, dsb)
        void playTone(int frequency, int duration);
         void playMelody(const int *melody, const int *durations, int length);
        void stop();
    private:
        uint8_t _buzzerPin;
        uint8_t _channel;
        uint8_t _resolution;
        uint8_t _volume;
};

#endif
