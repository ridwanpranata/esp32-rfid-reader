#include "LiquidCrystal.h"
#include "LiquidCrystal_I2C.h"

LiquidCrystal::LiquidCrystal(uint8_t column, uint8_t row) 
    : _column(column), _row(row), _lcd(0x27, column, row) {} // Objek langsung, tidak pakai pointer


void LiquidCrystal::init()
{
    _lcd.init();   // Pakai titik karena bukan pointer
    _lcd.backlight();
    _lcd.setCursor(0, 0);
    _lcd.print("[RFID Reader]");
    
    boatingAnimation();

    setLineText(2, "UID: - ");
    setLineText(3, "Result: - ");
}

void LiquidCrystal::setLineText(uint8_t row, const char* text)
{
    _lcd.setCursor(0, row);
    
    // Hapus baris terlebih dahulu (karena print tidak otomatis menghapus karakter sebelumnya)
    for (int i = 0; i < _column; i++) {  
        _lcd.print(" ");
    }
    
    // Kembali ke awal baris
    _lcd.setCursor(0, row);
    _lcd.print(text);
}

void LiquidCrystal::boatingAnimation()
{
    const char* baseText = "Status: Booting";  
    char animText[20]; // Buffer untuk teks
    
    for (int i = 0; i < 3; i++) {  // Ulangi animasi 3 kali
        for (int j = 0; j < 4; j++) {  // Animasi titik ". .. ..." -> "   "
            char dots[] = "...";  // Tiga titik maksimal
            dots[j] = '\0'; // Potong string sesuai `j`
            
            snprintf(animText, sizeof(animText), "%s%s", baseText, dots);  
            setLineText(1, animText);  
            delay(500);
        }
    }
    
    setLineText(1, "Status: System Ready");  
    delay(2000);
}