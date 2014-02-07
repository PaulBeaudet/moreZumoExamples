//PaulSumoTesting - Simplified Sumo sketch for the Zumo Arduino
//Import needed libraries, unnessisarry ones are commented out
#include <ZumoMotors.h> //Drive control
#include <Pushbutton.h> //Back button
#include <QTRSensors.h> //front sensor array
#include <ZumoReflectanceSensorArray.h> //front sensor array
#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values
Pushbutton button(ZUMO_BUTTON); // define ZUMO_BUTTON as back button. (pin 12)

// ----------Reflectance Sensor Settings
#define NUM_SENSORS 6 //number of sensors to interate across
unsigned int sensor_values[NUM_SENSORS]; //Array of reflectence sensors
// this might need to be tuned for different lighting conditions, surfaces, etc.
//########################################### reflectance condition value
#define LIGHT_SENSITIVITY  300 // in microseconds //no indication of scale?
//#########################################
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);//set sensor array as "sensors"  

ZumoMotors motors; // 0 is stopped, 400 is full speed
#define REVERSE_SPEED     200 // speed when reversing to correct itself
#define TURN_SPEED        200 // speed of correction
#define FORWARD_SPEED     400 // speed of foward charge!
#define REVERSE_DURATION  400 // ms --time taken baking away from the border
#define TURN_DURATION     600 // ms --time taken reorienting in a new direction

unsigned long senseEventTime;
byte reactCode = 0;

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define COMPARED > // this is the comparison type to test for high or low relectence
//change this in order to switch between reacting to black or white borders
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values

void setup()//part of code that executes once at the begining of the program
{
  // %%%% Robot %%%%%
  button.waitForButton(); // wait for a 2nd press to go
  senseEventTime = millis(); //hold a time to count timed events against
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{
  buttonEvent();//monitors the button to stop the zumo if pressed

  //oldway();//line detection and correction with delays
  
  marioInProgress(); //plays a note to the mario theme song

  if (reactCode == 0)//all is normal no task to complete
  {
    sensors.read(sensor_values); // reads array of sensor values
    //in this case only the leftmost and rightmost sensor values are used
    if (sensor_values[0] COMPARED LIGHT_SENSITIVITY)//!!note: opperator is defined!!
    {// if leftmost sensor detects line, reverse and turn to the right
     reactCode=2;//go left reaction code
     backUp(); //backn' up
    }
    else if (sensor_values[5] COMPARED LIGHT_SENSITIVITY)//!!note: opperator is defined!!
    {// if rightmost sensor detects line, reverse and turn to the left
     reactCode=3;//go right reaction code
     backUp(); //backn' up, because my daddy taught me good
    }
    else
    {// reaction code is null without border sense, go straight
      motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    };
  }
  //tasking cases, the monitoring of task allows for multitasking
  //unsigned long timePassed = millis() - senseEventTime;
  if (reactCode > 1 && millis() - senseEventTime > REVERSE_DURATION)
  {//complete backing up case
    if(reactCode == 2)
    {
       motors.setSpeeds(TURN_SPEED, -TURN_SPEED);//left sense turn right
    }
    else if(reactCode == 3)
    {
      motors.setSpeeds(-TURN_SPEED, TURN_SPEED);//right sense turn left
    }
    reactCode = 1;//in either condition reactCode now equals one
    //in anticipation of completing a turn for the desired duration
  }
  //
  if (reactCode == 1 && millis() - senseEventTime > REVERSE_DURATION + TURN_DURATION)
  {// case of completed reaction 
    //motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);//resume forward motion
    //recordCompassHeading();//add heading to eeprom
    reactCode = 0;
  }
}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$-End of Main loop 

      
void backUp()// omg, omg, omg, mg
{
  motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
  senseEventTime=millis(); 
}


void buttonEvent()
{
  if (button.isPressed())// case in which the zumo button is pressed
  {
    motors.setSpeeds(0, 0);// Stops the zumo motors
    button.waitForRelease();// be sure that release event has happend
    button.waitForButton(); // stop and wait for another press 
    //----!!--- this stops the loop till a button event---!!----
    //mario();
  }//get ready to continue the loop!
}

