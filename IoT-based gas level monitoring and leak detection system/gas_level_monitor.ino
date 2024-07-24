// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
//#define BLYNK_TEMPLATE_ID "TMPLlh8o1jPo"
//#define BLYNK_DEVICE_NAME "Getting started"
//#define BLYNK_AUTH_TOKEN "57xQ3I8tMFX5tsqzAuleqJsI0xU2wfaA"

#define BLYNK_TEMPLATE_ID "TMPL2G6J2u_h-"
#define BLYNK_TEMPLATE_NAME "Gas level Monitor"
#define BLYNK_AUTH_TOKEN "-xQDvkCPrXZ2_N9xUqjqFp1Q1Zza2Ham"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TimiNet";
char pass[] = "987654321";

BlynkTimer timer;



int pot=26;
//BLYNK_WRITE(V0)
BLYNK_WRITE(V1)
{
  int pinValue=param.asInt();
  digitalWrite(25,pinValue);
  }
void setup()
{
  pinMode(pot,INPUT);
  pinMode(25,OUTPUT);
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

}

void loop()
{
   Blynk.run();
  
//  Blynk.run();
//   int potvalue=analogRead(36);
//  Blynk.virtualWrite(V1,potvalue);
}
