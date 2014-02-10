
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


void musicInterupt()
{
  static boolean toggel = 0;//1=true, 0=false

  if ( button.isPressed() ) // waits for button press event
  {
    toggel = !toggel;// toggels the toggle! !toggle = not toggle
    return;
  }// so if toggle true set false if false set true

  if (toggel)//default if just determines true or false
  {//as long as the toggle is on (true)
    marioInProgress();//play next note in the melody if last is done
  }

}

byte buttonInterupt()// returns actionable situations 
{//monitors button (0-start music->1-start motors->2-stop music->3-stop motors->0)
  static boolean toggelMusic = 0;//1=true, 0=false
  static boolean toggelMotor = 0;// for a total of 4 cases

  if ( button.isPressed() ) // monitors for button press event
  {
    if (toggelMusic)
    {
      if (toggelMotor)
      {//Music and motors are going! 
        toggelMusic = false;//turn the music off
        return 3;//move to situation 3: only motors
      }
      //only music is playing
      toggelMotor = true;//turn on the motors
      return 2;//move to situation 2 where the motors are on
    }
    else// if the music is off
    {
      if (toggelMotor)
      {//The music has been turned off and the motors are still on
        toggelMotor = false;//now both are false again
        return 0;//move to situation 0 to complete the loop
      } 
      //both bools are false 
      toggelMusic = true;//turn the music on!
      return 1;//move to situation 1
    };
  }// in this way music is on in situations 1 && 2
  //and motors are on in 2 && 3, neither are on in 0
  // the following returns the current situation given no button press
  if (toggelMusic)
  {
    if (toggelMotor)
    {
      return 2;
    }
    else
    {
      return 1;
    };
  }
  else// if the music is off
  {
    if (toggelMotor)
    {// and the motors are on
      return 3;
    }
    else
    {
      return 0;
    }
  };
}






