///*
//libraries
#include <Wire.h>
#include <LSM303.h>

//################################Function variables setup

//compass
LSM303 compass;
#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate
LSM303::vector<int16_t> running_min = {
  32767, 32767, 32767}
, running_max = {
  -32767, -32767, -32767};
//set up object variables running min/max as ints //creats 6 assosiated variables
//lowest/highest -2047/2047

//################################### End variables


void compassSetup()
{
  Wire.begin(); //open I2C bus as the master to communicate with compass/accel
  compass.init();// Initiate LSM303
  compass.enableDefault();// Enables accelerometer and magnetometer
  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate
  /*
  Calibration values; the default values of +/-32767 for each axis
   lead to an assumed magnetometer bias of 0. Use the Calibrate example
   program to determine appropriate values for your particular unit.
   */
  // the following needs to be done to give perspective to the compass
  // in the following case perspective is set to maxim values
  // thus giving non useful results.
  //compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  //compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
  ///*
}

void calibrateCompass()
{
  for(unsigned char index = 0; index < CALIBRATION_SAMPLES; index ++)
  {// Take a reading of the magnetic vector and store it in compass.m
    goFor(9001, -150, 150);// turn sowly for over 9000ms
    compass.read();
    running_min.x = min(running_min.x, compass.m.x);//thanks for sharing what these methods do...
    running_min.y = min(running_min.y, compass.m.y);// I'll just guess
    running_max.x = max(running_max.x, compass.m.x);// my guess is decr/increments the max/min
    running_max.y = max(running_max.y, compass.m.y);// based on the reading
    delay(200);//delay inorder to be sure reading increments as robot spins 
  }
  stopMotors();//loop will end before gofor completes 

    // write calibration information to EEPROM

  promIntWrite(running_max.x, 0);//write intergers
  promIntWrite(running_max.y, 2);//to even places in EEPROM
  promIntWrite(running_min.x, 4);// this alows the arduino to remember
  promIntWrite(running_min.y, 6);// the calibration 

  // Set calibrated values to compass.m_max and compass.m_min
  compass.m_max.x = running_max.x;
  compass.m_max.y = running_max.y;
  compass.m_min.x = running_min.x;
  compass.m_min.y = running_min.y;

}

void rememberCalibration() // remembers calibration from last session
{
  compass.m_max.x = promRead(0);// read calibration value
  compass.m_max.y = promRead(2);// into compass.m variables
  compass.m_min.x = promRead(4);
  compass.m_min.y = promRead(6);
}


#define BUFFER 15 // can amount to sensitivity at degree increments

void headingRecord()// reports the heading to the serial monitor
{
  static byte index = 8;//initiate as the next adress space in eeprom
  static byte lastHeading;

  float more;
  float orLess;

  if (index < 100)//only in the case of availble recording spots 
  {//less than the key possition which should allow for 91 readings 
    compass.read();
    float heading = averageHeading();//read the average heading 
    //as opposed to the library heading method
    //################################################testing
    if (lastHeading + BUFFER > 360)
    {
      more = lastHeading - BUFFER; // more will just be a larger number
    }
    else
    {
      more = lastHeading + BUFFER;
    };
    if (lastHeading - BUFFER < 0)
    {
      orLess = lastHeading + BUFFER; // orless will just be a smaller number
    }
    else
    {
      orLess = lastHeading - BUFFER;
    };
    if (heading < more || heading > orLess)//!! potential overflow!!!
      //#@N#########################################testing
    {
      digitalWrite(LED, HIGH); //turn the LED on to show writing state
      lastHeading= (byte) heading;//cast the heading as a byte for recorded degrees all
      EEPROM.write(index, lastHeading); //record data for testing
      index++;//increment the index to write to the next place in eeprom
      //upon next reading
    }
  }
}

template <typename T> float heading(LSM303::vector<T> v)//?? needed for average heading function!
{
  float x_scaled =  2.0*(float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0*(float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled)*180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

// Average 10 vectors to get a better measurement and help smooth out
// the motors' magnetic interference.
float averageHeading()
{
  LSM303::vector<int32_t> avg = {
    0, 0, 0      };

  for(int i = 0; i < 10; i ++)
  {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;

  // avg is the average measure of the magnetic vector.
  return heading(avg);
}

//*/
