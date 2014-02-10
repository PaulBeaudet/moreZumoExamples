
#include <Pushbutton.h> //Button in the back of the Zumo

Pushbutton button(ZUMO_BUTTON); // define ZUMO_BUTTON as back button. (pin 12)
//honestly the you should just set the pin manually
//library uses 10ms delays to debounce with the .isPressed method
// and uses switch cases for actual debouncing (.getSingleDebouncedPress)

void intiateCalibration()
{  
  button.waitForButton(); // wait for a first press before getting too excited
  if (preSession(RESETKEY))//previous session returns true or false
  {
    printResults();// Prints the results of the last session if it existed
    rememberCalibration();// write the old calibration from EEPROM 
  }
  else
  {// calibrate the compass if previous calibration is forgoton
    calibrateCompass();
  };
  button.waitForButton();//wait before proceeding to loop
}


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


void musicInterupt()
{
  static boolean toggel = 0;//1=true, 0=false
  
  if ( button.isPressed() ) // waits for button press event
  {
    toggel = !toggel;// toggels the toggle! !toggle = not toggle
    motorInterupt();
    return;
  }// so if toggle true set false if false set true
  
  if (toggel)//default if just determines true or false
  {//as long as the toggle is on (true)
    marioInProgress();//play next note in the melody if last is done
  }
  
}

void motorInterupt()
{
  static boolean toggel = 0;//1=true, 0=false
  
  if (toggel)//default if just determines true or false
  {//as long as the toggle is on (true)
    reflections();
  }
  else
  {
    goFor(9000,0,0);//stop the motors
  }
  if ( button.isPressed() ) // waits for button press event
  {
    toggel = !toggel;// toggels the toggle! !toggle = not toggle
  }// so if toggle true set false if false set true
}
