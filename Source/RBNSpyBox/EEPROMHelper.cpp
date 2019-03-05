/*
 *  EEPROMHelper.cppp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <Arduino.h>
#include <Wire.h>

// AT24C64
//
// See http://ww1.microchip.com/downloads/en/devicedoc/doc0336.pdf

#define AT24C64_PAGE_SIZE 32

//void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
//{
  //Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8));   // MSB
  //Wire.write((int)(eeaddress & 0xFF)); // LSB
  //Wire.write(data);
  //Wire.endTransmission();
 
  //delay(5);
//}

void writeEEPROMBlock(int deviceaddress, unsigned int eeaddress, byte *buffer, int length)
{
  int remainingBytesToWrite = length;
  int nextByteToWrite = 0;
  
  // We need to write this out taking into account the AT24C64 page size.

  int startPage = eeaddress / AT24C64_PAGE_SIZE;
  int startPageOffset = eeaddress % AT24C64_PAGE_SIZE;

  // If we have non zero start page offset value we need to write
  // that made bytes and then start writing page size chunks again
  // based as the next page boundary.

  if (0 != startPageOffset) {

    // Start the transmission and set the starting address
    
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB

    // Write out bytes from the buffer until we hit the end of the
    // page or the end of the data.
    
    for (byte c = 0; c < (AT24C64_PAGE_SIZE - startPageOffset) && remainingBytesToWrite > 0; c++) {

      // Write out the byte
      
      Wire.write(buffer[nextByteToWrite]);

      // Update the buffer index and reduce the bytes to write count
      
      nextByteToWrite++;
      remainingBytesToWrite--;
      eeaddress++;
    }

    // End the transmission
    
    Wire.endTransmission();

    // Delay for the write cycle time
    
    delay(10);
  }

  // At this point we know that we are either done or sitting on
  // a page size boundary. We can nnow write page by page until
  // we are complete.

  if (remainingBytesToWrite > 0) {

    while (remainingBytesToWrite > 0) {
      
      // Start the transmission and set the starting address
    
      Wire.beginTransmission(deviceaddress);
      Wire.write((int)(eeaddress >> 8)); // MSB
      Wire.write((int)(eeaddress & 0xFF)); // LSB

      // Write out bytes from the buffer until we hit the end of the
      // page or the end of the data.
    
      for (byte c = 0; c < AT24C64_PAGE_SIZE && remainingBytesToWrite > 0; c++) {

        // Write out the byte
      
        Wire.write(buffer[nextByteToWrite]);

        // Update the buffer index and reduce the bytes to write count
      
        nextByteToWrite++;
        remainingBytesToWrite--;
        eeaddress++;
      }

      // End the transmission
    
      Wire.endTransmission();

      // Delay for the write cycle time
    
      delay(10);
    }
  }
}

//byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
//{
  //byte rdata = 0xFF;
 
  //Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8));   // MSB
  //Wire.write((int)(eeaddress & 0xFF)); // LSB
  //Wire.endTransmission();
 
  //Wire.requestFrom(deviceaddress,1);
 
  //if (Wire.available()) rdata = Wire.read();
 
  //return rdata;
//}

// maybe let's not read more than 30 or 32 bytes at a time!

//void readEEPROMBlock(int deviceaddress,unsigned int eeaddress,byte *buffer,int length) {

  // Begin the transmission, send the device address and the address of
  // the data we want.
  
  //Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8)); // MSB
  //Wire.write((int)(eeaddress & 0xFF)); // LSB
  //Wire.endTransmission();

  // Request the block
  
  //Wire.requestFrom(deviceaddress,length);

  // Read and save the block away
  
  //int c = 0;
  //for ( c = 0; c < length; c++ )
    //if (Wire.available()) buffer[c] = Wire.read();
//}

void readEEPROMBlock(int deviceaddress,unsigned int eeaddress,byte *buffer,int length) {

  int remainingBytesToRead = length;
  int nextByteToRead = 0;
  
  while (remainingBytesToRead > 0) {

    // Begin the transmission, send the device address and the address of
    // the data we want.
  
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();

    // Request the block

    int thisChunkSize = remainingBytesToRead > AT24C64_PAGE_SIZE ? AT24C64_PAGE_SIZE : remainingBytesToRead;
    Wire.requestFrom(deviceaddress,thisChunkSize);

    // Read and save the block away
  
    for (int c = 0; c < thisChunkSize; c++ ) {
      if (Wire.available()) {
        
        buffer[nextByteToRead] = Wire.read();

        // Update the buffer index and reduce the bytes to read count
      
        nextByteToRead++;
        remainingBytesToRead--;
        eeaddress++;
      }
    }
  }
}
