// Reflectance.ino ~ Copyright 2015-2016 ~ Paul Beaudet MIT licence - see LICENCE
#include <QTRSensors.h>                 //front sensor array
#include <ZumoReflectanceSensorArray.h> //front sensor array

// ----------Reflectance Sensor Settings
#define NUM_SENSORS 6 //number of sensors to interate across
// reflectance condition value might need to be tuned for different lighting conditions, surfaces, etc.
#define LIGHT_SENSITIVITY  300 // in microseconds //no indication of scale? 
//100-300 dark? 1000-3000 light? test it

// Create sensors object
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);//set sensor array as "sensors" 

byte reflectEvent() {                     // returns status of sensors
  static unsigned int sensor_values[NUM_SENSORS]; //Array of reflectence sensors (pins A0-A5?)
  sensors.read(sensor_values);            // reads array of sensor values
  for (byte i = 0; i < NUM_SENSORS; i++){ //check all of the sensors
    // Note that this will always "track" in one direction with preferance to one side
    //    > to avoid dark < to avoid light
    if (sensor_values[i] > LIGHT_SENSITIVITY){
      return i + 1; // add one to shift up values: zero is empty state
    }
    else { return 0; };
    //create a boolean false for simple overlook of empty state
  }
}

