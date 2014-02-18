//ZumoTesting - Detailed and explained Sumo sketch for the Zumo Arduino Shield

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values

#define LED 13 // for debuging durring run time (pin 13)
//#define DEBUGING // uncomment in order to serial debug last readings

//######### change this number to recalibrate
#define RESETKEY 0 // <-----needs to be a byte value
//##################### // I plus one to this every rewrite test


byte event = 0;//event to signal motor actions 

void setup()//Part of every Sketch: Executes once in the begining
{
  buttonUp();//set up the button
  //compassAccelUp();// intiates the lsm303 compass and accele
  lsm303Up();
  
// uncomment defined DEBUGING at top to run  
#ifdef DEBUGING // after runtime debugging, let the zumo run and get data next restart
  // "if debuging is defined" compile up to the end of the if
  // wait for a first press before getting too excited
  if (preSession(RESETKEY))//previous session returns true or false
  {
    printResults();// Prints the results of the last session if it existed
    // open the serial monitor to see the last samples of heading results !!
    rememberCalibration();// write the old calibration from EEPROM 
  }
  else
  {// calibrate the compass if previous calibration is forgoton
    calibrateCompass();
  };
#endif // this the end of a defined "if statement", meaning "if" is decided at compile time opposed to run time 

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
    //reflections(); // sets speeds and directions according to reflectence sensor events

    if ( event )
    {// given an event was detected last loop request reaction 
      if (motorReact(event))//testing reaction: "assures it occures" 
      {//when the reaction is complete
        event = 0;//signal that reaction is done
      }
    }
    else// absent other events
    {
      goFor(9000, 400, 400);//drive forward
      event = reflectEvent();// look for obstacels
    };
  }
  else// == 0
  {//stop motors 
    goFor(9000,0,0); //durration mearly returns true once time has lapsed
  };// without external flow control motors will perpetually be actuated to the same speed 
  if(!event && checkForPickup())
  {//in event case interferance creates false positives
    goFor(9001,0,0);//stop the presses! Zumo has been picked up!
    holdForButton();//wait for a button press to resume
  }
}







