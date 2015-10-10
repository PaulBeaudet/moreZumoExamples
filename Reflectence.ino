// Reflectance.ino ~ Copyright 2015 ~ Paul Beaudet MIT licence - see LICENCE
#define REVERSE_DURATION  200 // ms --time taken reversing away from the border
#define TURN_DURATION     600 // ms --time taken reorienting in a new direction
#include <QTRSensors.h> //front sensor array
#include <ZumoReflectanceSensorArray.h> //front sensor array

// ----------Reflectance Sensor Settings
#define NUM_SENSORS 6 //number of sensors to interate across
unsigned int sensor_values[NUM_SENSORS]; //Array of reflectence sensors (pins A0-A5?)
// this might need to be tuned for different lighting conditions, surfaces, etc.
//########################################### reflectance condition value
#define LIGHT_SENSITIVITY  300 // in microseconds //no indication of scale? 
//100-300 dark? 1000-3000 light? test it
//#########################################
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);//set sensor array as "sensors" 

// #################### Core functions ########################
// returns status of sensors
byte reflectEvent() {
  sensors.read(sensor_values); // reads array of sensor values
  //check all of the sensors
  for (byte i = 0; i < NUM_SENSORS; i++){
    // Note that this will always "track" in one direction with preferance to one side
    //    > to avoid dark < to avoid light
    if (sensor_values[i] > LIGHT_SENSITIVITY){
      return i + 1; // add one to shift up values: zero is empty state
    }
    else { return 0; };
    //create a boolean false for simple overlook of empty state
  }
}

// ########## to be depricated ################################ "removed in future" 

// sets motors to avoid dark or light surfaces
void reflections() {
  static byte reactCode = NUM_SENSORS;
  // to keep track event timing 0-5 represent reflectence sensors in typical case
  
  //all is normal no task to complete
  if (reactCode == NUM_SENSORS) {
    sensors.read(sensor_values); // reads array of sensor values
    //in this case only the leftmost and rightmost sensor values are used
    for (byte i = 0; i < NUM_SENSORS; i++){
      // > to avoid dark < to avoid light
      if (sensor_values[i] > LIGHT_SENSITIVITY){
        reactCode = i; // react based on whatever reflectence sensor is triped
        //headingRecord();//records heading samples in eeprom
      } else {
        goFor(9001,400,125);//go forward given no bariers  
      }// time running out in a lock may be an issue // maybe set time as i?
    }
  } else if (reactCode < NUM_SENSORS) {
    // opperational note!! must set the speeds different!!
    if (goFor(REVERSE_DURATION, -400, -400)){
      //headingRecord();//records heading samples in eeprom //should be the same heading
      //half, would be an issue with odd sensor amounts
      if (reactCode < NUM_SENSORS/2 ){
        reactCode= NUM_SENSORS+1; //creat turn right event
      } else {
        reactCode= NUM_SENSORS+2; //creat turn left event
      };
    }
  } else if (reactCode == NUM_SENSORS + 1) {
    // if leftmost sensor detects line, reverse and turn to the right
    if(goFor(TURN_DURATION, 200, -200)) {
      //headingRecord();//records heading samples in eeprom
      reactCode = NUM_SENSORS;
    }
  }
  else if (reactCode == NUM_SENSORS + 2) {
  // if rightmost sensor detects line, reverse and turn to the left
    if(goFor(TURN_DURATION, -200, 200 )) {
      //headingRecord();//records heading samples in eeprom
      reactCode = NUM_SENSORS;
    }
  }
}

