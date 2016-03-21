// motors.ino ~ Copyright 2015-2016 ~ Paul Beaudet MIT licence - see LICENCE
#include <ZumoMotors.h> // Import polulu moto controlor library
// pins: 10, 9 PWM and 8, 7 for direction
// motors: 0 is stopped, 400 is full speed negitives are reverse speed
ZumoMotors motors; // establish motors object

// ### Basic motion functions ###

void speedsSet(int left, int right){  // calibrated motor heading
  int offset = OFFSET;                // lazy way TODO: pull from EEPROM
  if (offset > 400) {                 // left offset case: decern weak side on range
    offset = offset - 1000;           // 1000 is extra amount added to left offsets
    left = nutralize(left, offset);   // get offset reletive to direction
  } else {                            // right tread ofset case
    right = nutralize(right, offset); // untouched is the right offset case
  };
  motors.setSpeeds(left, right);      // set motors on their way
}

// writes offset correctly no matter the direction
int nutralize(int bearing, int offset){
  if(bearing == 0){           // no bearing = no offset
    return 0;                 // motor is stoped! leave it be
  } else if ( bearing < 0 ) { // if the bearing is negitive (reverse)
    return bearing + offset;  // if reverse add offset
  } else {                    // example -400 + 15 = -385
    return bearing - offset;  // if going forward subtract offset
  };                          // example 400 - 15 = 385
} // in basic; same speeds are returned in relitive directions

void stopMotors(){motors.setSpeeds(0,0);} // abstracted to remove 0 case from goFor

boolean goFor( long durration, int left, int right){ // true on busy; false when done
  static unsigned long startTime = millis();
  static int lastLeft = 0; static int lastRight = 0;

  if (left != lastLeft || right != lastRight){ // check for new values
    startTime = millis();                      // time started
    lastLeft=left; lastRight=right;            // set previous speeds
    speedsSet(left, right);                    // set speeds
  } // else if alloted time has passed return true
  else if(millis() - startTime > durration){return true;}
  return false; // false denotes 'busy' signal
}

//### high level opperations ###

boolean motorReact (byte eventType) { // crude reactions to events
  static boolean doneBackinUp = false;
  
  if (doneBackinUp) {
    if (eventType < 4){          // right backup case
      if(goFor(600, 200, -200)){    
        doneBackinUp = false; 
        return true;  
      }
    } else if (eventType < 7) {  // left backup case
      if(goFor(600, -200, 200)){   
        doneBackinUp = false;
        return true;
      }     
    }
  } else if (goFor(300,-400,-400)) { //go backwards
    doneBackinUp = true;
  };
  return false;
}

