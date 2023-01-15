#include <Wire.h>

byte  address;
//int I2CDevices;
byte error=1;
int I2CDevices = 0;
void setup()
{
  pinMode(PC13, OUTPUT);
  Serial.begin(9600);
  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);                       // wait for a second
  Serial.println("ASDASD");
  Wire.begin();

}

void loop()
{

Serial.println("Scanning for I2C Devices…");


for (address = 1; address < 127; address++ )
{
Wire.beginTransmission(address);
error = Wire.endTransmission();
if (error == 0)
{
  Serial.println(address, HEX);
Serial.print("I2C device found at address 0x");

if (address < 16)
Serial.print("0");
Serial.print(address, HEX);
Serial.println(" !");
I2CDevices++;
}
else if (error == 4)
{
Serial.print("Unknown error at address 0x");
if (address < 16){
}
Serial.println(address, HEX);
}
}
if (I2CDevices == 0)
Serial.println("No I2C devices found\n");
else
Serial.println("****\n");

delay(5000);
}
