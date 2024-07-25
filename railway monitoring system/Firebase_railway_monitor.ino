#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
//#include <Firebase_ESP_CLient.h>
#define WIFI_SSID "TimiNet"
#define WIFI_PASSWORD "987654321"
#define API_KEY "AIzaSyBhG-p3iRDPL1K5o7YRm96cG9t7SK_WVUo" //secret: rlBMa4ZMLq5hodKpfw6EPFMFSuDisXZVYFDDTcbi
#define DATABASE_URL "https://railway-monitor-default-rtdb.firebaseio.com" 
String a, b;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0, railmillis=0;
#define echo1 16  
#define trig1 17  
#define echo2 12  
#define trig2 14  
int distance1, distance2, duration1, duration2, kpa, boss, kpag=27, bos=26;


void setup() {
  pinMode(kpag, INPUT_PULLUP);
  pinMode(bos, INPUT_PULLUP);
  pinMode(echo1, INPUT);
  pinMode(echo2, INPUT);
  pinMode(trig1, OUTPUT);
  pinMode(trig2, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
 /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
    // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);
 

}

void loop() {
  kpa=digitalRead(kpag);
  boss=digitalRead(bos);
  Serial.println("kpa: ");
  Serial.println(kpa);
  Serial.println("boss: ");
  Serial.println(boss);

    ultrasonic();
    if (distance1<=20 && kpa ==0 ) {
      a="Railway-is-occupied";
    }
    else if (distance1>20 && kpa ==0) {
      a="Railway-is-clear";
    }
        else if (distance1>20 && kpa ==1) {
      a="Railway-is-damaged";
    }
            else if (distance1<=20 && kpa ==1) {
      a="Blocked-and-damaged";
    }



    if (distance2<=20  && boss ==0) {
      b="Railway-is-occupied";
    }
    else if (distance2>20 && boss ==0) {
      b="Railway-is-clear";
    }
          else if (distance2>20 && boss ==1) {
      b="Railway-is-damaged";
    }
            else if (distance2<=20 && boss ==1) {
      b="Blocked-and-damaged";
    }


if (Firebase.ready() && (millis() - railmillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis == 1;
    railmillis = millis();
    Firebase.setString(fbdo, "/locations/Kpakungu", a); Firebase.setString(fbdo, "/locations/Bosso", b);
    delay(200);
    // Serial.printf("Get int a: %s\n", Firebase.getInt (fbdo, "/sensor/weight") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    // weight=fbdo.to<int>();
    //Serial.printf("Get int b: %s\n", Firebase.getString (fbdo, "locations/kpakungu") ? String(fbdo.to<const char *>()).c_str() : fbdo.errorReason().c_str());
    String Kpakungu = fbdo.stringData();
    //Serial.printf("Get int b: %s\n", Firebase.getString (fbdo, "locations/Bosso") ? String(fbdo.to<const char *>()).c_str() : fbdo.errorReason().c_str());
    String Bosso = fbdo.stringData();
    // Serial.println();
    // Serial.print("Kpakungu state: "); Serial.print(Kpakungu); Serial.print("  Bosso state: "); Serial.print(Bosso);
 
  }


}

void ultrasonic(){
    digitalWrite(trig1, 0); delayMicroseconds(2); digitalWrite(trig1, 1); delayMicroseconds(10); digitalWrite(trig1, 0);  
    digitalWrite(trig1, 0); delayMicroseconds(2);duration1=pulseIn(echo1, 1);  
    distance1=duration1*0.034/2;
    digitalWrite(trig2, 1); delayMicroseconds(10); digitalWrite(trig2, 0); duration2=pulseIn(echo2, 1);  
    distance2=duration2*0.034/2;
    Serial.print("distance1: "); Serial.print(distance1); Serial.println(" cm     "); 
    Serial.print("distance2: "); Serial.print(distance2); Serial.println(" cm       ");
  }
