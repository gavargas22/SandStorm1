#include <pt.h>   // include protothread library
#include <SoftwareSerial.h>
#include "IntersemaBaro.h"
#include "TinyGPS++.h" // Include TinyGPS library
#include <ArduinoJson.h> //Arduino JSON

#define RXPin 4
#define TXPin 3
#define SENSOR_INFO_INTERVAL 20000
#define BAUDRATE 115200
#define GPS_BAUDRATE 9600

Construct the protothreads
static struct pt, gps, heartbeat, sensors;
// Counter for threading
int counter = 0;

// Declare software serial variables.
SoftwareSerial gpsSerial(RXPin, TXPin);
// Altimeter initialization
Intersema::BaroPressure_MS5607B baro(true);
// Set up TinyGPS object
TinyGPSPlus gps;

void setup() {
  // Begin serial with Baudrate
  Serial.begin(BAUDRATE);
  // Begin serial connection to GPS
  gpsSerial.begin(GPS_BAUDRATE);
  // Barometer system initialize
  baro.init();
  // initialise the three protothread variables
  PT_INIT(&gps);
  PT_INIT(&heartbeat);
  PT_INIT(&sensors);
}

PT_THREAD(verifyHeartbeat(struct pt *pt, int interval)) {

  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while(1)
  {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
    timestamp = millis();
    // Send a heartbeat to the base station
  }
  PT_END(pt);
}

static int collectSensorPacket(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;

  PT_BEGIN(pt);

  while(1)
  {

    while(1) {
      PT_WAIT_UNTIL(pt, millis() - timestamp > interval);
       timestamp = millis();

      GPS Data Collection
    }
  }

  PT_END(pt);
}

int handle_byte(int byteGPS)
{
  if(byteGPS == '$')
  {
    Serial.println(nmea_sentence);
    counter = 0;
  }

  if(counter == 300)
    counter = 0;

  nmea_sentence[counter] = byteGPS;
  counter++;
}

// Update for reporting altimeter
PT_THREAD(updateSensorInfo(struct pt *pt, int interval)) {
  static unsigned long timestamp = 0;
  struct pt heartbeat;

  PT_BEGIN(pt);

  long alt_in_cm = baro.getHeightCentimeters();
  float al_in_feet = ((float)(alt_in_cm) / 30.48);

  while(1)
  {

    PT_WAIT_UNTIL(pt, millis() - timestamp > interval);

    Serial.println("Starting");

    timestamp = millis();

    Serial.println("UPDATE SENSOR");

    Serial.print("timestamp: ");
    Serial.println(timestamp);

  }

  PT_END(pt);
}

static Array getAltimeterValue {

}

void loop() {
  transmitGPS(&gps, 5); // by calling them infinitely
  updateSensorInfo(&sensors, 3000);
  verifyHeartbeat(&heartbeat, 1000);
}
