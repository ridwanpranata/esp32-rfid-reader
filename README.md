# RFID Reader with ESP32 and I2C LCD

This project is an RFID reader system using an **ESP32 WROOM 32D**, a **20x4 I2C LCD**, and a **switch button** for toggling system status. It uses the **LiquidCrystal_I2C** library for LCD control and is developed using **VSCode with PlatformIO**.

## Features
- Displays system status on an I2C LCD.
- Switch button to toggle ON/OFF state.
- Boot animation for better user experience.
- Serial output for debugging.

## Components
- **ESP32 WROOM 32D**
- **20x4 I2C LCD**
- **Switch (ON/OFF button)**
- **Silicon Labs CP210x USB to UART**

## Installation
1. **Clone the repository**
   ```sh
   git clone https://github.com/yourusername/your-repo.git
   cd your-repo
   ```
2. **Open in VSCode with PlatformIO**
3. **Install dependencies** (Ensure you have the required library)
   ```sh
   pio lib install "marcoschwartz/LiquidCrystal_I2C@^1.1.4"
   ```
4. **Connect ESP32 to your PC**
5. **Upload the code**
   ```sh
   pio run --target upload
   ```
6. **Monitor Serial Output**
   ```sh
   pio device monitor --baud 115200
   ```

## Wiring
| ESP32 Pin | LCD Pin  |
|-----------|---------|
| 21 (SDA)  | SDA     |
| 22 (SCL)  | SCL     |
| GND       | GND     |
| 5V        | VCC     |

| ESP32 Pin | Switch Pin |
|-----------|------------|
| 12        | One side   |
| GND       | Other side |

## How It Works
1. The system initializes the LCD and displays a **boot animation**.
2. The switch button toggles the system state **ON/OFF**.
3. The LCD displays the updated status dynamically.
4. Serial monitor logs system state changes.

## File Structure
```
project_root/
│-- lib/
│   ├── LiquidCrystal/
│   │   ├── LiquidCrystal.h
│   │   ├── LiquidCrystal.cpp
│-- src/
│   ├── main.cpp
│-- platformio.ini
│-- README.md
```

## License
This project is open-source under the **MIT License**.

## Author
Ridwan Pranata - [GitHub](https://github.com/ridwanpranata)

Feel free to contribute and improve this project! 🚀