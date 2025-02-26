#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include "LiquidCrystal.h"
#include "RFIDReader.h"
#include "BuzzerTone.h"
#include "RFIDMelody.h"

// Define PINs
#define PIN_SWITCH 12
#define PIN_RFID_RX 16
#define PIN_BUZZER 5

// Define LCD configuration and Serial baud rates
#define LCD_COLUMN 20
#define LCD_ROW 4
#define SERIAL_BAUD 115200    // Baud rate for Serial Monitor

LiquidCrystal lcd(LCD_COLUMN, LCD_ROW);

// Create a HardwareSerial instance for RFID (using UART2)
HardwareSerial rfidSerial(2);

// Define RFID configuration
#define RFID_READ_TIMEOUT 2000
RFIDReader rfid(rfidSerial, PIN_RFID_RX, RFID_READ_TIMEOUT); // 2000ms timeout for re-read

// Define buzzer configuration
#define BUZZER_CHANNEL 0
#define BUZZER_RESOLUTION 8
#define BUZZER_VOLUME 5

BuzzerTone buzzer(PIN_BUZZER, BUZZER_CHANNEL, BUZZER_RESOLUTION, BUZZER_VOLUME);
RFIDMelody rfidMelody(buzzer);

// System variables
bool systemOn = false;
bool switchState = false;

void setup() {
    // Initialize switch and serial communications
    pinMode(PIN_SWITCH, INPUT_PULLUP);
    Serial.begin(SERIAL_BAUD);
    lcd.init();
    
    // Initialize RFID reader
    rfid.begin();

    // Initialize Buzzer
    buzzer.begin();
    
    // Read initial switch state and update system status
    switchState = (digitalRead(PIN_SWITCH) == LOW);
    systemOn = switchState;
    lcd.setLineText(1, systemOn ? "Status: ON" : "Status: OFF");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("\nWiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    
    Serial.println("System initialized. Waiting for RFID...");
}

void loop() {
    // Update switch state
    switchState = (digitalRead(PIN_SWITCH) == LOW);
    
    // Turn system on/off based on switch state
    if (switchState && !systemOn) {
        systemOn = true;
        lcd.setLineText(1, "Status: ON");
        Serial.println("System ON");
        // Flush RFID buffer when switching on
        rfid.flushBuffer();
    }
    else if (!switchState && systemOn) {
        systemOn = false;
        lcd.setLineText(1, "Status: OFF");
        Serial.println("System OFF");
    }
    
    // If system is off, skip RFID processing
    if (!systemOn) {
        delay(100);
        return;
    }
    
    // Process RFID data if available
    char uidDecimalStr[11] = "";
    if (rfid.readTag(uidDecimalStr, sizeof(uidDecimalStr))) {
        Serial.print("RFID UID Decimal (4 Byte): ");
        Serial.println(uidDecimalStr);
        Serial.print("RFID UID Hexadecimal (4 Byte): ");
        Serial.println(rfid.getLastUIDHex());

        lcd.setLineText(2, ("UID: " + String(uidDecimalStr)).c_str());

        if(String(uidDecimalStr) == RFID_ALLOWED_UID) {
            lcd.setLineText(3,"Result: Allowed");
            rfidMelody.playAllowed();
        } else if (String(uidDecimalStr) == RFID_DENIED_UID) {
            lcd.setLineText(3,"Result: Denied");
            rfidMelody.playDenied();
        } else {
            rfidMelody.playUIDMelody(uidDecimalStr);
        }
    }
    
    delay(50);
}
