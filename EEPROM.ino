#include <EEPROM.h> // Persitent EEPROM memory storage (runtime persistence)

#define SESSIONPLACE 100

void promWrite(byte data)
{
  static int index= 8; //start at 8 given 0-7 is taking by calibration data

  if (index < SESSIONPLACE)
  {
    EEPROM.write( index, data);
    index++;
  }
}

void promIntWrite(int data, int place)//even numbers only!! 0,2,4,6,8 ext
{//writes integers to EEPROM
  EEPROM.write(place, highByte(data));//splits int into two bytes
  EEPROM.write(place+1, lowByte(data));//splits int into two bytes
}

int promRead(int place)//even numbers only!! 0,2,4,6,8 ext
{
  return word(EEPROM.read(place), EEPROM.read(place+1));
}

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

  for(int i = 8; i < 100; i++)
  {// Print the heading information that was gathered
    Serial.print(F("EEPROM "));
    Serial.print(i);
    Serial.print(F(" - heading - "));
    Serial.println(EEPROM.read(i));
  }
}

boolean preSession(byte key)
{
  if ( EEPROM.read(SESSIONPLACE) == key )
  {//location is arbitrary, choose around usespace
    return true;   
  } 
  else
  {
    EEPROM.write(SESSIONPLACE, key);//if its not it is now!!
    return false;
  };
}

//######debugging functions
void bugLight()
{//flashy light function for debugging
  digitalWrite(LED, HIGH); //turn on LED to show writing state
  delay(10);//delay long enought to see the light
  digitalWrite(LED, LOW); //turn off LED to show writing state
}


