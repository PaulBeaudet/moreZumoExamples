// motors.ino ~ Copyright 2015 ~ Paul Beaudet MIT licence - see LICENCE
#include <ZumoMotors.h> //Drive control

//pins 10, 9 PWM and 8, 7 for direction
#define GUESSEDOFFSET 40//guessing the offset of the mottors
//on paul's zumo the right motor is more powerfull
//add 1000 if the left is more powerfull

//motors
ZumoMotors motors; // establish motors object
// 0 is stopped, 400 is full speed negitives are reverse speed

//######## -- User driven Motor Parameters, These can be passed to goMotors("param") 
#define FOREVER 9001 // actually forever is default so this number only maters to return timed true
#define WCURVE 1 // wide curve
#define MCURVE 2 // moderate curve
#define SCURVE 3 // sharp curve
#define SFORWARD 4 //slow forward
#define MFORWARD 5 //moderate forward
#define FFORWARD 6 //fast forward
#define SBACKWARD 7 //slow Backward
#define MBACKWARD 8 //moderate Backward
#define MBACKWARD 9 //fast Backward
#define TURN_DURRATION 300
#define REVERSE_DURRATION 200

//----Propossed functions in process

void motoCorrect(int left, int right)
{// calibrated motor heading
  int offset = GUESSEDOFFSET; //promRead(8); //read int from EEPROM  
  // assuming the calibration has been done these are read from eeprom
  //discern weak side 
  if (offset > 400) {
     //greater than greatest offset, this is left offset case
     offset = offset - 1000; //subtract left boolean
     left = nutralize(left, offset);
  } else {
    right = nutralize(right, offset); // untouched is the right offset case
  };
  motors.setSpeeds(left, right);//onward!
}

int nutralize(int bearing, int offset)
{// this function writes the offset correctly no matter the direction
  if ( bearing == 0 ) {
    return 0; //the motor is stoped! leave it be
  } else if ( bearing < 0 ) { // if the bearing is negitive (reverse)
    return bearing + offset;  // if reverse add offset
  } else { //example -400 + 15 = -385
    return bearing - offset; // if going forward subtract offset
  }; //example 400 - 15 = 385
}    //in basic; same speeds are returned in relitive directions

boolean timer(unsigned long time) {
  static unsigned long startTime = millis();
  static unsigned long setTime;
  //check the clock
  if (setTime == time){
    //in this way a different durration is need to reset the clock
    if ( millis() - startTime > setTime) { // when in progress
      setTime=0;//ready the function for next timed action
      //this makes it posible to repeat the same durration 
      return true;// the time is up!
    }//given durration is still in progress fall thru to false case
  } else {
    setTime = time; //set the clock
    startTime = millis();//track what the start time was
  }
  return false;//fall thru case
}

// ###### Core functions ################

void stopMotors() { //abstracted to remove 0 case from goFor
  motors.setSpeeds(0,0);
}

//returns true while still in progress false when times up siginfying when
// next task can take place
boolean goFor( long time, int left, int right){
  static unsigned long startTime = millis();
  static int lastLeft = 0;
  static int lastRight = 0;

  if (left != lastLeft || right != lastRight){
    startTime = millis();
    lastLeft=left;
    lastRight=right;
    // motors.setSpeeds(left, right);//onward!
    motoCorrect(left, right);// motoCorrect account for offset
  } // or in other words, one motor being more powerful then the other
  else if ( millis() - startTime > time ) {
    return true;//signal for the next command to take place
  };
  return false;//signal that the motors are still in process
}

//############## Less general opperations ##################
// crude reaction events
boolean motorReact (byte eventType) {
  static boolean doneBackinUp = false;
  
  if (doneBackinUp) {
    //create turn right event
    if (eventType < 4){
      // bang a right
      if(goFor(600, 200, -200)){   
        doneBackinUp = false; 
        return true;  
      }
    } else if (eventType < 7) { //then bang a left
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



