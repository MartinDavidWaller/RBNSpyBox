/*
 *  Friend.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <time.h>                      
#include <sys/time.h>                   
#include <HardwareSerial.h>
#include <WiFi.h>

#include "FormatHelper.h"
#include "Friend.h"

void DumpFriend(struct Friend *_friend)
{ 
  Serial.printf("  sizeof(Friend): ........... %d\n", sizeof(Friend));  
  Serial.printf("  LastEmailSent: ............ %s\n", FormatTimeAsDateTime(_friend->LastEmailSent));
  Serial.printf("  LastSeen: ................. %s\n", FormatTimeAsDateTime(_friend->LastSeen));
  Serial.printf("  LastFrequency: ............ %f\n", _friend->LastFrequency);
  Serial.printf("  Callsign: ................. %s\n", &_friend->Callsign[0]);
  Serial.printf("  LastSeenAs: ............... %s\n", &_friend->LastSeenAs[0]);
  Serial.printf("  Comment: .................. %s\n", &_friend->Comment[0]);
  Serial.printf("  Name: ..................... %s\n", &_friend->Name[0]);
  Serial.printf("  LastMode: ................. %s\n", &_friend->LastMode[0]);
  //Serial.printf("  FreeFlag: ................. %s\n", FRIEND_FREE == _friend->FreeFlag ? "FREE" : "NOT FREE");
  Serial.printf("  Flags: .................... 0x%02x\n", _friend->Flags);
  if (true == IS_FRIEND_DIRTY(_friend))
    Serial.printf("                                  DIRTY\n");
  if (true == IS_FRIEND_FREE(_friend))
    Serial.printf("                                  FREE\n");
  if (true == IS_FRIEND_SOUND_ENABLED(_friend))
    Serial.printf("                                  SOUND ENABLED\n");
  Serial.printf("  Sound: .................... %d\n", _friend->Sound);
  Serial.printf("  memorySlot: ............... %d\n", _friend->MemorySlot);
  Serial.printf("\n");
}
