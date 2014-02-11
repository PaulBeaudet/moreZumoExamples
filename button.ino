
#define ZUMOBUTTON 12//define the pin that the zumo button is on

void buttonUp()// it's cold out there
{ 
  pinMode(ZUMOBUTTON, INPUT);//set the zumo button as an input
  digitalWrite(ZUMOBUTTON, HIGH);//this sets pull-up resistor/ie input through 20k to 5v
}

#define BOUNCETIME 10//ms anytime grater is to know the buton has settled
// wait this long to be sure of a legit press
#define HOLDSTATE 400//ms

byte pressEvent()// all the jouralist come
{//checks for a debounced button press event
  static unsigned long time = millis();
  static boolean timingState = false;

  if ( digitalRead(ZUMOBUTTON) == LOW ) // low is an event with the pullup
  {// if the button has been pressed
    if(timingState)
    { // given the timer has started
      if ( millis() - time > BOUNCETIME)
      { // check if the bounce time has elapesed 
        if ( millis() - time > HOLDSTATE)
        {//in case the button is held longer return state 2
          return 2;// held press
        }//keep in mind this function can still be read as a boolean 
        // as anything greater than 0 is = true
        return 1;//debounced press
      }  
      return 0;//return without changing timing state 
    }
    timingState = true; // note that the timing state is set
    time = millis();//placemark when time press event started
    return 0; // return with the timestate placeholder set
  }
  //outside of eventcases given no reading
  timingState = false; //in case the timing state was set, unset
  return 0;//not pressed
}



void holdForButton()
{
  while(!pressEvent())
  {
    ;
  }
}

void intiateCalibration()
{  
  holdForButton(); // wait for a first press before getting too excited
  if (preSession(RESETKEY))//previous session returns true or false
  {
    printResults();// Prints the results of the last session if it existed
    rememberCalibration();// write the old calibration from EEPROM 
  }
  else
  {// calibrate the compass if previous calibration is forgoton
    calibrateCompass();
  };
}


byte buttonInterupt()// returns actionable situations 
{//monitors button (0-start music->1-start motors->2-stop music->3-stop motors->0)
  static boolean toggelMusic = 0;//1=true, 0=false
  static boolean toggelMotor = 0;// for a total of 4 cases

  if ( pressEvent() ) // monitors for button press event
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
















