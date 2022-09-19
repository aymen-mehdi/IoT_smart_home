// Written by MEHDI Aymen & SAHEL Nazim
// It is very important to download all the libraries used in this code so it compile's properlly
// adafruit/DHT sensor library@^1.4.3
//	khoih-prog/Blynk_WiFiManager@^1.0.16

#include <Arduino.h>

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPLWbSPRjT7"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "P9BPpiFhYGAJ2BuWEhIHqTOMaHwjx02r"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wifi-name";
char pass[] = "wifi-password";

BlynkTimer timer;

// This function is called every time the Virtual Pin V0 state changes
BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

// =========================================================================

// DHT11 ******************

#include <Adafruit_Sensor.h> // Library : Adafruit Unified Sensor
#include <DHT.h>
#include <DHT_U.h> // Library : DHT sensor library

#define DHTPIN 27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// MQ-9 ******************

#define smokeA0 35 // Analog pin

// LED & FAN ******************

int LED = 4;

int FAN = 5;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  // =========================================

  // Debug console

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  // Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  // MQ-9
  pinMode(smokeA0, INPUT);

  // DHT11
  dht.begin();

  // =========================================
  pinMode(LED, OUTPUT);
  pinMode(FAN, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(15000);
  //====================================
  // Mesure the TEMPERATURE
  float h = dht.readHumidity();
  // celcius
  float t = dht.readTemperature();
  // fahrenheit
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\\==========");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\\t");
  Serial.print("==========");

  //====================================
  // Mesure the CO2
  int analogSensor = analogRead(smokeA0);
  Serial.print("CO2: ");
  Serial.println(analogSensor);

  // Moving on to the application
  //====================================
  Blynk.virtualWrite(V5, t);
  Blynk.virtualWrite(V6, h);
  Blynk.virtualWrite(V7, analogSensor);
  //====================================

  // ====================================
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  // ====================================
}

// Switch relay for the LED HIGH/LOW
BLYNK_WRITE(V4)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V4 to a variable
  if (pinValue == 1)
  {
    digitalWrite(LED, HIGH); // Turn LED on.
  }
  else
  {
    digitalWrite(LED, LOW); // Turn LED off.
  }
}
//===================================

// Switch relay for the FAN HIGH/LOW
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt(); // Assigning incoming value from pin V3 to a variable
  if (pinValue == 1)
  {
    digitalWrite(FAN, HIGH); // Turn LED on.
  }
  else
  {
    digitalWrite(FAN, LOW); // Turn LED off.
  }
}