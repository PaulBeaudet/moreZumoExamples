
#include <ZumoMotors.h> //Drive control

//motors
ZumoMotors motors; // 0 is stopped, 400 is full speed
//negitives are reverse speed

void stopMotors()
{
  motors.setLeftSpeed(0);//stop the motors
  motors.setRightSpeed(0);
}

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


