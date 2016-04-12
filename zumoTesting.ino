 /* zumoTesting.ino ~ Copyright 2015-2016 Paul Beaudet ~ MIT licence ~ See LICENCE 
  * Detailed and explained Sumo sketch for the Zumo Arduino Shield covering
  * Succesfully built with Arduino IDE 1.6.5
  * SEE required libraries in respective function files i.e. motor.ino imu.ino Reflectence.ino
  * async motor control 
  * async buzzer (music) control
  * async button control cycle through 4 modes
  * Remote serial control via android phone or raspi payload
  */

#include <SimpleTimer.h>
// http://playground.arduino.cc/Code/SimpleTimer#GetTheCode
SimpleTimer timer;    // instantiate timer object

// ### definitions of constant numbers ###
#define LED 13        // for debuging durring run time (pin 13)
#define MONITOR 0     // param for a polymorphic fuction that has a monitor mode
#define TRIGGER 1     // truth parameter = true || high
#define RESETKEY 0    // < 256: change to reset persistent calibration options
#define OFFSET 13     // hard coded offset of the motors set this proper to drive straight
#define BTNEVENTS 4   // number of events zumo button can trigger

void setup(){           // ### Part of every Sketch: runs once on start up ###
  buttonUp();           // set up the button
  motorSetup();         // set up motor drivers
  Serial.begin(115200); // set up communication to controler (rasppi/android) TODO change to 115200
  lsm303Up();           // set up the compass and accelometer
  timer.setInterval(5000, sensorReadout); // set sensors to read out to master
}

void loop(){          // ### Part of every Sketch: Continuously loops ### 
  /*  Independent oppration modes are determined by on board button press
   *  1st press: start sumo program
   *  2nd press: start music
   *  3rd press: stop sumo progam
   *  4th press: stop music / Remote operation mode
   */
  // byte onPress = buttonWatch(); // monitors button
  // detect cycling through a number of button events
  byte onPress = buttonEvents(BTNEVENTS);
  // on press two an three play music
  if (onPress == 2 || onPress == 3){marioInProgress();}
  // on press one and two run sumo
  if (onPress == 1 || onPress == 2){sumo();}
  // stop autonomous drive condition
  if(onPress == 3){stopMotors();}
  // if not sumo defualt condition is remote control
  if(onPress == 0){onListen();} // listen for remote commands from host

  timer.run();
} 

// ### END MAIN LOOP ###

void sensorReadout(){
  lsmReadout(); // readout compass and accellerometer values
  reflectOut(); // readout reflectance values
}

// High level functions, see assosiated .ino files for specific function logic
void sumo(){                           // run async to sumo
  static byte event = 0;               // event determining motor actions
  if ( event ) {                       // if event react to it 
    if (motorReact(event)){event = 0;} // signal that reaction is done
  } else {                             // absent other events
    goFor(9000, 400, 400);             // drive forward
    event = reflectEvent();            // look for obstacels
  };
}

#define MOVEMENT 'M'  // signal char for direction change
#define SPEED 'S'     // signal char for speed change
#define PROGRAM 'P'   // signal char for programs
#define MUSIC 'C'     // signal char for music

void onListen(){
  if(Serial.available()){                 // listen for serial events
    char* packet = gather(Serial.read()); // potential pointer to packet of incoming bytes
    if(packet){                           // if we have recieved a pointer to a packet
      if(packet[0] == MOVEMENT){          // if this is a movement packet
        steer(packet[1]);                 // do some driving in directions 
      } else if(packet[0] == SPEED){      // if this is a speed packet
        speedPower(packet[1]);            // change speed of zumo
      } else if(packet[0] == PROGRAM){    // if this is a program packet
        // set a program mode
      } else if(packet[0] == MUSIC){      // music packet sent
        horn(packet[1]);                  // play a sound
      }
    }
  }
}

// steer constants
#define BACK_LEFT  '1'
#define BACK       '2'
#define BACK_RIGHT '3'
#define SPIN_LEFT  '4'
#define STOP       '5'
#define SPIN_RIGHT '6'
#define FWD_LEFT   '7'
#define FWD        '8'
#define FWD_RIGHT  '9'
#define MAX_POWER 400 // max drive speed

void steer(char mode){
  if(mode){
    int pwr = speedPower(MONITOR); // figure current speed of zumo
    if     ( mode == STOP)      {stopMotors();}
    else if( mode == BACK_LEFT) {speedsSet(-(pwr/2), -pwr);}
    else if( mode == BACK)      {speedsSet(-pwr, -pwr);}
    else if( mode == BACK_RIGHT){speedsSet(-pwr, -(pwr/2));}
    else if( mode == SPIN_LEFT) {speedsSet(-pwr, pwr);}
    else if( mode == SPIN_RIGHT){speedsSet(pwr, -pwr);}
    else if( mode == FWD_LEFT)  {speedsSet(pwr/2, pwr);}
    else if( mode == FWD)       {speedsSet(pwr, pwr);}
    else if( mode == FWD_RIGHT) {speedsSet(pwr, pwr/2);}
    else {invalid(mode);}
  }
}

int speedPower(char mode){
  static int zumoSpeed = MAX_POWER; // defualt to maximum power
  if(mode){
    if     (mode == '1'){zumoSpeed = MAX_POWER * 0.25;} // 25% spd
    else if(mode == '2'){zumoSpeed = MAX_POWER * 0.50;} // 50% spd
    else if(mode == '3'){zumoSpeed = MAX_POWER * 0.75;} // 75% spd
    else if(mode == '4'){zumoSpeed = MAX_POWER;}
    else{invalid(mode);}
  }
  return zumoSpeed;
}

void invalid(char entry){
  Serial.print(entry);
  Serial.println(F(":Invalid entry"));
}

void testingStuff(){  
  lsm303Up(); // intiates the lsm303 compass and accelerometer
  if (preSession(RESETKEY)){ // previous session returns true or false
    printResults(); // Prints the results of the last session if it existed
    // open the serial monitor to see the last samples of heading results !!
    // rememberCalibration();// write the old calibration from EEPROM 
  } else {
    // calibrate the compass if previous calibration is forgoton
    // calibrateCompass();
  };
}

void pickupStop(byte event){
  if(!event && checkForPickup()){
    //in event case interferance creates false positives
    stopMotors();//stop the presses! Zumo has been picked up!
    holdForButton();//wait for a button press to resume
  }
}

// ### debugging functions ###
void bugLight() {          // flashy light function for debugging
  digitalWrite(LED, HIGH); // turn on LED to show writing state
  delay(10);               // delay long enought to see the light
  digitalWrite(LED, LOW);  // turn off LED to show writing state
}
