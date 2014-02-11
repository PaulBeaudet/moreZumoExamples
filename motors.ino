
#include <ZumoMotors.h> //Drive control

//motors
ZumoMotors motors; // 0 is stopped, 400 is full speed
//negitives are reverse speed



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



