#include <Wire.h>
#include <SensirionI2cScd4x.h>

#define SDA_PIN D3   // GPIO4
#define SCL_PIN D6   // GPIO5

SensirionI2cScd4x scd4x;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);
  scd4x.begin(Wire, SCD41_I2C_ADDR_62);

  scd4x.startPeriodicMeasurement();
  Serial.println("SCD41 connected, measuring...");
}

void loop() {
  uint16_t co2;
  float temp, hum;

  delay(5000);  // SCD41 updates every ~5 sec

  if (scd4x.readMeasurement(co2, temp, hum) == 0) {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" ppm | ");

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C | ");

    Serial.print("RH: ");
    Serial.print(hum);
    Serial.println(" %");
  } else {
    Serial.println("SCD41 read error");
  }
}
