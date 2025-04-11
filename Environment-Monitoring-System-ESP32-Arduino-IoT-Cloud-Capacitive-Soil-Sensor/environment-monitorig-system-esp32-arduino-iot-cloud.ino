#include "arduino_secrets.h"
/* 
  Sketch generated by the Arduino IoT Cloud Thing "Environment Monitoring System"
  https://create.arduino.cc/cloud/things/0634e927-0b65-4ae8-9b14-e5c9eff30d16 

  Arduino IoT Cloud Variables description

  The following variables are automatically generated and updated when changes are made to the Thing

  float humidity;
  float temperature;
  int soilmoisture;

  Variables which are marked as READ/WRITE in the Cloud Thing will also have functions
  which are called when their values are changed from the Dashboard.
  These functions are generated with the Thing and added at the end of this sketch.
*/

#include "thingProperties.h"
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SoilSensorPin 34

const int dryValue  = 2910;   // Replace with your sensor dry calibration
const int wetValue = 1465;    // Replace with your sensor wet calibration

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastSendTime = 0;
const long interval = 1000; // 1 second


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  
  // Initialize DHT Sensor
  dht.begin();

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  // Your code here 
   // Read and update sensor values every second
  if (millis() - lastSendTime > interval) {
    lastSendTime = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    int sensorValue = analogRead(SoilSensorPin);
    int soilPercent = map(sensorValue, dryValue, wetValue, 0, 100);

    if (!isnan(h) && !isnan(t)) {
      humidity = h;
      temperature = t;
      soilmoisture = soilPercent;

      Serial.print("Soil Moisture: ");
      Serial.print(soilPercent);
      Serial.print(" %   Temperature: ");
      Serial.print(t);
      Serial.print(" °C   Humidity: ");
      Serial.print(h);
      Serial.println(" %");
    } else {
      Serial.println("Failed to read from DHT sensor!");
    }
  }
  
}

/*
  Since Temperature is READ_WRITE variable, onTemperatureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTemperatureChange()  {
  // Add your code here to act upon Temperature change
}

/*
  Since Humidity is READ_WRITE variable, onHumidityChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onHumidityChange()  {
  // Add your code here to act upon Humidity change
}

/*
  Since Soilmoisture is READ_WRITE variable, onSoilmoistureChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onSoilmoistureChange()  {
  // Add your code here to act upon Soilmoisture change
}


