
void promIntWrite(int data, int place)//even numbers only!! 0,2,4,6,8 ext
{//writes integers to EEPROM
  EEPROM.write(place, highByte(data));
  EEPROM.write(place+1, lowByte(data));
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
  if ( EEPROM.read(100) == key )
  {//location is arbitrary, choose around usespace
    return true;   
  } 
  else
  {
    EEPROM.write(100, key);//if its not it is now!!
    return false;
  };
}

