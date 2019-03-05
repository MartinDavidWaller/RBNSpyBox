/*
 *  Configuration.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 *  
 *  The configuration data for the device is held in EEPROM (or flash) and the
 *  code in this module is here to save and retrieve the configuration data.
 */

#include <HardwareSerial.h>
#include <EEPROM.h>

#include "Configuration.h"

// The following method is used to calculate a 16 bit checksum for the data held 
// in the configuration. The algorithm is details here:
//
// https://en.wikipedia.org/wiki/Fletcher%27s_checksum

uint16_t Fletcher16( uint8_t *data, int count )
{
   uint16_t sum1 = 0;
   uint16_t sum2 = 0;
   int index;

   for( index = 0; index < count; ++index )
   {
      sum1 = (sum1 + data[index]) % 255;
      sum2 = (sum2 + sum1) % 255;
   }

   return (sum2 << 8) | sum1;
}

void dumpConfiguration(struct Configuration *configuration)
{
  Serial.println("Configuration");
  Serial.printf("   MajorVersion: ........... %d\n",configuration->MajorVersion);
  Serial.printf("   MinorVersion: ........... %d\n",configuration->MinorVersion);
  Serial.printf("   Flags: .................. 0x%02x\n",configuration->Flags);
  if (true == IS_EEPROM_VALID(configuration))
    Serial.printf("                               EEPROM_VALID\n");
  if (true == IS_LED_ENABLED(configuration))
    Serial.printf("                               LED ENABLED\n");    
  if (true == IS_SOUND_ENABLED(configuration))
    Serial.printf("                               SOUND ENABLED\n");    
  Serial.printf("   Hostname: ................ %s\n",configuration->Hostname);
  Serial.printf("   WiFi_SSID: ............... %s\n",configuration->WiFi_SSID);
  Serial.printf("   WiFi_Password: ........... %s\n",configuration->WiFi_Password);
  Serial.printf("   Callsign: ................ %s\n",configuration->Callsign);
  Serial.printf("   TimeZone: ................ %s\n",configuration->TimeZone);
  Serial.printf("   Volume: .................. %d\n",configuration->Volume);
  Serial.printf("   Minutes Between Alerts: .. %d\n",configuration->MinutesBetweenAlerts);
  Serial.printf("   Friend Cycle Count: ...... %d\n",configuration->FriendCycleCount);
}

//
// This method is called to read the configuration information from the EEPROM
//
bool readConfiguration(struct Configuration *configuration)
{
  // Activate EEPROM access
  
  EEPROM.begin(EEPROM_SIZE);

  // We read from the EEPROM byte by byte, Allocate a structure to read
  // into and locate the start of it.

  struct EEPROM_DATA eepromData;
  char *q = (char *)&eepromData;

  // Read all the data from the EEPROM, this will include the checksum
  
  for(int i = 0; i < sizeof(EEPROM_DATA); i++)
    *q++ = EEPROM.read(i);

  // End the EEPROM access
  
  EEPROM.end();

  // Either way, if the checksum is correct or not we will copy over the
  // configuration information read back.

  memcpy(configuration,&eepromData.Configuration,sizeof(Configuration));

  // Display the stored size of checksum

  Serial.printf("readConfiguration, Size = %d, compared to %d\n",eepromData.Size,sizeof(Configuration)); 
  Serial.printf("readConfiguration, CheckSum = %d\n",eepromData.CheckSum);
  
  // We can now check the checksum
  
  uint16_t checkSum = Fletcher16((uint8_t *)&eepromData.Configuration,sizeof(Configuration));

  Serial.printf("readConfiguration, expected CheckSum = %d\n",checkSum);

  // Return the a Bool to indicate if all is OK?
  
  return (sizeof(Configuration) == eepromData.Size) && (checkSum == eepromData.CheckSum);
}

//
// This method is called to write the configuration information to the EEPROM
//
void writeConfiguration(struct Configuration *configuration)
{
  // Activate EEPROM access
  
  EEPROM.begin(EEPROM_SIZE);

  // We read need to allocate an EEPROM_DATA object, populate it with the
  // configuration information passed in, and update size and the checksum.

  struct EEPROM_DATA eepromData;
  memcpy(&eepromData.Configuration,configuration,sizeof(Configuration));
  eepromData.Size = sizeof(Configuration);
  eepromData.CheckSum = Fletcher16((uint8_t *)&eepromData.Configuration,sizeof(Configuration));

  Serial.print("writeConfiguration, checksum = ");
  Serial.println(eepromData.CheckSum);
  
  // We write the data out byte by byte so we need a character pointer to
  // the start of the data.
  
  char *p = (char *)&eepromData;

  // Write out the data
  
  for(int i = 0; i < sizeof(EEPROM_DATA); i++)
    EEPROM.write(i,*p++);

  // End the EEPROM access
  
  EEPROM.end();
}
