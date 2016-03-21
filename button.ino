// button.ino ~ Copyright 2015 ~ Paul Beaudet ~ MIT licence, See LICENCE 
#define ZUMOBUTTON 12//define the pin that the zumo button is on

#define BOUNCETIME 10 //ms: wait this long to be sure of a legit press
#define HOLDSTATE 200 //ms: time to wait for a hold

void buttonUp(){ pinMode(ZUMOBUTTON, INPUT_PULLUP); } // setup button

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

byte buttonEvents(byte totalEvents){ // rotates through a series of specified events 
  static byte event = 0;                            // defualt return event
  static byte pressed = false;                      // defualt press state
  // make sure release event occurs before detecteding another press
  if(pressed && pressEvent(HIGH)){pressed = false;} // release event
  if(!pressed && pressEvent()){                     // press event
    pressed = true;                                 // record pressed
    event++;                                        // increment event
    if(event == totalEvents){event=0;}              // limit to total events 
  }
  return event;                                     // return current event
}

