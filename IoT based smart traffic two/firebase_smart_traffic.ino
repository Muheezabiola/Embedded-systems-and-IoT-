#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define WIFI_SSID "smarttraffic"
#define WIFI_PASSWORD "smarttraffic"
#define API_KEY "AIzaSyAQaHaxxxxxxxxxxxah2gbyBgsMhgHI"
#define DATABASE_URL "https://smaxxxxx-6c023-dexxxxt-rtdb.firebaseio.com"
#define USER_EMAIL "muheezabiola99@gmail.com"
#define USER_PASSWORD "xxxxxx"

#define LCD1_ADDR 0x27
#define LCD2_ADDR 0x26
#define LCD3_ADDR 0x25

#define IR1_1_PIN 35
#define IR1_2_PIN 32
#define IR1_3_PIN 33

#define IR2_1_PIN 2
#define IR2_2_PIN 15
#define IR2_3_PIN 13

#define IR3_1_PIN 36
#define IR3_2_PIN 39
#define IR3_3_PIN 34

#define RED1_PIN 16
#define YEL01_PIN 17
#define GREEN1_PIN 4

#define RED2_PIN 27
#define YEL02_PIN 14
#define GREEN2_PIN 12

#define RED3_PIN 18
#define YEL03_PIN 19
#define GREEN3_PIN 23

LiquidCrystal_I2C lcd1(LCD1_ADDR, 16, 2);
LiquidCrystal_I2C lcd2(LCD2_ADDR, 16, 2);
LiquidCrystal_I2C lcd3(LCD3_ADDR, 16, 2);

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long greenLightDuration = 0;
unsigned long yellowLightDuration = 1000;

int currentRoad = 1;
bool yellowLightOn = false;
unsigned long lane1Congestion, lane2Congestion, lane3Congestion;
unsigned long countDown1, countDown2, countUp3;
String Lane1, Lane2, Lane3;

// Function to get congestion level
int getCongestionLevel(int irPin1, int irPin2, int irPin3) {
  int level = 0;
  level += !digitalRead(irPin1); // 0 means obstacle detected
  level += !digitalRead(irPin2);
  level += !digitalRead(irPin3);

  if (level == 3) return 30; // High congestion
  if (level == 2) return 20; // Medium congestion
  return 10;                 // Low congestion
}

// Function to get congestion level string
String getCongestionLevelString(int level) {
  if (level == 30) return "HIGH";
  if (level == 20) return "MEDIUM";
  return "LOW";
}
int a = 0;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

