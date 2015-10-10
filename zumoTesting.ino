 //zumoTesting.ino ~ Copyright 2015 Paul Beaudet ~ MIT licence ~ See LICENCE 
 // Detailed and explained Sumo sketch for the Zumo Arduino Shield

// definitions of variables and defined values
#define LED 13  // for debuging durring run time (pin 13)
byte event = 0; // event to signal motor actions 

//Part of every Sketch: Executes once in the begining
void setup(){
  buttonUp(); // set up the button
  //testingStuff(); 
}

// Part of every Sketch: Continuously runs over and over until out of power
void loop(){ 
  byte situation = buttonWatch(); // monitors button
  
  // 0-start music->1-start motors->2-stop music->3-stop motors->0
  if (situation == 2 || situation == 3){
    marioInProgress(); //plays notes of Mario theme song as appropriate
  }
  if (situation == 1 || situation == 2){
    if ( event ) { // given an event was detected last loop request reaction
                   //when the reaction is complete 
      if (motorReact(event)){ event = 0; } //signal that reaction is done
    } else {                               // absent other events
      goFor(9000, 400, 400);               //drive forward
      event = reflectEvent();              // look for obstacels
    };
  } else { stopMotors(); };  // stop motors if the situation is zero
  
  //pickupStop();//stops the zumo when picked up
}

//###########################################################################END MAIN LOOP

//######### change this number to recalibrate
#define RESETKEY 0 // <-----needs to be a byte value
//##################### 

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

void pickupStop(){
  if(!event && checkForPickup()){
    //in event case interferance creates false positives
    stopMotors();//stop the presses! Zumo has been picked up!
    holdForButton();//wait for a button press to resume
  }
}

