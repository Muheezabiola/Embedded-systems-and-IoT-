#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  
int mq=32;
#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
//#include <Firebase_ESP_CLient.h>
#define WIFI_SSID "TimiNet"
#define WIFI_PASSWORD "987654321"
#define API_KEY "AIzaSyDJYEwbqh_QU_fuDtMUupuYnJGUfgXmC48"
#define DATABASE_URL "https://new-gas-system-default-rtdb.firebaseio.com" 
int a, b, c, weight, gas, flame;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;
#include "HX711.h"
const int LOADCELL_DOUT_PIN = 14;
const int LOADCELL_SCK_PIN = 12;
HX711 scale;
int loadsensormillis=0;

void setup() {
  lcd.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Design and");
lcd.setCursor(0,1);
lcd.print("Construction");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Of a remote");
lcd.setCursor(0,1);
lcd.print("Gas level");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("monitoring and");
lcd.setCursor(0,1);
lcd.print("Leakage");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Detection");
lcd.setCursor(0,1);
lcd.print("System");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Department Of");
lcd.setCursor(0,1);
lcd.print("Elect/Electrns");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Engineering");
lcd.setCursor(0,1);
lcd.print("----------");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("By Ebuetse");
lcd.setCursor(0,1);
lcd.print("Abraham Oshogie");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Matric No.");
lcd.setCursor(0,1);
lcd.print("2017/1/69091EE");
delay(2000);
lcd.clear();
lcd.print("Federal");
lcd.setCursor(0,1);
lcd.print("University Of");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Technology");
lcd.setCursor(0,1);
lcd.print("Minna");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("System Booting");
lcd.setCursor(0,1);
lcd.print("Please Wait...........");
// delay(1000);
// lcd.clear();
  pinMode(mq, INPUT);
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(22500.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();
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
  
lcd.clear();
lcd.print("System connected");
lcd.setCursor(0,1);
lcd.print("to the internet");
}

void loop() {
  String gas_state;
  the_weight ();
  b=analogRead(mq); c=analogRead(26);
if (b<=500) {
  gas_state = "No";
}
else {
  gas_state="Yes";
}
  if (Firebase.ready() && (millis() - loadsensormillis > 1000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis == 1;
    loadsensormillis = millis();
    Firebase.setInt(fbdo, "/sensor/weight", a); Firebase.setString(fbdo, "/sensor/gas", gas_state); Firebase.setInt(fbdo, "/sensor/flame", c);
    delay(200);
    Serial.printf("Get int a: %s\n", Firebase.getInt (fbdo, "/sensor/weight") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    weight=fbdo.to<int>();
    Serial.printf("Get int b: %s\n", Firebase.getString (fbdo, "/sensor/gas") ? String(fbdo.to<const char *>()).c_str() : fbdo.errorReason().c_str());
    String gas = fbdo.stringData();
    Serial.printf("Get int c: %s\n", Firebase.getInt (fbdo, "/sensor/flame") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
    flame=fbdo.to<int>();
    Serial.println();
    Serial.print("weight: "); Serial.print(weight); Serial.print("  gas: "); Serial.print(gas); Serial.print("  flame: "); Serial.println(flame);
  
  lcd.clear();
lcd.setCursor(0,0);
lcd.print("Level: "); lcd.print(weight); lcd.print(" kg");
lcd.setCursor(0,1);
lcd.print("Leakage: "); lcd.print(gas);
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