void setup() {
  Wire.begin();

  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();
  lcd3.init();
  lcd3.backlight();

  Serial.begin(115200);
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("CONNECTING");
  lcd1.setCursor(0, 1);
  lcd1.print ("Please wait...");
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("CONNECTING");
  lcd2.setCursor(0, 1);
  lcd2.print ("Please wait...");
  lcd3.clear();
  lcd3.setCursor(0, 0);
  lcd3.print("CONNECTING");
  lcd3.setCursor(0, 1);
  lcd3.print ("Please wait...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Firebase.begin(&config, &auth);
  Firebase.setDoubleDigits(5);

  lcd1.clear();
  lcd2.setCursor(0, 0);
  lcd1.print("CONNECTED!");
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("CONNECTED!");
  lcd3.clear();
  lcd3.setCursor(0, 0);
  lcd3.print("CONNECTED!");
  delay(500);
  lcd1.clear();
  lcd2.clear();
  lcd3.clear();

  lcd1.setCursor(0, 0);
  lcd1.print("LANE 1: ");

  lcd2.setCursor(0, 0);
  lcd2.print("LANE 2: ");

  lcd3.setCursor(0, 0);
  lcd3.print("LANE 3: ");

  pinMode(IR1_1_PIN, INPUT);
  pinMode(IR1_2_PIN, INPUT);
  pinMode(IR1_3_PIN, INPUT);
  pinMode(IR2_1_PIN, INPUT);
  pinMode(IR2_2_PIN, INPUT);
  pinMode(IR2_3_PIN, INPUT);
  pinMode(IR3_1_PIN, INPUT);
  pinMode(IR3_2_PIN, INPUT);
  pinMode(IR3_3_PIN, INPUT);

  pinMode(RED1_PIN, OUTPUT);
  pinMode(YEL01_PIN, OUTPUT);
  pinMode(GREEN1_PIN, OUTPUT);
  pinMode(RED2_PIN, OUTPUT);
  pinMode(YEL02_PIN, OUTPUT);
  pinMode(GREEN2_PIN, OUTPUT);
  pinMode(RED3_PIN, OUTPUT);
  pinMode(YEL03_PIN, OUTPUT);
  pinMode(GREEN3_PIN, OUTPUT);

  digitalWrite(RED1_PIN, LOW);
  digitalWrite(YEL01_PIN, LOW);
  digitalWrite(GREEN1_PIN, HIGH);

  digitalWrite(RED2_PIN, HIGH);
  digitalWrite(YEL02_PIN, LOW);
  digitalWrite(GREEN2_PIN, LOW);

  digitalWrite(RED3_PIN, HIGH);
  digitalWrite(YEL03_PIN, LOW);
  digitalWrite(GREEN3_PIN, LOW);

  lane1Congestion = getCongestionLevel(IR1_1_PIN, IR1_2_PIN, IR1_3_PIN);
  lane2Congestion = getCongestionLevel(IR2_1_PIN, IR2_2_PIN, IR2_3_PIN);
  lane3Congestion = getCongestionLevel(IR3_1_PIN, IR3_2_PIN, IR3_3_PIN);

  Lane1 = getCongestionLevelString(lane1Congestion);
  Lane2 = getCongestionLevelString(lane2Congestion);
  Lane3 = getCongestionLevelString(lane3Congestion);

  greenLightDuration = lane1Congestion * 1000;

  countDown1 = greenLightDuration / 1000;
  countDown2 = countDown1;
  countUp3 = 0;
  previousMillis = millis();
}

void loop() {
  currentMillis = millis();

  if (countDown1 < 10 && a == 1) {
    a = 0;
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
  }
  if (countDown1 > 10) {
    a = 1;
  }
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    Firebase.setString(fbdo, "/traffic/Lane1", Lane1);
    Firebase.setString(fbdo, "/traffic/Lane2", Lane2);
    Firebase.setString(fbdo, "/traffic/Lane3", Lane3);
    delay(200);
  }

  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;

    if (yellowLightOn) {
      yellowLightOn = false;
      digitalWrite(YEL01_PIN, LOW);
      digitalWrite(YEL02_PIN, LOW);
      digitalWrite(YEL03_PIN, LOW);
    } else {
      countDown1--;
      countDown2--;
      countUp3++;

      // Update displays
      lcd1.setCursor(0, 0);
      lcd1.print("LANE 1: ");
      lcd1.print(Lane1);

      lcd2.setCursor(0, 0);
      lcd2.print("LANE 2: ");
      lcd2.print(Lane2);

      lcd3.setCursor(0, 0);
      lcd3.print("LANE 3: ");
      lcd3.print(Lane3);

      lcd1.setCursor(0, 1);
      lcd1.print("Time: ");
      lcd1.print(countDown1);

      lcd2.setCursor(0, 1);
      lcd2.print("Time: ");
      lcd2.print(countDown1);  // All lanes should have the same countdown

      lcd3.setCursor(0, 1);
      lcd3.print("Time: ");
      lcd3.print(countDown1);  // All lanes should have the same countdown

      // Check if green time is over
      if (countDown1 == 0) {
        yellowLightOn = true;
        digitalWrite(GREEN1_PIN, LOW);
        digitalWrite(GREEN2_PIN, LOW);
        digitalWrite(GREEN3_PIN, LOW);

        digitalWrite(YEL01_PIN, HIGH);
        digitalWrite(YEL02_PIN, HIGH);
        digitalWrite(YEL03_PIN, HIGH);

        delay(yellowLightDuration);

        currentRoad++;
        if (currentRoad > 3) currentRoad = 1;

        switch (currentRoad) {
          case 1:
            digitalWrite(RED1_PIN, LOW);
            digitalWrite(GREEN1_PIN, HIGH);

            digitalWrite(RED2_PIN, HIGH);
            digitalWrite(RED3_PIN, HIGH);

            lane1Congestion = getCongestionLevel(IR1_1_PIN, IR1_2_PIN, IR1_3_PIN);
            Lane1 = getCongestionLevelString(lane1Congestion);
            greenLightDuration = lane1Congestion * 1000;
            countDown1 = greenLightDuration / 1000;
            countDown2 = countDown1;
            countUp3 = 0;
            break;
          case 2:
            digitalWrite(RED2_PIN, LOW);
            digitalWrite(GREEN2_PIN, HIGH);

            digitalWrite(RED1_PIN, HIGH);
            digitalWrite(RED3_PIN, HIGH);

            lane2Congestion = getCongestionLevel(IR2_1_PIN, IR2_2_PIN, IR2_3_PIN);
            Lane2 = getCongestionLevelString(lane2Congestion);
            greenLightDuration = lane2Congestion * 1000;
            countDown1 = greenLightDuration / 1000;
            countDown2 = countDown1;
            countUp3 = 0;
            break;
          case 3:
            digitalWrite(RED3_PIN, LOW);
            digitalWrite(GREEN3_PIN, HIGH);

            digitalWrite(RED1_PIN, HIGH);
            digitalWrite(RED2_PIN, HIGH);

            lane3Congestion = getCongestionLevel(IR3_1_PIN, IR3_2_PIN, IR3_3_PIN);
            Lane3 = getCongestionLevelString(lane3Congestion);
            greenLightDuration = lane3Congestion * 1000;
            countDown1 = greenLightDuration / 1000;
            countDown2 = countDown1;
            countUp3 = 0;
            break;
        }
      }
    }
  }
}
