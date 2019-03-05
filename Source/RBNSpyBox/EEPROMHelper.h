/*
 *  EEPROMHelper.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
extern void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ); 
extern void writeEEPROMBlock(int deviceaddress, unsigned int eeaddress, byte *buffer, int length);
//extern byte readEEPROM(int deviceaddress, unsigned int eeaddress ); 
extern void readEEPROMBlock(int deviceaddress,unsigned int eeaddress,byte *buffer, int length);
