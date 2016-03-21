// LSM303.ino ~ Copyright 2015 ~ Paul Beaudet MIT licence - see LICENCE
// This is the zumo's on board compass accelormeter
#include <Wire.h>// I2C library comes with IDE
#include <Adafruit_LSM303.h> //Adafruit's library for the same chip
// https://github.com/adafruit/Adafruit_LSM303

Adafruit_LSM303 lsm303;

//detects if the zumo has been picked up
byte checkForPickup() {
  lsm303.read(); //read the z axis of the accelerometer
  if ( lsm303.accelData.z > 1500){
    // detect if its enough acceloration on z event for a pick-up event
    return 1 ;//return event number if picked up 0 if not
  }
  return 0;
}

void lsm303Up(){
  Wire.begin();
  if (!lsm303.begin()) {
    bugLight(); //flash the LED if the chip Fails to load
  }
}
