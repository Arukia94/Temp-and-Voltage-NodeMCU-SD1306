#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Poppins_SemiBold18pt7b.h"  // Include the custom Poppins SemiBold font

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Voltage sensor parameters
#define ADC_MAX 1023.0    // ADC resolution (10-bit)
#define VREF 3.3          // Reference voltage of NodeMCU
#define R1 29850.0        // Resistor R1 in the divider (in ohms)
#define R2 7500.0         // Resistor R2 in the divider (in ohms)

// Calibration parameters
float calibrationFactor = 0.95; // Adjusted calibration factor

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  // Read raw ADC value
  int adcValue = analogRead(A0);

  // Convert ADC value to voltage (scaled for divider)
  float rawVoltage = (adcValue / ADC_MAX) * VREF * ((R1 + R2) / R2);

  // Apply calibration adjustment
  float calibratedVoltage = rawVoltage * calibrationFactor;

  // Clear the display
  display.clearDisplay();

  // Set custom Poppins SemiBold font
  display.setFont(&Poppins_SemiBold18pt7b); // Use the new Poppins SemiBold font
  display.setCursor(0, 40);                 // Adjust position for larger font
  display.print(calibratedVoltage, 2);     // Display calibrated voltage
  display.print("V");

  // Show the content on OLED
  display.display();

  // Print raw and calibrated voltage to Serial Monitor for debugging
  Serial.print("Raw Voltage: ");
  Serial.print(rawVoltage);
  Serial.print(" V, Calibrated Voltage: ");
  Serial.print(calibratedVoltage);
  Serial.println(" V");

  delay(1000);  // Update every second
}
