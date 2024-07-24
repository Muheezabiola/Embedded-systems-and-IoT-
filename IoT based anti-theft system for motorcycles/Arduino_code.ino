#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define API_KEY ""
#define DATABASE_URL " 
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
int Firebasemillis=0;
String sit_state="The sit is undisturbed", rest_state="The bike has not been moved", tog="nil";

#include <TinyGPS++.h>
#define rxPin 4
#define txPin 2
HardwareSerial sim800(1);
#define RXD2 16
#define TXD2 17
HardwareSerial gpsSerial(2);
TinyGPSPlus gps;
String smsStatus,senderNumber,receivedDate,msg;
boolean isReply = false;
#include "HX711.h"
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 12;
HX711 scale;
const int x =35, y=32, z=33;
int xx,yy,zz,toggle, d=0, a;
boolean sendw=true;
boolean sends=true;
boolean send_sit =true;
int loadsensormillis =0;
#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>

const char* ssid = "";
const char* password = "";

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "";  //""; 
String apiKey =  "";  //"";

String lat ="nil", lon="nil", alt="nil";
int sendmillis=0;
int sendmillis2=0;
    int last_x;
    int last_y;

void setup() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi...");
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
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);

  pinMode(x, INPUT);
  pinMode(y, INPUT);
  pinMode(23, INPUT_PULLUP);
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(22800.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();	
  Serial.println("esp32 serial initialize");
  sim800.begin(9600, SERIAL_8N1, rxPin, txPin);
  Serial.println("SIM800L serial initialize");
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("neogps serial initialize");
  delay(1000);
  sim800.println("AT"); //Once the handshake test is successful, it will back to OK

}


void loop() {
  gyro();
  the_weight();
  toggle= digitalRead(23);

  if (Firebase.ready() && (millis() - Firebasemillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis == 1;
    Firebasemillis = millis();
    Firebase.setString(fbdo, "/Bike/sit", sit_state);  Firebase.setString(fbdo, "/Bike/movement", rest_state);
    Firebase.setString(fbdo, "/Bike/longitude", lon); Firebase.setString(fbdo, "/Bike/lat", lat); 
    Firebase.setString(fbdo, "/Bike/Toggle state", tog); 
    delay(200);
 }

  // Serial.printlln(String(abs(last_x-xx))  abs(last_y-xx)
  // Serial.printlln(String(abs(last_x-xx)) 

  while (gpsSerial.available() > 0)
      if (gps.encode(gpsSerial.read()))
        displayInfo();
    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
      Serial.println("No GPS detected");
      while(true);
    }
   
    if (toggle==1) {
      sendw=true;
      sends=true;
      send_sit=true;
      tog="OFF";
      sit_state="The system has been reset";
      rest_state="The system has been reset";
      if (d==1){
        d=0;
        Serial.println("toggled OFF");
        delay(1000);
      last_x=xx;
      last_y=yy;
      }
    }
  if (toggle==0) {
      tog="ON";
    if (d==0) {
      d=1;
      last_x=xx;
      last_y=yy;
      Serial.println("toggled ON");
      delay(1000);
    }
      
      if ( WiFi.status() == WL_CONNECTED /*&& ( millis()-sendmillis2>60000 || */){
      //sendmillis2=millis();
      if (abs(a)>=10) {
          sit_state ="The bike is being sit on";
        if (send_sit==true) {
          send_sit=false;
      Serial.println("Someone is currently sitting on the bike, but the bike has not been moved");
      sendMessage("Someone is currently sitting on the bike, but the bike has not been moved");
      delay(1000);
        }
       }
                else  if (abs(a)<10) {
   sit_state="The sit is undisturbed";
   }
      }
      
    
      if (WiFi.status() != WL_CONNECTED/*& millis()-sendmillis2>30000*/){
      //sendmillis2=millis();
      if (abs(a)>=10){
          sit_state ="The bike is being sit on";
         if (send_sit==true) {
          send_sit=false;
      Serial.println("Someone is currently sitting on the bike, but the bike has not been moved");
      Serial.println("No Wifi Connected, opting for SMS");
      sms2 ();
      delay(1000);
         }
      }
               else  if (abs(a)<10){
   sit_state="The sit is undisturbed";
   }
      }
    

      if (WiFi.status() == WL_CONNECTED)  {
      if (abs(last_x-xx)>=2||abs(last_y-yy)>=2) {
        if (sendw==true/*millis()-sendmillis>30000*/) {
          sendw=false;
          //sendmillis=millis();
          rest_state="The bike is being moved";
          Serial.print("Theft alert!");
          sendMessage("Your bike is about to be stolen, hurry there now! Here's its current co-ordinates:");
          sendMessage("Latitide: "+ lat);
          sendMessage("Longitude: "+ lon);
          sendMessage("Altitude: "+ alt);
      }
      }
//            else {
//   sit_state="The sit is undisturbed"; rest_state="The bike has not been moved";
//  }
      }
    
      if (WiFi.status() != WL_CONNECTED) {
        if (abs(last_x-xx)>=2 || abs(last_y-yy)>=2) {
        rest_state="The bike is being moved";
        if (sends==true /*millis()-sendmillis>30000*/) {
        sends=false;
        //sendmillis=millis();
        Serial.print("Theft alert, No Wifi Connected, opting for SMS");
        delay(500);
        sms();
      }
      }
//            else {
//   sit_state="The sit is undisturbed"; rest_state="The bike has not been moved";
//  }
    }
 }

}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim800.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800.available()) 
  {
    Serial.write(sim800.read());//Forward what Software Serial received to Serial Port
  }
}

void the_weight () {
  a=abs(scale.get_units(10));    // average of 10 reading values
  Serial.print("average:\t");
  Serial.println(a);
  scale.power_down();			        // put the ADC in sleep mode
  if (millis()-loadsensormillis>1000) {
      loadsensormillis=millis();
      scale.power_up();
 }

}

void gyro () {
  xx=(map(analogRead(x),0,4096,0,100));
  yy=(map(analogRead(y),0,4096,0,100));
  zz=(map(analogRead(z),0,4096,0,100));
  Serial.print("X: "); Serial.print(map(analogRead(x),0,4096,0,100));
  Serial.print("  Y: "); Serial.print(map(analogRead(y),0,4096,0,100)); 
  Serial.print("  Z: "); Serial.println(map(analogRead(z),0,4096,0,100)); /*delay(500)*/;
}

void sms () {
  updateSerial();
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800.println("AT+CMGS=\"+2348151764707\"");// 2348151764707 change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("Your bike is about to be stolen, hurry there now! Here's its current co-ordinates: lat: "+ lat + " lon: " + lon + " alt: " + alt); //text content
  updateSerial();
  sim800.write(0x1A);

}

void sms2 () {
  updateSerial();
  sim800.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  sim800.println("AT+CMGS=\"+2348151764707\"");// 2348151764707 change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  sim800.print("Someone is currently sitting on the bike, but the bike has not been moved"); //text content
  updateSerial();
  sim800.write(0x1A);

}

void displayInfo ()
{
  if (gps.location.isValid())
  {
    lat = String(float(gps.location.lat()), 6);
    lon = String(float(gps.location.lng()), 6);
    alt =gps.altitude.meters();

    Serial.print("Latitude: ");
    Serial.print(lat);
    Serial.print("        ");
    Serial.print("Longitude: ");
    Serial.print(lon);
    Serial.print("        ");
    Serial.print("Altitude: ");
    Serial.println(alt);
    delay(100);
  }
  else
  {
    Serial.println("Location: Not Available");
    delay(100);
  }
}

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}