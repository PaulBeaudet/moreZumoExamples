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
  compassSetup();// intiates the compass
  intiateCalibration();// waits for a user press to calibrate or give debug read out
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{ 
  musicInterupt();//monitors button (start music-> start motors-> stop music-> stop motors

  //marioInProgress(); //plays notes of Mario theme song as appropriate
  
  digitalWrite(LED, LOW); //turn LED off to indicate writing state 
  // currently heading writes
  
  reflections(); // sets the motors according to reflectence sensor events
  //motorInterupt();
  
  //headingRecord();//records heading samples in eeprom 
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$-End of Main loop 



