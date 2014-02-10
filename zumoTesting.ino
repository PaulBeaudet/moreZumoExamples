//ZumoTesting - Detailed and explained Sumo sketch for the Zumo Arduino Shield

#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values

#define LED 13 // for debuging durring run time (pin 13)
#define DEBUGING false // set to true in order to serial debug last readings

//######### change this number to recalibrate
#define RESETKEY 0 // <-----needs to be a byte value
//##################### // I plus one to this every rewrite test

void setup()//Part of every Sketch: Executes once in the begining
{
  //compassSetup();// intiates the compass
  //intiateCalibration();// waits for a user press to calibrate or give debug read out
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{ 
  byte situation = buttonInterupt();//monitors button 
  //0-start music->1-start motors->2-stop music->3-stop motors->0
  if (situation == 1 || situation == 2)
  {
    marioInProgress(); //plays notes of Mario theme song as appropriate
  }
  if (situation == 2 || situation == 3)
  {
    reflections(); // sets speeds and directions according to reflectence sensor events
  }
  else
  {//stop motors 
    goFor(9000,0,0); //durration mearly returns true once time has lapsed
  };// without external flow control motors will perpetually be actuated to the same speed 
}




