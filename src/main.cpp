#include <Arduino.h>
#include "LiquidCrystal.h"

// Define PIN
#define PIN_SWITCH 12

// Define LCD configuration
#define LCD_COLUMN 20  // Perbaikan typo
#define LCD_ROW 4
#define SERIAL_BAUD 115200

LiquidCrystal lcd(LCD_COLUMN, LCD_ROW);

bool systemOn = false;
bool switchState = false;

void setup() {
    pinMode(PIN_SWITCH, INPUT_PULLUP);
    Serial.begin(SERIAL_BAUD);
    lcd.init();

    // Cek status awal dan tampilkan di LCD
    switchState = digitalRead(PIN_SWITCH) == LOW;
    systemOn = switchState;  // Samakan status awal dengan switch
    lcd.setLineText(1, systemOn ? "Status: ON" : "Status: OFF"); 
}

void loop() {
    switchState = digitalRead(PIN_SWITCH) == LOW;
    Serial.println(switchState);
    Serial.println(systemOn);

    if (switchState && !systemOn) {
        systemOn = true;
        lcd.setLineText(1, "Status: ON");
        Serial.println("System ON");
    } else if (!switchState && systemOn) {
        systemOn = false;
        lcd.setLineText(1, "Status: OFF");
        Serial.println("System OFF");
    }

    delay(100);
}
