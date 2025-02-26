#ifndef RFIDMELODY_H
#define RFIDMELODY_H

#include <Arduino.h>
#include "BuzzerTone.h"

// Define note frequency in Hertz (octave 4)
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494
#define C5 523
#define D5 587
#define E5 659

class RFIDMelody {
    public:
        RFIDMelody(BuzzerTone &buzzer);
        void playAllowed();
        void playDenied();
        void playUIDMelody(const char* uidDecimalStr);
    private:
        BuzzerTone &_buzzer; // Reference to BuzzerTone object that has defined in main program
};

#endif
