#include <Wire.h>
#include "max6675.h" //https://github.com/adafruit/MAX6675-library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);
int thermoDO = 8; // SO of MAX6675 module to D8
int thermoCS = 9; // CS of MAX6675 module to D9
int thermoCLK = 10; // SCK of MAX6675 module to D6
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);



void setup()
{
Serial.begin(9600);
Serial.println("GROUP ONE\n MCE 315");
// time for stabilization
delay(1000);
// initialize with the I2C addr 0x3C
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
// Clear the buffer.
display.clearDisplay();
}


void loop()
{
float t = thermocouple.readCelsius();
float tF = thermocouple.readFahrenheit();
// current temperature readout
Serial.print("Deg C = ");
Serial.println(t);
Serial.print("\t Deg F = ");
Serial.println(tF);
Serial.println();
// Clear the buffer.
display.clearDisplay();
//Celsius
display.setTextColor(WHITE);
display.setTextSize(1);
display.setCursor(0, 7);
display.print("Temperature: ");
display.setTextSize(2);
display.setCursor(0, 18);
display.print(t);
display.setTextSize(1);
display.cp437(true);
display.write(167);
display.setTextSize(2);
display.print("C");
//Celsius
display.setTextSize(2);
display.setCursor(0, 45);
display.print(tF);
display.setTextSize(1);
display.cp437(true);
display.write(167);
display.setTextSize(2);
display.print("F");
display.display();
delay(1000);
}
