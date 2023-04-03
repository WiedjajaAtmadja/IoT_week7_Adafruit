#include <Arduino.h>
#include <DHTesp.h>
#include <BH1750.h>
#include <Ticker.h>
#include <AdafruitIO_WiFi.h>
#include "wifi_id.h"

#define LED_GREEN  4
#define LED_YELLOW 5
#define LED_RED    18
#define PUSH_BUTTON 23
#define DHT_PIN 19

DHTesp dht;
BH1750 lightMeter;
Ticker tickerSendData;

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASSWORD);
AdafruitIO_Feed *feedLuxMeter = io.feed("luxMeter");
AdafruitIO_Feed *feedHumidity = io.feed("humidity");
AdafruitIO_Feed *feedTemperature = io.feed("temperature");
AdafruitIO_Feed *feedLedRed = io.feed("ledRed");
AdafruitIO_Feed *feedLedYellow = io.feed("ledYellow");
AdafruitIO_Feed *feedLedGreen = io.feed("ledGreen");
void onSendData()
{
    digitalWrite(LED_GREEN, HIGH);
    float fHumidity = dht.getHumidity();
    float fTemperature = dht.getTemperature();
    float lux = lightMeter.readLightLevel();
    feedHumidity->save(fHumidity);
    feedTemperature->save(fTemperature);
    feedLuxMeter->save(lux);
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
  tickerSendData.attach(30, onSendData);
  Serial.print("Connecting to Adafruit IO...");
  io.connect();
  // set message handler to read feed from dashboard
  feedLedRed->onMessage([](AdafruitIO_Data *data)
                        { digitalWrite(LED_RED, data->toPinLevel()); });
  feedLedGreen->onMessage([](AdafruitIO_Data *data)
                          { digitalWrite(LED_GREEN, data->toPinLevel()); });
  feedLedYellow->onMessage([](AdafruitIO_Data *data)
                           { digitalWrite(LED_YELLOW, data->toPinLevel()); });
  // wait for a connection
  while (io.status() < AIO_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  Serial.println("System ready");
}

void loop() {
  io.run();
  // digitalWrite(LED_RED, HIGH);
  // digitalWrite(LED_YELLOW, HIGH);
  // delay(100);
  // digitalWrite(LED_RED, LOW);
  // digitalWrite(LED_YELLOW, LOW);
  // delay(900);
}