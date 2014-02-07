//PaulSumoTesting - Simplified Sumo sketch for the Zumo Arduino
//Import needed libraries, unnessisarry ones are commented out
#include <ZumoMotors.h> //Drive control
#include <Pushbutton.h> //Back button
#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values
Pushbutton button(ZUMO_BUTTON); // define ZUMO_BUTTON as back button. (pin 12)

ZumoMotors motors; // 0 is stopped, 400 is full speed
#define REVERSE_SPEED     200 // speed when reversing to correct itself
#define TURN_SPEED        200 // speed of correction
#define FORWARD_SPEED     400 // speed of foward charge!
#define REVERSE_DURATION  400 // ms --time taken baking away from the border
#define TURN_DURATION     600 // ms --time taken reorienting in a new direction



void setup()//part of code that executes once at the begining of the program
{
  button.waitForButton(); // wait for a press to get started
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{
  buttonEvent();//monitors the button to stop the zumo if pressed

  marioInProgress(); //plays a note to the mario theme song
  
  oldReactions(); // sets the motors according to sensor events

}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$-End of Main loop 


void buttonEvent()
{
  if (button.isPressed())// case in which the zumo button is pressed
  {
    motors.setSpeeds(0, 0);// Stops the zumo motors
    button.waitForRelease();// be sure that release event has happend
    button.waitForButton(); // stop and wait for another press 
    //----!!--- this stops the loop till a button event---!!----
  }//get ready to continue the loop!
}

