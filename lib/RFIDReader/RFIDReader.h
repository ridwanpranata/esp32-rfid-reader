#ifndef RFIDREADER_H
#define RFIDREADER_H

#include <Arduino.h>

// Define frame size for RDM6300 data (14 bytes per frame)
#define RDM6300_FRAME_SIZE 14

class RFIDReader {
  public:
    // Constructor: takes a reference to the HardwareSerial instance, the RFID RX pin, and the tag timeout (in ms)
    RFIDReader(HardwareSerial &serial, int rxPin, unsigned long tagTimeout = 1000);
    
    // Begin RFID serial communication
    void begin();
    
    // Read an RFID tag.
    // If a new tag is detected, it formats the UID as a 10-digit decimal string and copies it to lastUID.
    // Returns true only when a new tag is processed.
    bool readTag(char *lastUID, size_t uidSize);
    
    // Flush the RFID serial buffer
    void flushBuffer();

  private:
    HardwareSerial &_rfidSerial;
    int _rxPin;
    unsigned long _tagTimeout;
    unsigned long _lastDecimalUID;
    unsigned long _lastTagTime;
};

#endif
