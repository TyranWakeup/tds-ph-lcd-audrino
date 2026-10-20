#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "GravityTDS.h"

#define TdsSensorPin A0
#define PhSensorPin A1
#define BuzzerPin 8

GravityTDS gravityTds;
// If the screen remains blank, try changing 0x27 to 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);

float temperature = 25.0;
float tdsValue = 0;
float phValue = 0;

void setup() {
    Serial.begin(115200);

    // Setup Buzzer pin
    pinMode(BuzzerPin, OUTPUT);
    digitalWrite(BuzzerPin, LOW);

    // Inisialisasi Sensor TDS
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);  
    gravityTds.setAdcRange(1024);  
    gravityTds.begin();  

    // Inisialisasi LCD
    lcd.init();                       
    lcd.backlight(); 
    
    lcd.setCursor(0, 0);
    lcd.print("Inisialisasi...");
    delay(2000);
    lcd.clear();
}

void loop() {
    // --- Baca Sensor TDS ---
    gravityTds.setTemperature(temperature);  
    gravityTds.update();  
    tdsValue = gravityTds.getTdsValue();  

    // --- Baca Sensor pH ---
    int sensorValue = analogRead(PhSensorPin);
    float voltage = sensorValue * (5.0 / 1024.0);
    phValue = 7.0 + ((2.5 - voltage) / 0.18); 

    // --- Cetak ke Serial Monitor ---
    Serial.print("TDS: ");
    Serial.print(tdsValue, 0);
    Serial.print(" ppm | pH: ");
    Serial.println(phValue, 2);

    // --- Logika Buzzer (Peringatan pH di luar 6.5 - 8.5) ---
    if (phValue < 6.5 || phValue > 8.5) {
        digitalWrite(BuzzerPin, HIGH);
    } else {
        digitalWrite(BuzzerPin, LOW);
    }

    // --- Tampilkan ke LCD 1602 ---
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("TDS: ");
    lcd.print(tdsValue, 0);
    lcd.print(" ppm");
    
    lcd.setCursor(0, 1);
    lcd.print("pH:  ");
    lcd.print(phValue, 2);

    delay(1000);
}