#include <BH1750.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <LiquidCrystal_I2C.h>
#define SEALEVELPRESSURE_HPA (1013.25)
#include "Adafruit_VEML6070.h"
#include <MechaQMC5883.h>
BH1750 lightMeter(0x23);
Adafruit_BME680 bme;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_VEML6070 uv = Adafruit_VEML6070();
#define VEML6070_ADDR_L (0x39) ///< Low address
MechaQMC5883 qmc;
void setup() {
  
    Serial.begin(9600);
    Wire.begin();
    lcd.begin();
    uv.begin(VEML6070_1_T);
    lcd.backlight();
    lcd.setCursor(0,1);
    lcd.print(" STM32F103C8T6");
      lightMeter.begin();
  if (!bme.begin()) {
    lcd.println(("Could not find a valid BME680 sensor, check wiring!"));
    while (1);
  }
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
  lcd.clear();
}

void loop() {

  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }

  delay(50); // This represents parallel work.

  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  float lux = lightMeter.readLightLevel();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LUX ");
  lcd.setCursor(0,1);
  lcd.print(lux);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("temp ");
  lcd.setCursor(0,1);
  lcd.print(bme.temperature);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(("Pressure hPa = "));
  lcd.setCursor(0,1);
  lcd.print(bme.pressure / 100.0);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(("Humidity % = "));
  lcd.setCursor(0,1);
  lcd.print(bme.humidity);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(("UV Index"));
  lcd.setCursor(0,1);
  lcd.print((uv.readUV()/230));
  delay(1000);
  int x,y,z;
  qmc.read(&x,&y,&z);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(("compas"));
  lcd.setCursor(0,1);/*
  String xx=String(x);
  String yy=String(y);
  String zz=String(z);
  String asd="x"+xx+" y"+yy+" z"+zz;*/
  float heading = atan2(y, x);
  float declinationAngle = 0.0878;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
   
  float headingDegrees = heading * 180/M_PI; 

  lcd.print(headingDegrees);
  delay(1000);












  
}
