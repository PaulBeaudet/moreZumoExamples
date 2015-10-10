// EEPROM.ino ~ Copyright 2015 ~ Paul Beaudet MIT licence - see LICENCE
#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

// Database ledgend // outlines addresses of persistent runtime data 
// amount of addressable space on the uno and leonardo - 1
#define EEPROMSIZE 1024 
#define KEYADDRESS 10 //place where the session key is stored
//Sample space to address starts after key address

// 0-6 = compass calibration information (int)
// 8 = Motor offset 0-400 (int) add 1000 for left
// in this way a boolean is also stored in the motor offset integer
// 10 = key

// meant for writting debugging samples to the eeprom
void promWrite(byte data) {
  static int index= KEYADDRESS + 1;// +1 is the next availible space
  //start past inportant calibration values
  if (index < EEPROMSIZE) {
    //besure not to write outside of the scope of the space availble
    EEPROM.write( index, data);
    index++;
  }
}

// even numbers only!! 0,2,4,6,8 ext: writing ints to byte adresses
void promIntWrite(int data, int place){//writes integers to EEPROM
  EEPROM.write(place, highByte(data)); //splits int into two bytes
  EEPROM.write(place+1, lowByte(data));//splits int into two bytes
}

// even numbers only!! 0,2,4,6,8 ext: reading ints from byte adresses
int promRead(int place){
  return word(EEPROM.read(place), EEPROM.read(place+1));
}// reads an integer from EEPROM, word() creats an int from 2 bytes 

void printResults()//runs in setup
{//prints previous calibration data
  Serial.begin(9600);//bring up the serial interface
  Serial.println("results"); // describe the output
  // add output aka read the results of the last sesion from
  // EEPROM if in fact those results exist

  Serial.print(F("max.x   "));
  Serial.println(promRead(0));
  Serial.print(F("max.y   "));
  Serial.println(promRead(2));
  Serial.print(F("min.x   "));
  Serial.println(promRead(4));
  Serial.print(F("min.y   "));
  Serial.println(promRead(6));

  // Print the heading information that was gathered
  for(int i = 8; i < 100; i++){
    Serial.print(F("EEPROM "));
    Serial.print(i);
    Serial.print(F(" - heading - "));
    Serial.println(EEPROM.read(i));
  }
}

boolean preSession(byte key){
  //location is arbitrary, choose around usespace
  if ( EEPROM.read(KEYADDRESS) == key ){
    return true;
  } else {
    EEPROM.write(KEYADDRESS, key);//if its not it is now!!
    return false;
  };
}

//######debugging functions
void bugLight() {          // flashy light function for debugging
  digitalWrite(LED, HIGH); // turn on LED to show writing state
  delay(10);               // delay long enought to see the light
  digitalWrite(LED, LOW);  // turn off LED to show writing state
}


