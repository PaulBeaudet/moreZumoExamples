// serial.ino Copyright 2016 Paul Beaudet ~ MIT LICENSE
// This code will allow the Zumo to communiacate w/ a serial controler device like an Android phone or Raspi

char* gather(char incoming){  // returns if a command has been recieved
  static char packet[2] = { 0, 0 };  // store potential incoming command packet
                                     // reset if a full packet has been recieved
  if(packet[0] && packet[1]){packet[0] = 0; packet[1] = 0;}
  if(incoming){              // data available from Serial? 0xff signals no data
    if(packet[0]){          // do we already have the first char in packet?
      packet[1] = incoming; // then this this the second
      return packet;        // in this case a "packet" is 2 chars, we are done
    }                       // return pointer to first address of the packet
    else if(validCommand(incoming)){packet[0] = incoming;}
    // otherwise this is the first char in the packet, record it as such
  }
  return 0; // protocol does nothing until 
}

char validCommand(char cmd){
  if     (cmd == MOVEMENT){;}
  else if(cmd == SPEED){;}
  else if(cmd == PROGRAM){;}
  else if(cmd == MUSIC){;}
  else{
    Serial.print(F("E:Invalid cmd ")); Serial.println(cmd);
    cmd = 0; // set command to zero if not a valid command
  }
  return cmd;
}

