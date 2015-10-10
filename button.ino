// button.ino ~ Copyright 2015 ~ Paul Beaudet ~ MIT licence, See LICENCE 
#define ZUMOBUTTON 12//define the pin that the zumo button is on
// ##############  Core Functions ##################
void buttonUp(){ pinMode(ZUMOBUTTON, INPUT_PULLUP); } // setup button

#define BOUNCETIME 10 // ms anytime grater is to know the buton has settled
                      // wait this long to be sure of a legit press
#define HOLDSTATE 200 // ms

// argument is assumed to be a press event
// careful functions with default value can only be used in this file
// when the true argument is passed high is tested to detect a release
// type assumes HIGH = 1 and LOW = 0 in the form of a boolean argument
// checks for a debounced button press event
byte pressEvent(boolean type = LOW) {     // remove default value to use in main sketch
  static unsigned long time = millis();
  static boolean timingState = false;
                                          // low is a press with the pullup
  if ( digitalRead(ZUMOBUTTON) == type ){ // if the button has been pressed
    if(timingState) {                     // given timer has started
      if ( millis() - time > BOUNCETIME){ // check if bounce time has elapesed 
        if ( millis() - time > HOLDSTATE){// case button held longer return state 2
          return 2;                       // return hold state
        }
        return 1;                         // return debounced press state
      }  
      return 0;                           // still in potential "bounce" window 
    }
    timingState = true; // note that the timing state is set
    time = millis();    // placemark when time press event started
    return 0;           // return with the timestate placeholder set
  }                     // outside of eventcases given no reading
  timingState = false;  // in case the timing state was set, unset
  return 0;             // not pressed
}

void holdForButton(){while(!pressEvent()){;}} // block until pressed

// ####################  less general opperations #######################
// buttonInterupt is to signal four series of presses
// rotating through the four possible return states 0-3

byte buttonWatch()// returns actionable situations 
{//monitors button (0-start music->1-start motors->2-stop music->3-stop motors->0)
  static boolean toggelMusic = 0;//1=true, 0=false
  static boolean toggelMotor = 0;// for a total of 4 cases
  static boolean pressWait = true; //default to wait for a press
  static byte lastEvent = 0; // hold on to the event type
  
  // note checking detection state first
  if (!pressWait && pressEvent(HIGH)){
    //is very important,-> only check release in the case of a press
    pressWait=true;// signal that another press can be accepeted
  }// if !pressWait is basically saying besure pressWait is false
  // only check press in case of a release
  if ( pressWait && pressEvent() ){
    // monitors for hold button press event and if waiting for a press
    pressWait = false;      // signal that now a release is expected
    if (toggelMusic){       // if the music is playing
      if (toggelMotor){     // if the motors are running
        toggelMusic = false;// turn the music off
        lastEvent=3;
        return 3;           // move to situation 3: only motors
      }                     // only music is playing
      toggelMotor = true;   // turn on the motors
      lastEvent = 2;
      return 2; // move to situation 2 where the motors are on
    } else {    // if the music is off
      if (toggelMotor){
        //The music has been turned off and the motors are still on
        toggelMotor = false;//now both are false again
        lastEvent = 0;
        return 0;//move to situation 0 to complete the loop
      } 
                          // both bools are false 
      toggelMusic = true; // turn the music on!
      lastEvent = 1;
      return 1;           // move to situation 1
    };
  }
  return lastEvent; 
}
// In the case of no changes return the last event
// in this way music is on in situations 1 && 2
// and motors are on in 2 && 3, neither are on in 0

