#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(9);
DallasTemperature sensors(&oneWire);

SoftwareSerial mySerial(3, 2); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  pinMode(4, INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
  sensors.begin();
  mySerial.println("AT");

}

void loop()
{
  int vib = digitalRead(4);
  sensors.requestTemperatures();
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.print("degree Celcius  |  ");
  Serial.print("   viberation:  ");
  Serial.println(vib);

  if (sensors.getTempCByIndex(0)>40 && vib==0) {
   call(); 
  }
  delay(100);
}



void call() {
  updateSerial();
  mySerial.println("ATD+ +2349060429452;");
  updateSerial();
  delay(15000); // wait for 20 seconds...
  mySerial.println("ATH"); //hang up
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
