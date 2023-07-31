#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <QMC5883LCompass.h>
#include <ESPAsyncWebServer.h>
#include "BME280-SOLDERED.h"
AsyncWebServer server(80);
BME280 bme280;
QMC5883LCompass compass;
           int x, y, z;
float temperature=0.0;
float humidity=0.0;
float pressure=0.0;
float altitude=0.0;
float offset = 0; // offset in °C
          int azimut;
          float bearing;
          char rizik='l';
int calibrationData[3][2];
bool changed = false;
bool done = false;
int t = 0;
int c = 0;
int uvss =0;
bool calibrated = false;
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_BME280.h>
#include "VEML6070.h"
#include <EasyButton.h>
//#include <SPI.h>
#include <SD.h>
#include <MechaQMC5883.h>
#include <Adafruit_GFX.h>
    uint16_t uvs;
    int risk_level;
    char* risk;
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
#include "Digital-light-sensor-with-easyc-SOLDERED.h"

#include <Adafruit_HMC5883_U.h>
//#define SEALEVELPRESSURE_HPA (1013.25)
unsigned long delayTime;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
MechaQMC5883 qmc;
float kompas;
IPAddress IP;
//Adafruit_BME280 bme; // I2C
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino resetpin)
String asd ="";
#define BUTTON_PIN 0
LTR507 sensorr;
unsigned long startTime = 0;
unsigned long currentTime = 0;
const unsigned long interval = 5000; // 5 seconds in milliseconds
int lig = 0;
EasyButton button(BUTTON_PIN);

File myFile;
// Counter for button presses.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int buttonPressCounter = 0;
void onPressed() {
    buttonPressCounter++;
    Serial.print("Button has been pressed! Count: ");
    Serial.println(buttonPressCounter);

    // Check if the button has been pressed 8 times.
    if (buttonPressCounter <= 7) {
        Serial.println("Button pressed 8 times!");
        // Add your desired code here after the button is pressed 8 times.
    }
    if(buttonPressCounter==9){
      buttonPressCounter=1;
      }
}
uint16_t light;
void setup()
{
  Serial.begin(9600);
  compass.init();
Serial.print("Initializing SD card...");
  bme280.begin();
      // Initialize the button.
      VEML.begin();
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
   // for (;;); // Don't proceed, loop forever
  }
    button.begin();
    button.onPressed(onPressed);
    bool status;
    qmc.init();
    
  //status = bme.begin(0x76, &Wire); 
  Serial.print("Initializing SD card...");
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    //return;
  }
  Serial.println("initialization done.");
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
   // myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening teest.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("error opening teest.txt");
  }
  Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  sensorr.init();        //Initialize sensor
  Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  // Clear the buffer.
 while (Serial.available()) {
  Serial.read();
}
            while (calibrated!=1){
        // If not calibrated
        Serial.println("CALIBRATING... Keep moving your sensor around.");
             display.clearDisplay();
   display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("CALIBRATING... Keep moving your sensor around.");
  display.display();
      // Read compass values
      compass.read();
 
      // Return XYZ readings
      x = compass.getX();
      y = compass.getY();
      z = compass.getZ();
 
      changed = false;
 
      if(x < calibrationData[0][0]) {
        calibrationData[0][0] = x;
        changed = true;
      }
      if(x > calibrationData[0][1]) {
        calibrationData[0][1] = x;
        changed = true;
      }
 
      if(y < calibrationData[1][0]) {
        calibrationData[1][0] = y;
        changed = true;
      }
      if(y > calibrationData[1][1]) {
        calibrationData[1][1] = y;
        changed = true;
      }
 
      if(z < calibrationData[2][0]) {
        calibrationData[2][0] = z;
        changed = true;
      }
      if(z > calibrationData[2][1]) {
        calibrationData[2][1] = z;
        changed = true;
      }
      if (changed && !done) {
         while (Serial.available()) {
            Serial.read();
          }
  Serial.println("CALIBRATING... Keep moving your sensor around.");
        c = millis();
      }
        t = millis();
 
 
      if ( (t - c > 5000) && !done) {
        done = true;
        display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("Gotovo.");
  display.display();
  buttonPressCounter=1;
        Serial.println();
 
        Serial.print("compass.setCalibration(");
        Serial.print(calibrationData[0][0]);
        Serial.print(", ");
        Serial.print(calibrationData[0][1]);
        Serial.print(", ");
        Serial.print(calibrationData[1][0]);
        Serial.print(", ");
        Serial.print(calibrationData[1][1]);
        Serial.print(", ");
        Serial.print(calibrationData[2][0]);
        Serial.print(", ");
        Serial.print(calibrationData[2][1]);
        Serial.println(");");
 
        compass.setCalibration( calibrationData[0][0], calibrationData[0][1], calibrationData[1][0],
                                calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);
        calibrated = true;
        }

    }
     while (Serial.available()) {
  Serial.read();
}
    WiFi.softAP("MyAP", "password");
  IPAddress IP = WiFi.softAPIP();
