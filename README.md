# RFID Reader with ESP32 and I2C LCD

This project is an RFID reader system using an **ESP32 WROOM 32D**, a **20x4 I2C LCD**, a **switch button**, a **buzzer for sound feedback**, and an **RDM6300 RFID Reader (125kHz)**. It utilizes the **LiquidCrystal_I2C** library for LCD control and is developed using **VSCode with PlatformIO**. The RFID reader scans **4-byte UID values** in both **Decimal and Hexadecimal formats**, allowing identification of different RFID tags. The scanned UID is processed to determine access permissions (allowed/denied) or to generate a unique sound feedback using the buzzer.


## Features
- Displays system status on an I2C LCD.
- Switch button to toggle ON/OFF state.
- Boot animation for better user experience.
- Serial output for debugging.
- Reads RFID UID in both Decimal and Hexadecimal formats.
- Prevents duplicate readings within a timeout period.
- Buzzer feedback with different melodies for accepted, denied, and custom RFID tags.
- WiFi connectivity to enable network-based functionalities.
- Uses a **`secrets.h` file** for storing sensitive credentials (WiFi SSID & password).

## Components
- ESP32 WROOM 32D
- 20x4 I2C LCD
- Switch (ON/OFF button)
- RDM6300 RFID Reader (125kHz)
- Buzzer (PWM-controlled)
- Silicon Labs CP210x USB to UART

## Installation
1. **Clone the repository**
   ```sh
   git clone https://github.com/ridwanpranata/esp32-rfid-reader.git
   cd esp32-rfid-reader
   ```
2. **Copy `secrets.example.h` file in the `include/` directory to `include/secrets.h`**
   ```sh
   cp include/secrets.example.h include/secrets.h
   ```
   **Note:** Do not commit `secrets.h` to GitHub. Add it to `.gitignore`.
3. **Open in VSCode with PlatformIO**
4. **Install dependencies** (Ensure you have the required library)
   ```sh
   pio lib install "marcoschwartz/LiquidCrystal_I2C@^1.1.4"
   ```
5. **Connect ESP32 to your PC**
6. **Upload the code**
   ```sh
   pio run --target upload
   ```
7. **Monitor Serial Output**
   ```sh
   pio device monitor --baud 115200
   ```

## Wiring
### LCD Wiring
| ESP32 Pin | LCD Pin  |
|-----------|---------|
| 21 (SDA)  | SDA     |
| 22 (SCL)  | SCL     |
| GND       | GND     |
| 5V        | VCC     |

### Switch Wiring
| ESP32 Pin | Switch Pin |
|-----------|------------|
| 12        | One side   |
| GND       | Other side |

### RFID Reader Wiring
| ESP32 Pin | RFID Reader Pin |
|-----------|----------------|
| 16        | TX             |
| GND       | GND            |
| 3.3V      | VCC            |

### Buzzer Wiring
| ESP32 Pin | Buzzer Pin |
|-----------|------------|
| 5         | Buzzer     |
| GND       | GND        |

## How It Works
1. The system initializes the LCD and displays a **boot animation**.
2. The switch button toggles the system state **ON/OFF**.
3. The LCD displays the updated status dynamically.
4. The RFID reader continuously listens for a tag scan.
5. If a RFID tag is detected:
   - The UID is displayed in Decimal format on the LCD.
   - The Serial Monitor outputs show the UID in both Decimal and Hexadecimal formats.
   - A melody plays on the buzzer based on the tag's UID.
   - Specific UIDs have predefined sounds (allowed/denied sounds).
   - Other UIDs generate a melody based on their last four digits.
6. Duplicate RFID Tag are ignored within a configurable timeout period (RFID_READ_TIMEOUT).
7. The system connects to WiFi using credentials from `secrets.h`.

## WiFi Integration
- The ESP32 will attempt to connect to WiFi at startup using credentials stored in `secrets.h`.
- If successful, the **IP address** is displayed in the Serial Monitor.
- This WiFi connection can be used for future network-related functionalities.

## Sound Feedback
| RFID UID         | Buzzer Sound |
|------------------|--------------|
| RFID_ALLOWED_UID | "Allowed" melody |
| RFID_DENIED_UID  | "Denied" melody |
| Other UIDs       | Custom melody based on last 4 digits |

## Serial Monitor Output Example
```
System ON
Connecting to WiFi...
WiFi connected!
IP Address: 192.168.1.100
System initialized. Waiting for RFID...
RFID UID Decimal (4 Byte): 0012345678
RFID UID Hexadecimal: 12AB34CD
Playing melody for UID last 4 digits: 5678
```

## File Structure
```
root/
│-- include/
│   ├── secrets.h (Ignored in Git)
│   ├── secrets.example.h
│-- lib/
│   ├── LiquidCrystal/
│   │   ├── LiquidCrystal.h
│   │   ├── LiquidCrystal.cpp
│   ├── RFIDReader/
│   │   ├── RFIDReader.h
│   │   ├── RFIDReader.cpp
│   ├── BuzzerTone/
│   │   ├── BuzzerTone.h
│   │   ├── BuzzerTone.cpp
│   ├── RFIDMelody/
│   │   ├── RFIDMelody.h
│   │   ├── RFIDMelody.cpp
│-- src/
│   ├── main.cpp
│-- platformio.ini
│-- README.md
```

## License
This project is open-source under the **MIT License**.

## Author
**Ridwan Pranata** - [GitHub](https://github.com/ridwanpranata)

Feel free to contribute and improve this project! 🚀

