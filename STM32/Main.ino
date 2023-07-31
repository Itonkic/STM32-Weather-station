#include <BH1750.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
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
unsigned long currentMilliss;
unsigned long previousMilliss = 0;
float lux;
float stupnjevi;
float tlak;
float vlaga;
int uvv;
float kompas;
File file;
int lastState = HIGH;
int bLUX = PA10;
int btemp = PA9;
int btlak = PA8;
int bvlaga = PB15;
int buv = PB14;
int bkompas = PB13;

float readcompass() {
  int x,y,z;
  qmc.read(&x,&y,&z);
  String xx=String(x);
  String yy=String(y);
  String zz=String(z);
  String asd="x"+xx+" y"+yy+" z"+zz;
  float heading = atan2(y, x);
  float declinationAngle = 0.0878;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
  }
  
void setup() {
  
    Serial.begin(9600);
    Wire.begin();
    qmc.init();
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
 // lcd.clear();
}

void loop() {

  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return;
  }
  if (!SD.begin()) {
    Serial.println("begin failed");
    return;
  }
  delay(50); // This represents parallel work.
  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return;
  }
  currentMilliss = millis();
  unsigned long intervall = 5000;
  if (currentMilliss - previousMilliss > intervall) {
    lux = lightMeter.readLightLevel();
    stupnjevi = bme.temperature;
    tlak = (bme.pressure / 100.0);
    vlaga = bme.humidity;
    uvv = (uv.readUV()/230);
    kompas = readcompass();
    file = SD.open("data.TXT", FILE_WRITE);
    file.println("Temperatura "+String(stupnjevi));
    file.println("Lux "+String(lux));
    file.println("Tlak "+String(tlak));
    file.println("Vlaga "+String(vlaga));
    file.println("UV "+String(uvv));
    file.println("Kompas "+String(kompas));
    file.close();
    previousMilliss = currentMilliss;
  }
  
  int buttonstate =digitalRead(bLUX);
  if (lastState==HIGH && buttonstate == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LUX ");
      lcd.setCursor(0,1);
      lcd.print(lux);
    }
  int buttonstate1 =digitalRead(btemp);
  if (lastState==HIGH && buttonstate1 == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("temp ");
      lcd.setCursor(0,1);
      lcd.print(stupnjevi);
    }
  int buttonstate2 =digitalRead(btlak);
  if (lastState==HIGH && buttonstate2 == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Pressure hPa =  ");
      lcd.setCursor(0,1);
      lcd.print(tlak);
    }
    
  int buttonstate3 =digitalRead(bvlaga);
  if (lastState==HIGH && buttonstate3 == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Humidity % = ");
      lcd.setCursor(0,1);
      lcd.print(vlaga);
    }
      int buttonstate4 =digitalRead(buv);
  if (lastState==HIGH && buttonstate4 == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("UV index ");
      lcd.setCursor(0,1);
      lcd.print(uvv);
    }
      int buttonstate5 =digitalRead(bkompas);
  if (lastState==HIGH && buttonstate5 == LOW){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("compas ");
      lcd.setCursor(0,1);
      lcd.print(readcompass());
    }
  
}
