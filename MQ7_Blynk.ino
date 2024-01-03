/*MQ-7 Gas Detector
 */
 //Fill ID, NAME and AUTH based on Blynk Device
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <LiquidCrystal_I2C.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = BLYNK_AUTH_TOKEN;// Enter your Auth token
char ssid[] = "";//Enter your WIFI SSIS
char pass[] = "";//Enter your WIFI password
BlynkTimer timer;
int pinValue = 0;

#define Buzzer D5
#define Green D6
#define Red D7
#define Fan D4
#define Sensor A0

void setup() {
  Serial.begin(9600);
  lcd.backlight();
  lcd.begin();
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, notifiaction);
}
BLYNK_WRITE(V0) {
  pinValue = param.asInt();
}

void notifiaction() {
  int sensor = analogRead(Sensor);
  Serial.println(sensor);
  sensor = map(sensor, 0, 1024, 0, 100);
  if (pinValue == 1) {
    if (sensor <= 40) {
      digitalWrite(Green, HIGH);
      digitalWrite(Red, LOW);
      digitalWrite(Buzzer, LOW);
      digitalWrite(Fan, HIGH);
      lcd.setCursor(0, 1);
      lcd.print("Gas value:Normal");
    } else if (sensor > 40) {
      Blynk.logEvent("high_co");
      digitalWrite(Green, LOW);
      digitalWrite(Red, HIGH);
      digitalWrite(Buzzer, HIGH);
      digitalWrite(Fan, LOW);
      lcd.setCursor(0, 1);
      lcd.print("Gas value:High  ");
    }
    lcd.setCursor(0, 0);
    lcd.print("Value : ");
    lcd.print(sensor);
    Blynk.virtualWrite(V1, sensor);
  } else {
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    digitalWrite(Green, LOW);
    digitalWrite(Fan, HIGH);
    lcd.clear();
  }
}

void loop() {
  Blynk.run();
  timer.run();  
}