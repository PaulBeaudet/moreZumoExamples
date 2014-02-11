//ZumoTesting - Detailed and explained Sumo sketch for the Zumo Arduino Shield

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values

#define LED 13 // for debuging durring run time (pin 13)
#define DEBUGING false // set to true in order to serial debug last readings

//######### change this number to recalibrate
#define RESETKEY 0 // <-----needs to be a byte value
//##################### // I plus one to this every rewrite test

byte event = 0;//event to signal motor actions 

void setup()//Part of every Sketch: Executes once in the begining
{
  buttonUp();//set up the button
  if (DEBUGING)//if we are in the mood to mess around with the compass and try to check results from it
  {
    compassSetup();// intiates the compassholdForButton(); // wait for a first press before getting too excited
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
  }
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
      goFor(9000, 400, 200);//drive forward
      event = reflectEvent();// look for obstacels
    };
  }
  else
  {//stop motors 
    goFor(9000,0,0); //durration mearly returns true once time has lapsed
  };// without external flow control motors will perpetually be actuated to the same speed 
}






