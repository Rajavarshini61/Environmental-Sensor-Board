#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// ---------- I2C Pins ----------
#define SDA_PIN D2   // GPIO4
#define SCL_PIN D1   // GPIO5

// ---------- I2C Addresses ----------
#define SCD4X_ADDR SCD41_I2C_ADDR_62
#define OLED_ADDR  0x3C
#define RST_PIN    -1

// ---------- Objects ----------
SensirionI2cScd4x scd4x;
SSD1306AsciiWire oled;

// ---------- Timing ----------
const unsigned long MEASURE_INTERVAL = 5000;
unsigned long lastMeasureTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // I2C Init
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000L);

  // OLED Init
  oled.begin(&Adafruit128x64, OLED_ADDR, RST_PIN);
  oled.setFont(System5x7);
  oled.clear();
  oled.println("SCD41 Init...");

  // SCD41 Init
  scd4x.begin(Wire, SCD4X_ADDR);
  delay(30);

  // Start periodic measurement
  scd4x.startPeriodicMeasurement();
  delay(500);

  oled.println("Measuring...");
  Serial.println("SCD41 connected, measuring...");
}

void loop() {
  if (millis() - lastMeasureTime < MEASURE_INTERVAL) return;
  lastMeasureTime = millis();

  uint16_t co2;
  float temperature, humidity;

  // Read sensor (returns 0 on success)
  if (scd4x.readMeasurement(co2, temperature, humidity) == 0) {

    // Serial Output
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" ppm | Temp: ");
    Serial.print(temperature, 2);
    Serial.print(" C | RH: ");
    Serial.print(humidity, 2);
    Serial.println(" %");

    // OLED Output
    oled.clear();
    oled.setCursor(0, 0);
    oled.print("CO2:  "); oled.print(co2); oled.println(" ppm");
    oled.print("Temp: "); oled.print(temperature, 1); oled.println(" C");
    oled.print("RH:   "); oled.print(humidity, 1); oled.println(" %");

  } else {
    Serial.println("SCD41 read error");

    oled.clear();
    oled.setCursor(0, 0);
    oled.println("SCD41 Error");
  }
}

