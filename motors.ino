
#include <ZumoMotors.h> //Drive control

//motors
ZumoMotors motors; // 0 is stopped, 400 is full speed
//negitives are reverse speed

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
  int offset = promRead(8); //read int from EEPROM  
  // assuming the calibration has been done these are read from eeprom
  if (offset > 400) //discern weak side 
  {//greater than greatest offset, this is left offset case
     offset = offset - 1000; //subtract left boolean
     left = nutralize(left, offset);
  }
  else// untouched is the right offset case
  {
    right = nutralize(right, offset);
  };
  motors.setLeftSpeed(left);//onward!
  motors.setRightSpeed(right);
}

int nutralize(int bearing, int offset)
{// this function writes the offset correctly no matter the direction
  if ( bearing < 0 ) // if the bearing is negitive (reverse)
  {
    return bearing + offset; // if reverse add offset
  }//example -400 + 15 = -385
  else
  {
    return bearing - offset; // if going forward subtract offset
  };//example 400 - 15 = 385
}//in basic; same speeds are returned in relitive directions

void calibrateMotors ()
{
  // find the curve in heading information over 1000 ms at full power
  // turn 180 degrees !! requires the ability to do this with the compass !!
  // -1 in power on strong motor
  // repeat process untill curve disappears 
  // than write power offset and weak side to EEPROM 
  // !! note !!! add 1000 if left is the weak side untouched is assumed weak right  
}

boolean timer(unsigned long time)
{
  static unsigned long startTime = millis();
  static unsigned long setTime;

  if (setTime == time)//check the clock
  {//in this way a different durration is need to reset the clock
    // when in progress
    if ( millis() - startTime > setTime)
    {
      setTime=0;//ready the function for next timed action
      //this makes it posible to repeat the same durration 
      return true;// the time is up!
    }//given durration is still in progress fall thru to false case
  }
  else
  {
    setTime = time; //set the clock
    startTime = millis();//track what the start time was
  }
  return false;//fall thru case
}

// ###### Core functions ################

boolean goFor( long time, int left, int right)// Thats right, long time..
{//returns true while still in progress false when times up
  //thus signifing that the next task can take place
  static unsigned long startTime = millis();
  static int lastLeft = 0;
  static int lastRight = 0;

  if (left != lastLeft || right != lastRight)
  {
    startTime = millis();
    lastLeft=left;
    lastRight=right;
    motors.setLeftSpeed(left);//onward!
    motors.setRightSpeed(right);
  }
  else if ( millis() - startTime > time )
  {
    return true;//signal for the next command to take place
  };
  return false;//signal that the motors are still in process
}

//  ############## Less general opperations ##################

boolean motorReact (byte eventType)//crude
{
  static boolean doneBackinUp = false;
  
  if (doneBackinUp)
  {
    if (eventType < 4)//create turn right event
    {
      if(goFor(600, 200, -200))// then bang a right
      {   
        doneBackinUp = false; 
        return true;  
      }
    }
    else if (eventType < 7)//create left turn event
    {
      if(goFor(600, -200, 200))//then bang a left
      {   
        doneBackinUp = false;
        return true;   
      }     
    }
  } 
  else if (goFor(300,-400,-400))//go backwards
  {
    doneBackinUp = true;
  };
  return false;
}



