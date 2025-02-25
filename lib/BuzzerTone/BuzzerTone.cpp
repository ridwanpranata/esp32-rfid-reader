#include "BuzzerTone.h"

BuzzerTone::BuzzerTone(uint8_t buzzerPin, uint8_t channel, uint8_t resolution, uint8_t volume)
    : _buzzerPin(buzzerPin), _channel(channel), _resolution(resolution), _volume(volume)
{
}

void BuzzerTone::begin()
{
    pinMode(_buzzerPin, OUTPUT);
    ledcAttachPin(_buzzerPin, _channel);
    ledcSetup(_channel, 0, _resolution);
}

void BuzzerTone::playTone(int frequency, int duration)
{
    ledcWriteTone(_channel, frequency);
    ledcWrite(_channel, _volume);
    delay(duration);
    ledcWrite(_channel, 0);
    delay(200);
}

void BuzzerTone::playMelody(const int *melody, const int *durations, int length)
{
    for (int i = 0; i < length; i++) {
        playTone(melody[i], durations[i]);
    }
}

void BuzzerTone::stop() {
    ledcWriteTone(_channel, 0);  // Matikan buzzer
}
