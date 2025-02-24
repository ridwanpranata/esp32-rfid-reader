#include "RFIDReader.h"

RFIDReader::RFIDReader(HardwareSerial &serial, int rxPin, unsigned long tagTimeout)
  : _rfidSerial(serial), _rxPin(rxPin), _tagTimeout(tagTimeout), _lastDecimalUID(0), _lastTagTime(0) {
}

void RFIDReader::begin() {
  // Start RFID serial communication at 9600 baud.
  _rfidSerial.begin(9600, SERIAL_8N1, _rxPin, -1);
}

void RFIDReader::flushBuffer() {
  // Read and discard all available data.
  while (_rfidSerial.available()) {
    _rfidSerial.read();
  }
}

bool RFIDReader::readTag(char *lastUID, size_t uidSize) {
  // Check if enough data is available in the serial buffer
  if (_rfidSerial.available() < RDM6300_FRAME_SIZE) {
    return false;
  }
  
  // If timeout has passed since the last tag, reset last tag info
  if (millis() - _lastTagTime > _tagTimeout) {
    _lastDecimalUID = 0;
  }
  
  char frameBuffer[RDM6300_FRAME_SIZE + 1]; // Buffer to store RFID frame (+1 for null terminator)
  int index = 0;
  unsigned long startTime = millis();
  
  // Wait until the complete frame is available (timeout after 100ms)
  while (_rfidSerial.available() < RDM6300_FRAME_SIZE) {
    if (millis() - startTime > 100) {
      flushBuffer();  // Clear the buffer on timeout
      return false;
    }
  }
  
  // Read the complete RFID frame
  while (index < RDM6300_FRAME_SIZE && _rfidSerial.available()) {
    frameBuffer[index++] = _rfidSerial.read();
  }
  frameBuffer[index] = '\0'; // Null terminate the string
  
  // Validate the frame: it must start with 0x02 and end with 0x03
  if (frameBuffer[0] != 0x02 || frameBuffer[13] != 0x03) {
    return false;
  }
  
  // Extract the UID from the frame (8 characters starting at index 3)
  char uidHex[9]; // 8 characters UID + null terminator
  strncpy(uidHex, frameBuffer + 3, 8);
  uidHex[8] = '\0';
  
  // Convert the HEX UID to a decimal number
  unsigned long decimalUID = strtoul(uidHex, NULL, 16);
  
  // If a new tag is detected (or the same tag re-tapped after timeout)
  if (decimalUID != _lastDecimalUID) {
    _lastDecimalUID = decimalUID;
    _lastTagTime = millis();
    snprintf(lastUID, uidSize, "%010lu", decimalUID);  // Format as a 10-digit string with leading zeros
    return true;
  } else {
    // Same tag is still present; update the detection time without reprocessing
    _lastTagTime = millis();
    return false;
  }
}
