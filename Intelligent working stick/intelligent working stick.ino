#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define WIFI_SSID 
#define WIFI_PASSWORD 
const String YOUR_PHONE =  
const String YOUR_APIKEY =  
const String URL = "http://api.whatabot.net/whatsapp/sendMessage?";
WiFiUDP ntpUDP;

#define echo1  5  //d1
#define trig1  4  //d2
#define echo2 12  //d6
#define trig2 14  //d5
#define vibt  13  //d7
#define sound 15  //d8
#define button  2  //d4
int distance1, distance2, duration1, duration2, push, a=1;
boolean send =true, send2=true;
int beep_delay;
double vib;
void setup() {
  pinMode(A0, INPUT);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  pinMode(sound, OUTPUT);
  pinMode(vibt, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   while(WiFi.status() != WL_CONNECTED && millis()< 10000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP()); 
  }
  else if (WiFi.status() != WL_CONNECTED) {
  Serial.println("");
  Serial.print("Haven't connected to Wifi Yet...");
  delay(500);
  Serial.println("Skipping to the next step for now");
  delay(500);
  }
//sendWhatsapp("Hello!%20Your%20old%20man%20walking%20stick%20just%20experienced%20a%20fall,%20you%20might%20want%20to%20check%20up%20on%20him.");
    digitalWrite(sound,1);
    delay(1000);
    digitalWrite(sound,0);
}

void loop() {
  // digitalWrite(sound,0);
  // digitalWrite(vibt, 0);
  vib=analogRead(A0);
  ultrasonic();
  push=digitalRead(button);
  Serial.println(vib);
  if (push==1) {
  }

  if (push==0) { 
  if (a==1) {
    a=0;
    send=true;
    send2=true;
    Serial.println("button pushed");
    Serial.println("System reset mode");
    delay(2000);
  }  

  else if (a==0) {
    a=1;
    Serial.println("button pushed"); 
      if (send2=true) {
      send2=false;
      sendWhatsapp("Hello!%20Your%20old%20man%20need%20your%20attention%20pllease%20hurry%20here%20now!.");
      Serial.println("Message sent");
      digitalWrite(vibt, 1);
      delay(1000);
      digitalWrite(vibt, 0);
      delay(3000);
      }
      }
      else {
    digitalWrite(sound,0);
    digitalWrite(vibt, 0);
  } 
    }
     else {
    digitalWrite(sound,0);
    digitalWrite(vibt, 0);
  } 
  

  if (distance1 < 25 || distance2 < 25)  {
    delay(100);
    if (distance1 < 25 || distance2 < 25) {
    digitalWrite(vibt, 1);
    digitalWrite(sound,1);
    delay(1000);
    }

    else {
    digitalWrite(sound,0);
    digitalWrite(vibt, 0);
    }
  } 
      else {
    digitalWrite(sound,0);
    digitalWrite(vibt, 0);
    }
  
  if (vib>15 && vib<=210) {
    delay(2000);
    if (vib>12 && vib<=210) {
    if (send==true) {
      send=false;
      sendWhatsapp("Hello!%20Your%20old%20man%20walking%20stick%20just%20experienced%20a%20fall,%20you%20might%20want%20to%20check%20up%20on%20him.");
      Serial.println("Message sent");
      digitalWrite(vibt, 1);
      delay(1000);
      beep();
      digitalWrite(vibt, 0);
      delay(2000);   
    }
    }
    else {
        digitalWrite(sound,0);
    digitalWrite(vibt, 0);
    }
  }
      else {
        digitalWrite(sound,0);
    digitalWrite(vibt, 0);
    }

}



void beep () {
  beep_delay=millis();
  while (millis()-beep_delay<3000) {
    digitalWrite(sound,1);
    delay(300);
    digitalWrite(sound,0);
    delay(300);
}
    digitalWrite(sound,0);
}

  void ultrasonic(){
    digitalWrite(trig1, 0); delayMicroseconds(2);   digitalWrite(trig1, 1); delayMicroseconds(10); digitalWrite(trig1, 0);  
    duration1=pulseIn(echo1, 1);  distance1=duration1*0.034/2;
    digitalWrite(trig2, 0); delayMicroseconds(2); digitalWrite(trig2, 1); delayMicroseconds(10); digitalWrite(trig2, 0);
    duration2=pulseIn(echo2, 1);  distance2=duration2*0.034/2;
    Serial.print("distance1: "); Serial.print(distance1); Serial.print(" cm     "); 
    Serial.print("distance2: "); Serial.print(distance2); Serial.print(" cm       ");
  }

void sendWhatsapp (String text) {
  WiFiClient client;
  HTTPClient https;
  String url = URL + "apikey=" + YOUR_APIKEY + "&text=" +text + "&phone=" + YOUR_PHONE;
  https.begin(client, url);
  int httpCode=https.GET();
  https.end();
}