Serial.println(IP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "  <script>\n"
                  "    function refreshPage() {\n"
                  "      setTimeout(function() {\n"
                  "        location.reload();\n"
                  "      }, 2000); // Refresh the page after 5 seconds\n"
                  "    }\n"
                  "    refreshPage();\n"
                  "  </script>\n"
                  "</head>\n"
                  "<body>\n"
                  "  <h1>Temperatura: " + String(temperature) + "</h1>\n"
                  "  <h1>Vlaga: " + String(humidity) + " %</h1>\n"
                  "  <h1>Tlak: " + String(pressure) + " hPa</h1>\n"
                  "  <h1>Nadmorska visina: " + String(altitude) + " m</h1>\n"
                  "  <h1>Lux: " + String(lig) + " m</h1>\n"
                  "  <h1>UV faktor: " + String(uvss) + " m</h1>\n"
                  
                  "</body>\n"
                  "</html>\n";
    request->send(200, "text/html", html);
  });

  server.begin();
  delay(200);
 
}
 
void loop() {
  /*    if(sensorr.Available())  //Check if sensor is available
  {
    uint16_t light; 
    light = sensorr.getLightIntensity(); // get light intensity
    if(light!=0){
      Serial.print("Light intensity: ");
      Serial.print(light);              // print light intensity
        Serial.println("");}
  }*/

  
    currentTime = millis();

 // Check if 5 seconds have elapsed
 button.read();
 
 if (currentTime - startTime >= interval) {

  delay(25);
    uint16_t uvs = VEML.read_uvs_step();
    int risk_level = VEML.convert_to_risk_level(uvs);
    char* risk = VEML.convert_to_risk_char(risk_level);
    rizik =*risk;
    uvss=uvs;
  Serial.print(bme280.readTemperature());
        if(sensorr.Available())  //Check if sensor is available
  {
   // uint16_t light; 
    light = sensorr.getLightIntensity(); // get light intensity
    lig = light;
  }
  compass.read();
  temperature = bme280.readTemperature() + offset;
  humidity = bme280.readHumidity();
  pressure = bme280.readPressure();
  altitude = bme280.readAltitude();
 // light = sensorr.getLightIntensity(); // get light intensity
          // Return XYZ readings
          x = compass.getX();
          y = compass.getY();
          z = compass.getZ();
          azimut = compass.getAzimuth();
          bearing = compass.getBearing(azimut);
    myFile = SD.open("Weatherinfo.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");  
    // close the file:
  myFile.println("Temp");
  myFile.println(temperature);
  myFile.println("Tlak");
  myFile.println(pressure);
  myFile.println("Visina");
  myFile.println(altitude);
  myFile.println("Vlaga");
  myFile.println(humidity);
  myFile.println("UV");
  myFile.println(uvs);
  myFile.println("UV rizik");
  myFile.println(*risk);
  myFile.println("LUX");
  light = sensorr.getLightIntensity(); // get light intensity
  myFile.println(light);
            if((azimut < 22.5)  || (azimut > 337.5 ))  myFile.print("North     ");
          if((azimut > 22.5)  && (azimut < 67.5 ))   myFile.print("North-East");
          if((azimut > 67.5)  && (azimut < 112.5 ))  myFile.print("East      ");
          if((azimut > 112.5) && (azimut < 157.5 ))  myFile.print("South-East");
          if((azimut > 157.5) && (azimut < 202.5 ))  myFile.print("South     ");
          if((azimut > 202.5) && (azimut < 247.5 ))  myFile.print("SOuth-West");
          if((azimut > 247.5) && (azimut < 292.5 ))  myFile.print("West      ");
          if((azimut > 292.5) && (azimut < 337.5 ))  myFile.print("North-West");
 
          myFile.print(" Azimuth: ");myFile.print(azimut);
          myFile.print(" Bearing: ");myFile.print(bearing);
 
          myFile.print(" - X: ");
          myFile.print(x);
          myFile.print(" Y: ");
          myFile.print(y);
          myFile.print(" Z: ");
          myFile.print(z);
    myFile.close();
    Serial.println("done.");  
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
   startTime = currentTime;
  }  
  
  if(buttonPressCounter==1){
      //delay(1000);
      display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("Temp");
  display.println(temperature);
  display.display();
  //temperature = bme280.readTemperature() + offset;
  Serial.println(temperature);  
    }
    if(buttonPressCounter==2){
        //delay(1000);
  display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("Vlaga");
  display.println(humidity);
  display.display();
 // humidity = bme280.readHumidity();
  Serial.println(humidity);  
    }
    if(buttonPressCounter==3){
        //delay(1000);
  display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("Tlak");
  display.println(pressure);
  display.display();
 // pressure = bme280.readPressure();
  Serial.println(pressure);  
    }
        if(buttonPressCounter==4){
        //delay(1000);
  display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("Visina");
  display.println(altitude);
  display.display();
 // altitude = bme280.readAltitude();
  Serial.println(altitude);  
    }
            if(buttonPressCounter==5){
        //delay(1000);
      display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("UV");
  display.println(uvss);
  display.display();
  Serial.println("done.");  
    }
   if(buttonPressCounter==6){
      display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(2);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  display.println("UV rizik");
  display.println(rizik);
  display.display();
  Serial.println("done.");  
    }
       if(buttonPressCounter==7){
        //delay(1000);
      if(sensorr.Available())  //Check if sensor is available
  {
      Serial.print("Light intensity: ");
      Serial.print(lig);              // print light intensity
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE); // 'inverted' text
      display.setCursor(0, 0);
      display.println("LUX");
      display.println(lig);
      display.display();
  }

    }
           if(buttonPressCounter==8){
      display.clearDisplay();
  // Display Inverted Text
  display.setTextSize(1);
  display.setTextColor(WHITE); // 'inverted' text
  display.setCursor(0, 0);
  Serial.println("done.");  
 //display.println("LUX");
            if((azimut < 22.5)  || (azimut > 337.5 ))  display.print("North     ");
          if((azimut > 22.5)  && (azimut < 67.5 ))   display.print("North-East");
          if((azimut > 67.5)  && (azimut < 112.5 ))  display.print("East      ");
          if((azimut > 112.5) && (azimut < 157.5 ))  display.print("South-East");
          if((azimut > 157.5) && (azimut < 202.5 ))  display.print("South     ");
          if((azimut > 202.5) && (azimut < 247.5 ))  display.print("SOuth-West");
          if((azimut > 247.5) && (azimut < 292.5 ))  display.print("West      ");
          if((azimut > 292.5) && (azimut < 337.5 ))  display.print("North-West");
 
          display.print(" Azimuth: ");display.print(azimut);
          display.print(" Bearing: ");display.print(bearing);
 
          display.print(" - X: ");
          display.print(x);
          display.print(" Y: ");
          display.print(y);
          display.print(" Z: ");
          display.print(z);
 // display.println(light);
  display.display();
  Serial.println("done."); 
    }
    }

  void myfunction(){
          display.clearDisplay();

  display.display();

}
