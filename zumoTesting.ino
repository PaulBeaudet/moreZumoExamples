//ZumoTesting - Detailed and explained Sumo sketch for the Zumo Arduino Shield

//Import needed libraries, unnessisarry ones are commented out
#include <Pushbutton.h> //Button in the back of the Zumo
#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values
Pushbutton button(ZUMO_BUTTON); // define ZUMO_BUTTON as back button. (pin 12)

#define LED 13 // for debuging durring run time (pin 13)

//######### change this number to recalibrate
#define RESETKEY 0 // <-----needs to be a byte value
//##################### // I plus one to this every rewrite test

void setup()//Part of every Sketch: Executes once in the begining
{
  compassSetup();// intiates the compass
  button.waitForButton(); // wait for a first press before getting too excited
  if (preSession(RESETKEY))//previous session returns true or false
  {
    printResults();// Prints the results of the last session if it existed
    rememberCalibration();// write the old calibration  
  }
  else
  {// calibrate the compass if previous calibration is forgoton
    calibrateCompass();
  };
  button.waitForButton();//wait before proceeding to loop
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{ 
  buttonEvent();//monitors the button to stop the zumo if pressed

  marioInProgress(); //plays a note to the mario theme song
  
  digitalWrite(LED, LOW); //turn LED off to indicate writing state 
  // currently heading writes
  
  reflections(); // sets the motors according to reflectence sensor events
  
  //headingRecord();//records heading samples in eeprom
  

}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$-End of Main loop 


void buttonEvent()
{
  if (button.isPressed())// case in which the zumo button is pressed
  {
    stopMotors();//stops the motors...
    button.waitForRelease();// be sure that release event has happend
    button.waitForButton(); // stop and wait for another press 
    //----!!--- this stops the loop till a button event---!!----
  }//get ready to continue the loop!
}

