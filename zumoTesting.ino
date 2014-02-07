//PaulSumoTesting - Simplified Sumo sketch for the Zumo Arduino
//Import needed libraries, unnessisarry ones are commented out
#include <Pushbutton.h> //Back button
#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% definitions of variables and autoplaced values
Pushbutton button(ZUMO_BUTTON); // define ZUMO_BUTTON as back button. (pin 12)

#define LED 13 // for debuging durring run time (pin 13)


void setup()//Part of every Sketch: Executes once in the begining
{
  button.waitForButton(); // wait for a press to get started
}

void loop()// Part of every Sketch: Continuously runs over and over until out of power
{
  buttonEvent();//monitors the button to stop the zumo if pressed

  marioInProgress(); //plays a note to the mario theme song
  
  reflections(); // sets the motors according to reflectence sensor events

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
