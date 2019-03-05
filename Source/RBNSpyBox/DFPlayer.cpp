/*
 *  DFPlayer.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <Arduino.h>
#include "DFPlayer.h"
#include "RBNSpyBox.h"

/*
 * This module provides support for the DFPlayer device. The device is best described
 * here:
 * 
 * http://www.picaxe.com/docs/spe033.pdf
 */

/*
 * This method is called to calculate and insert the DF Player checksum
 * into the command buffer.
 */
void DFPlayerSetChecksum(uint8_t* commandBuffer)
{
  // Start by calculating the checksum
  
  uint16_t sum = 0;
  for (int i = DFPLAYER_COMMAND_BUFFER_VERSION; i < DFPLAYER_COMMAND_BUFFER_HIBYTE_CHECKSUM; i++)
  {
    sum += commandBuffer[i];
  }
  sum *= -1;

  // Now put the checksum into the buffer
  
  commandBuffer[DFPLAYER_COMMAND_BUFFER_HIBYTE_CHECKSUM] = (sum >> 8);
  commandBuffer[DFPLAYER_COMMAND_BUFFER_LOWBYTE_CHECKSUM] = (sum & 0xff);
}

/*
 * This method is called to send a given command to the DF Player. We
 * can pass in two arguments. It will build a buffer from these arguments,
 * insert the checksum and send it off.
 */
void DFPlayerSendCommand(void writeHandler(uint8_t b), uint8_t command, uint8_t arg1, uint8_t arg2)
{
  // Allocate a buffer
  
  uint8_t b[DFPLAYER_COMMAND_BUFFER_SIZE];

  // Populate the buffer
  
  b[DFPLAYER_COMMAND_BUFFER_STARTBYTE] = 0x7e;
  b[DFPLAYER_COMMAND_BUFFER_VERSION] = 0xff;
  b[DFPLAYER_COMMAND_BUFFER_LENGTH] = 0x06;
  b[DFPLAYER_COMMAND_BUFFER_COMMAND] = command;
  b[DFPLAYER_COMMAND_BUFFER_REQUEST_ACK] = 0x00;
  b[DFPLAYER_COMMAND_BUFFER_HIBYTE_ARGUMENT] = arg1;
  b[DFPLAYER_COMMAND_BUFFER_LOWBYTE_ARGUMENT] = arg2;

  // DFPLAYER_COMMAND_BUFFER_HIBYTE_CHECKSUM and DFPLAYER_COMMAND_BUFFER_LOWBYTE_CHECKSUM are reserved for the checksum
  
  b[DFPLAYER_COMMAND_BUFFER_ENDCODE] = 0xef;

  // Insert the checksum
  
  DFPlayerSetChecksum(&b[0]);

  // For all the bytes in the buffer send them off.
  
  for (byte i=0; i < DFPLAYER_COMMAND_BUFFER_SIZE; i++) 
    //DFPlayerSerialPort.write((uint8_t)b[i]);  
    writeHandler((uint8_t)b[i]); 
}

/*
 * This method is called to initialise the DFPlayer and must be
 * called first.
 */
void DFPlayerInitialise(void writeHandler(uint8_t b)) {

  pinMode(DFPLAYER_BUSY_PIN,INPUT);
   
  // Send the initialise command
  
  DFPlayerSendCommand(writeHandler,DFPLAYER_COMMAND_INITIALIZE,0x0,0x0);

  // Delay to let it happen
  
  delay(1000);
}

/*
 * This method is called to set the volume of the DFPlayer. The volume
 * should be in the range 0 - 30.
 */
void DFPlayerSetVolume(void writeHandler(uint8_t b),uint8_t volume) {

  // Send the set volume command
  
  DFPlayerSendCommand(writeHandler,DFPLAYER_COMMAND_SET_VOLUME,0,volume); 

  // Delay to let it happen
  
  delay(1000);
}

/*
 * This method is called to play a given MP3 file. The MP3 file is specified
 * by its manifest value.
 */
void DFPlayerPlay(void writeHandler(uint8_t b), int mp3ToPlay, bool withWait)
{
  // Send the command to play
  
  DFPlayerSendCommand(writeHandler,DFPLAYER_COMMAND_PLAY,1,mp3ToPlay);

  // Small pause and...
  
  delay(100);

  // ...wait for it to finish

  if (true == withWait) 
    while (0 == digitalRead(DFPLAYER_BUSY_PIN)); 
}
