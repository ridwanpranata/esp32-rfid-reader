#include <Arduino.h>
#include "LiquidCrystal.h"

// Define PINs
#define PIN_SWITCH 12
#define PIN_RFID_RX 16

// Define LCD configuration and baud rates
#define LCD_COLUMN 20
#define LCD_ROW 4
#define SERIAL_BAUD 115200    // Baud rate for Serial Monitor
#define SERIAL_BAUD_RFID 9600 // Baud rate for RFID module

LiquidCrystal lcd(LCD_COLUMN, LCD_ROW);

// Define system variables
bool systemOn = false;
bool switchState = false;

// Define frame size for RDM6300 data (14 bytes per frame)
#define RDM6300_FRAME_SIZE 14

// Setup HardwareSerial for RFID (using UART2)
HardwareSerial RFIDSerial(2); // Use UART2

// Variables to store the last detected tag and time
unsigned long lastDecimalUID = 0;       // Last UID in decimal
unsigned long lastTagTime = 0;          // Time when the last tag was detected
const unsigned long TAG_TIMEOUT = 1000; // 1 second timeout to reset tag detection

void setup()
{
    // Initialize switch and serial communications
    pinMode(PIN_SWITCH, INPUT_PULLUP);
    Serial.begin(SERIAL_BAUD);
    lcd.init();

    // Initialize RFID serial port at 9600 baud (TX not used, so set to -1)
    RFIDSerial.begin(SERIAL_BAUD_RFID, SERIAL_8N1, PIN_RFID_RX, -1);

    // Read initial switch state and update system status
    switchState = (digitalRead(PIN_SWITCH) == LOW);
    systemOn = switchState;
    lcd.setLineText(1, systemOn ? "Status: ON" : "Status: OFF");

    Serial.println("System initialized. Waiting for RFID...");
}

void loop()
{
    // Update switch state
    switchState = digitalRead(PIN_SWITCH) == LOW;

    // Turn system on/off based on switch state
    if (switchState && !systemOn)
    {
        systemOn = true;
        lcd.setLineText(1, "Status: ON");
        Serial.println("System ON");

        // Flush RFIDSerial buffer
        while (RFIDSerial.available())
        {
            RFIDSerial.read();
        }
    }
    else if (!switchState && systemOn)
    {

        systemOn = false;
        lcd.setLineText(1, "Status: OFF");
        Serial.println("System OFF");
    }

    // If system is off, skip RFID processing
    if (!systemOn)
    {
        delay(100);
        return;
    }

    // Process RFID data if there is enough data available in the buffer
    if (RFIDSerial.available() >= RDM6300_FRAME_SIZE)
    {
        char frameBuffer[RDM6300_FRAME_SIZE + 1]; // Buffer to store RFID frame (+1 for null terminator)
        int index = 0;
        unsigned long startTime = millis();

        // Wait until the complete frame is available to avoid partial reads
        while (RFIDSerial.available() < RDM6300_FRAME_SIZE)
        {
            if (millis() - startTime > 100) // Timeout setelah 100ms
            {
                Serial.println("Timeout: Incomplete RFID data.");
                return;
            }
        }

        // Read the complete RFID frame
        while (RFIDSerial.available() && index < RDM6300_FRAME_SIZE)
        {
            frameBuffer[index] = RFIDSerial.read();
            index++;
        }

        frameBuffer[index] = '\0'; // Null terminator for string

        // Validate frame by checking start (0x02) and end (0x03) markers
        if (frameBuffer[0] == 0x02 && frameBuffer[13] == 0x03)
        {
            // Extract UID from frame: 8 characters starting at index 3
            char uidHex[9]; // 8 characters UID + null terminator
            strncpy(uidHex, frameBuffer + 3, 8);
            uidHex[8] = '\0'; // Ensure string is null-terminated

            // Convert UID from HEX string to decimal number
            unsigned long decimalUID = strtoul(uidHex, NULL, 16);

            // Check if the detected tag is different from the last one
            if (decimalUID != lastDecimalUID)
            {
                // New tag detected: update last tag and time
                lastDecimalUID = decimalUID;
                lastTagTime = millis();

                // Format the decimal UID as a 10-digit string with leading zeros
                char uidDecimalStr[11];
                sprintf(uidDecimalStr, "%010lu", decimalUID);

                Serial.print("RFID UID HEX (4 Byte): ");
                Serial.println(uidHex);
                Serial.print("RFID UID Decimal (4 Byte): ");
                Serial.println(uidDecimalStr);

                // Display the UID on the LCD
                lcd.setLineText(2, ("UID: " + String(uidDecimalStr)).c_str());
            }
            else
            {
                // Same tag is still present; update the last detection time to extend timeout
                lastTagTime = millis();
            }
        }
        else
        {
            Serial.println("Invalid RFID frame");
        }
    }

    // If no RFID frame is detected for a while, reset the last tag so that a new read will be accepted
    if (millis() - lastTagTime > TAG_TIMEOUT)
    {
        lastDecimalUID = 0;
    }

    delay(50);
}
