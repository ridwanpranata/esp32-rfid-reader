#include "RFIDMelody.h"

RFIDMelody::RFIDMelody(BuzzerTone &buzzer) : _buzzer(buzzer) {}

void RFIDMelody::playOk() {
    int melody[] = {C4, E4, G4};
    int durations[] = {200, 200, 400};
    int length = sizeof(melody) / sizeof(melody[0]);
    _buzzer.playMelody(melody, durations, length);
}

void RFIDMelody::playDenied() {
    int melody[] = {G4, E4, C4};
    int durations[] = {200, 200, 400};
    int length = sizeof(melody) / sizeof(melody[0]);
    _buzzer.playMelody(melody, durations, length);
}

void RFIDMelody::playUIDMelody(const char* uidDecimalStr) {
    if (strlen(uidDecimalStr) < 4) return;  // at lease 4 character

    // get last 4 digits of UID
    String last4 = String(uidDecimalStr).substring(strlen(uidDecimalStr) - 4);
    Serial.print("Playing melody for UID last 4 digits: ");
    Serial.println(last4);

    // Map note for digit 0-9
    const int noteMap[10] = {C4, D4, E4, F4, G4, A4, B4, C5, D5, E5};
    const int noteDuration = 200;

    int melody[4];
    int durations[4];

    // convert last 4 digits to melody
    for (int i = 0; i < 4; i++) {
        int digit = last4.charAt(i) - '0';  // Convert char to int
        melody[i] = noteMap[digit];
        durations[i] = noteDuration;
    }

    int length = sizeof(melody) / sizeof(melody[0]);
    _buzzer.playMelody(melody, durations, length);
}

