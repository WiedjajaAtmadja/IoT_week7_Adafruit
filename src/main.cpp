#include <Arduino.h>
#include <DHTesp.h>
#include <BH1750.h>
#include <Ticker.h>
#define LED_GREEN  4
#define LED_YELLOW 5
#define LED_RED    18
#define PUSH_BUTTON 23
#define DHT_PIN 19

DHTesp dht;
BH1750 lightMeter;
Ticker tickerSendData;

void onSendData()
{
    digitalWrite(LED_GREEN, HIGH);
    float fHumidity = dht.getHumidity();
    float fTemperature = dht.getTemperature();
    float lux = lightMeter.readLightLevel();
    Serial.printf("Humidity: %.2f, Temperature: %.2f, Light: %.2f \n",
       fHumidity, fTemperature, lux);
    digitalWrite(LED_YELLOW, (fHumidity > 80)?HIGH:LOW);
    digitalWrite(LED_GREEN, LOW);
}

void setup() {
  Serial.begin(460800);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  dht.setup(DHT_PIN, DHTesp::DHT11);
  Wire.begin();
  lightMeter.begin(); 
  tickerSendData.attach(5, onSendData);
  Serial.println("System ready");
}

void loop() {
  // digitalWrite(LED_RED, HIGH);
  // digitalWrite(LED_YELLOW, HIGH);
  // delay(100);
  // digitalWrite(LED_RED, LOW);
  // digitalWrite(LED_YELLOW, LOW);
  // delay(900);
}