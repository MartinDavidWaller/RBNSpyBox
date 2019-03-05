/*
 *  Friends.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */

#include <Arduino.h>
#include <time.h>                      
#include <sys/time.h>       
#include <HardwareSerial.h>

#include "EEPROMHelper.h"
#include "FormatHelper.h"
#include "Friend.h"
#include "Friends.h"
#include "LCD20x4.h"
#include "RBNSpyBox.h"
#include "SortHelper.h"
#include "Sounds.h"
#include "StringHelper.h"
#include "XMLHelper.h"

struct Friend friends[MAX_FRIEND_COUNT] = {0};
struct FriendExt friendsExt[MAX_FRIEND_COUNT] = {0};

void BuildFriends() {

  //friends = (Friend *)calloc(MAX_FRIEND_COUNT,sizeof(Friend));
  
  // Zero all the memory
  
  //memset(&friends[0],0,sizeof(sizeof(Friend) * MAX_FRIEND_COUNT));

  // Mark all friends as free

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];
    
    _friend->LastEmailSent = 0;
    _friend->LastSeen = 0;
        
    strcpy((char*)&_friend->Callsign[0],"");
    _friend->LastFrequency = 0.0f;
    SET_FRIEND_FREE(_friend);
    CLEAR_FRIEND_DIRTY(_friend);
    _friend->MemorySlot = i;
  }

  struct X
  {
    char *callsign;
    char *_name;
    char *comment;
    uint8_t sound;
  } friendsToAdd[] = {
    { "*/QRP",    "Any QRP Operator", "",                 SOUND_NOTE },  
    { "*/MM",     "All at Sea",       "",                 SOUND_NOTE },  
    { "WB6ACU",   "Joe Walsh",        "The Eagles",       SOUND_NOTE },  
    { "W6XRL4",   "Herman Munster",   "The Munsters",     SOUND_NOTE },   
    { "GB*",      "Special Event",    "",                 SOUND_NOTE },   
    { "*G*2OT*",  "Old Timers",       "",                 SOUND_NOTE },  
    { "*G*0PJO*", "Martin Waller",    "Chelmondiston",    SOUND_MARIMBA },  
    { "*G*3PYE*", "Cambridge Club",   "",                 SOUND_MARIMBA },   
    { "*G*3XVL*", "Chris McCarthy",   "Stowupland",       SOUND_JAMES_BOND },  
    { "*G*5LOW*", "GQRP Club",        "",                 SOUND_NOTE },  
    { "*G*8BBC*", "BBC",              "London",           SOUND_NOTE },  
    { "*GX4ZFR*", "FDARS",            "Felixstowe ARS",   SOUND_NOTE },  
    { "KN0WCW",   "FISTS USA",        "",                 SOUND_NOTE },
    { "ZD7*",     "St Helena",        "",                 SOUND_NOTE }
  };
  
  for(int i = 0; i < 14; i++) {

    struct Friend *f = &friends[i];

    StringCopyWithLimit((char*)&f->Callsign[0],friendsToAdd[i].callsign,FRIEND_CALLSIGN_MAX_LENGTH);
    StringCopyWithLimit((char*)&f->Name[0],friendsToAdd[i]._name,FRIEND_NAME_MAX_LENGTH);
    StringCopyWithLimit((char*)&f->Comment[0],friendsToAdd[i].comment,FRIEND_COMMENT_MAX_LENGTH);

    f->LastFrequency = 0.0f;
    CLEAR_FRIEND_FREE(f);  
    SET_FRIEND_SOUND_ENABLED(f);
    f->Sound = (uint8_t)friendsToAdd[i].sound;  
  }
}

void DumpFriends() {

  Serial.printf("Friends - %d in total\n\n", MAX_FRIEND_COUNT);

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    Serial.printf("Friend - %d\n",i);
    DumpFriend(_friend);   
  }
}

/*
// This method is called to add a new friend. We get passed the callsign, name,
// and comment. All we need to do is save it away if not already there.
*/
bool AddNewFriend(const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundSelection) {

  bool retVal = false;

  // We'll do a quick scan to see if we already have that friend in the list

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    struct Friend *_friend = &friends[i];

    // If the friend is not free
    
    if (false == IS_FRIEND_FREE(_friend)) {

      // If the callsigns match then exit, return false
      
      if (0 == strcmp(callsign,&_friend->Callsign[0])) {
        return false;
      }
    }
  }
  
  // We don't already have this callsign so we can go and
  // find a free friend slot.

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    if (true == IS_FRIEND_FREE(_friend)) {

      // Build the new friend

      StringCopyWithLimit((char*)&_friend->Callsign[0],callsign,FRIEND_CALLSIGN_MAX_LENGTH);
      StringCopyWithLimit((char*)&_friend->LastSeenAs[0],"",FRIEND_LAST_SEEN_AS_MAX_LENGTH);
      StringCopyWithLimit((char*)&_friend->Name[0],_name,FRIEND_NAME_MAX_LENGTH);
      StringCopyWithLimit((char*)&_friend->Comment[0],comment,FRIEND_COMMENT_MAX_LENGTH);
      _friend->LastSeen = 0;
      _friend->LastSpeed = 0;
      StringCopyWithLimit((char*)&_friend->LastSpeedUnits[0],"",FRIEND_LAST_SPEED_UNITS_MAX_LENGTH);
      _friend->LastFrequency = 0.0f;
      StringCopyWithLimit((char*)&_friend->LastMode[0],"",FRIEND_LAST_MODE_MAX_LENGTH);
      _friend->LastEmailSent = 0;
      _friend->Sound = soundSelection;
      
      // Clear the free flag and mark it as dirty. Also add the sound enabled
      // flag as required.
      
      CLEAR_FRIEND_FREE(_friend);
      SET_FRIEND_DIRTY(_friend);
      if (true == soundEnabled)
        SET_FRIEND_SOUND_ENABLED(_friend);

      // Make sure that we save our dirty friend!
      
      doSaveDirtyFriends = true;

      // Do a quick sort to make sure all is correct!

      SortFriends();

      // Set the return value and break the loop

      retVal = true;
      break;
    }
  } 

  // Return the result

  return retVal;
}

/*
// This method is called to update a friend. We get passed all the data we need
// to locate and update.
*/
void UpdateFriend(const uint8_t memorySlot, const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundSelection) {

  bool retVal = false;

  // Pull out the friend

  struct Friend *_friend = NULL;
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    _friend = &friends[i];

    // If this is it
    
    if (memorySlot == _friend->MemorySlot) {
      break;
    }
  }    
  
  // Update the friend

  StringCopyWithLimit((char*)&_friend->Callsign[0],callsign,FRIEND_CALLSIGN_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->Name[0],_name,FRIEND_NAME_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->Comment[0],comment,FRIEND_COMMENT_MAX_LENGTH);
  _friend->Sound = soundSelection;
  if (true == soundEnabled)
    SET_FRIEND_SOUND_ENABLED(_friend);
  else
    CLEAR_FRIEND_SOUND_ENABLED(_friend);
    
  // Make sure that we save our dirty friend!

  SET_FRIEND_DIRTY(_friend);      
  doSaveDirtyFriends = true;

  // Do a quick sort to make sure all is correct!

  SortFriends();
}

void RestoreFriend(const uint8_t memorySlot, const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundIndex, const char *lastSeenAs, const time_t lastSeen, const int lastSpeed, const char *lastSpeedUnits, const float lastFrequency, const char *lastMode) {
  
  bool retVal = false;

  // Pull out the friend

  struct Friend *_friend = NULL;
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    _friend = &friends[i];

    // If this is it
    
    if (memorySlot == _friend->MemorySlot) {
      break;
    }
  }    
  
  // Update the friend

  StringCopyWithLimit((char*)&_friend->Callsign[0],callsign,FRIEND_CALLSIGN_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->Name[0],_name,FRIEND_NAME_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->Comment[0],comment,FRIEND_COMMENT_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->LastSeenAs[0],lastSeenAs,FRIEND_LAST_SEEN_AS_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->LastSpeedUnits[0],lastSpeedUnits,FRIEND_LAST_SPEED_UNITS_MAX_LENGTH);
  StringCopyWithLimit((char*)&_friend->LastMode[0],lastMode,FRIEND_LAST_MODE_MAX_LENGTH);
  _friend->LastSeen = lastSeen;
  _friend->LastSpeed = lastSpeed;
  _friend->Sound = soundIndex;
  _friend->LastFrequency = lastFrequency;
  if (true == soundEnabled)
    SET_FRIEND_SOUND_ENABLED(_friend);
  else
    CLEAR_FRIEND_SOUND_ENABLED(_friend);

  // Clear the FREE flag
  
  CLEAR_FRIEND_FREE(_friend);
  
  // Make sure that we save our dirty friend!

  SET_FRIEND_DIRTY(_friend);      
}

bool RemoveFriend(const char *callsign) {

  bool retVal = false;

  // Find the callsign in the list

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    struct Friend *_friend = &friends[i];

    // If the friend is not free
    
    if (false == IS_FRIEND_FREE(_friend)) {

      // If the callsigns match then exit, return false
      
      if (0 == strcmp(callsign,&_friend->Callsign[0])) {

        // This is our friend to delete! Set the free flag and 
        // mark it as dirty
      
        SET_FRIEND_FREE(_friend);
        SET_FRIEND_DIRTY(_friend);

        DumpFriend(_friend);

        // Make sure that we save our dirty friend!
      
        doSaveDirtyFriends = true;

        // Do a quick sort to make sure all is correct!

        SortFriends();

        // Set the return value and break the loop

        retVal = true;

        // Break out of the loop

        break;
      }
    }
  }
  
  // Return the result

  return retVal;
}

void RemoveAllFriends(bool setDirty) {

  // Loop through all friends

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    struct Friend *_friend = &friends[i];

    // Set the free flag and conditionally mark it as dirty

    SET_FRIEND_FREE(_friend);

    if(true == setDirty)
      SET_FRIEND_DIRTY(_friend); 
  }

  // If we have set the dirty flag then set the save dirty friends flag

  if(true == setDirty)
    doSaveDirtyFriends = true;
}

void CommitAllFriends() {

  // Loop through all friends

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    struct Friend *_friend = &friends[i];

    // Set the dirty flag
   
    SET_FRIEND_DIRTY(_friend); 
    Serial.printf("commit %d\n",i);
  }

  // Arrange for the friends to be written out

  doSaveDirtyFriends = true;

  // Do a quick sort to make sure all is correct!

  SortFriends();
}

int GetFriendsAsXML(void textHandler(char *s)) {
  
  textHandler("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  textHandler("<Friends xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> ");
  
  const int xmlBufferSize = 1 * 1024;
  
  static char xmlBuffer[xmlBufferSize];
  char t2[100];

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    // Empty the buffer

    xmlBuffer[0] = '\0';

    if (false == IS_FRIEND_FREE(_friend)) {
      
      // Start off the friend
    
      strcat(xmlBuffer,"<Friend ");

      // Put in the data

      // Callsign
      
      addTagNameValue(xmlBuffer,"Callsign",(char*)&_friend->Callsign[0]);

      // Name
      
      addTagNameValue(xmlBuffer,"Name",(char*)&_friend->Name[0]);
      
      // Comment
      
      addTagNameValue(xmlBuffer,"Comment",(char*)&_friend->Comment[0]);

      // Sound

      if (true == IS_FRIEND_SOUND_ENABLED(_friend))
        addTagNameValue(xmlBuffer,"Sound",GetSoundNameByIndex(_friend->Sound));
      else
        addTagNameValue(xmlBuffer,"Sound","");
          
      // Only add the frequency etc if the call has been seen.
      
      if (0.0d == _friend->LastFrequency) {
        addTagNameValue(xmlBuffer,"LastSeenAs","");
        addTagNameValue(xmlBuffer,"LastSeen","");
        addTagNameValue(xmlBuffer,"LastSpeed","");
        addTagNameValue(xmlBuffer,"LastFrequency",""); 
        addTagNameValue(xmlBuffer,"LastMode","");
        addTagNameValue(xmlBuffer,"LastEmailSent","");
      }
      else {

        // Last Seen As

        addTagNameValue(xmlBuffer,"LastSeenAs",(char*)&_friend->LastSeenAs[0]);

        // Last Seen

        addTagNameValue(xmlBuffer,"LastSeen",FormatTimeAsDateTime(_friend->LastSeen));
        
        // Last Speed
        
        sprintf(t2,"%d WPM",_friend->LastSpeed);
        addTagNameValue(xmlBuffer,"LastSpeed",t2);

        // Last Frequency
        
        //sprintf(t2,"%.4f Mhz",_friend->LastFrequency);
        addTagNameValue(xmlBuffer,"LastFrequency",FormatFrequency(_friend->LastFrequency)); 

        // Last Mode

        addTagNameValue(xmlBuffer,"LastMode",(char*)&_friend->LastMode[0]);
        
        // Last Email Sent

        //if (0 == _friend->LastEmailSent)
          //addTagNameValue(xmlBuffer,"LastEmailSent","");
        //else
          //addTagNameValue(xmlBuffer,"LastEmailSent",FormatTimeAsDateTime(_friend->LastEmailSent));
      }
    
      // Finish off the friend
    
      strcat(xmlBuffer,"/>");

      // Add it to the Friends

      textHandler(xmlBuffer); 
    }
  }

  textHandler("</Friends>");
}

int GetFriendsForEditAsXML(void textHandler(char *s)) {
  
  textHandler("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  textHandler("<FriendsForEdit xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> ");
  
  const int xmlBufferSize = 1 * 1024;
  
  static char xmlBuffer[xmlBufferSize];
  char t2[100];

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    // Empty the buffer

    xmlBuffer[0] = '\0';

    if (false == IS_FRIEND_FREE(_friend)) {
      
      // Start off the friend
    
      strcat(xmlBuffer,"<FriendForEdit ");

      // Put in the data

      // MemorySlot

      sprintf(t2,"%d",_friend->MemorySlot);
      addTagNameValue(xmlBuffer,"MemorySlot",t2);
       
      // Callsign
      
      addTagNameValue(xmlBuffer,"Callsign",(char*)&_friend->Callsign[0]);

      // Name
      
      addTagNameValue(xmlBuffer,"Name",(char*)&_friend->Name[0]);
      
      // Comment
      
      addTagNameValue(xmlBuffer,"Comment",(char*)&_friend->Comment[0]);

      // Sound

      if (true == IS_FRIEND_SOUND_ENABLED(_friend))
        addTagNameValue(xmlBuffer,"Sound",GetSoundNameByIndex(_friend->Sound));
      else
        addTagNameValue(xmlBuffer,"Sound","");
    
      // Finish off the friend
    
      strcat(xmlBuffer,"/>");

      // Add it to the Friends

      textHandler(xmlBuffer); 
    }
  }

  textHandler("</FriendsForEdit>");
}

int GetFriendsForBackup(void textHandler(char *s)) {
  
  textHandler("<Friends> ");
  
  const int xmlBufferSize = 1 * 1024;
  
  static char xmlBuffer[xmlBufferSize];
  char t2[100];

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    // Empty the buffer

    xmlBuffer[0] = '\0';

    if (false == IS_FRIEND_FREE(_friend)) {
      
      // Start off the friend
    
      strcat(xmlBuffer,"<Friend ");

      // Put in the data

      // Callsign
      
      addTagNameValue(xmlBuffer,"Callsign",(char*)&_friend->Callsign[0]);

      // Name
      
      addTagNameValue(xmlBuffer,"Name",(char*)&_friend->Name[0]);
      
      // Comment
      
      addTagNameValue(xmlBuffer,"Comment",(char*)&_friend->Comment[0]);

      // Sound Enabled

      addTagNameValue(xmlBuffer,"SoundEnabled",IS_FRIEND_SOUND_ENABLED(_friend) ? (char *)"on" : (char *)"off");

      // Sound Index

      sprintf(t2,"%d",_friend->Sound);
      addTagNameValue(xmlBuffer,"SoundIndex",t2);
        
      // Sound

      char *sound = GetSoundNameByIndex(_friend->Sound);
      addTagNameValue(xmlBuffer,"Sound",sound != NULL ? sound : (char *)"");
          
      // Only add the frequency etc if the call has been seen.
      
      if (0.0d == _friend->LastFrequency) {
        addTagNameValue(xmlBuffer,"LastSeenAs","");
        addTagNameValue(xmlBuffer,"LastSeen","");
        addTagNameValue(xmlBuffer,"LastSeenSeconds","");
        addTagNameValue(xmlBuffer,"LastSpeed","");
        addTagNameValue(xmlBuffer,"LastSpeedUnits","");
        addTagNameValue(xmlBuffer,"LastFrequency",""); 
        addTagNameValue(xmlBuffer,"LastMode","");
      }
      else {

        // Last Seen As

        addTagNameValue(xmlBuffer,"LastSeenAs",(char*)&_friend->LastSeenAs[0]);

        // Last Seen

        addTagNameValue(xmlBuffer,"LastSeen",FormatTimeAsDateTime(_friend->LastSeen));
        sprintf(t2,"%d",_friend->LastSeen);
        addTagNameValue(xmlBuffer,"LastSeenSeconds",t2);
        
        // Last Speed
        
        sprintf(t2,"%d",_friend->LastSpeed);
        addTagNameValue(xmlBuffer,"LastSpeed",t2);

        // Last Speed Units
        
        addTagNameValue(xmlBuffer,"LastSpeedUnits",_friend->LastSpeedUnits);        

        // Last Frequency
        
        //sprintf(t2,"%.4f Mhz",_friend->LastFrequency);
        addTagNameValue(xmlBuffer,"LastFrequency",FormatFrequency(_friend->LastFrequency)); 

        // Last Mode

        addTagNameValue(xmlBuffer,"LastMode",(char*)&_friend->LastMode[0]);
      }
    
      // Finish off the friend
    
      strcat(xmlBuffer,"/>");

      // Add it to the Friends

      textHandler(xmlBuffer); 
    }
  }

  textHandler("</Friends>");
}

int FriendsSeenCount(){

  // Build the return value
  
  int retVal = 0;

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Locate the friend
    
    struct Friend *_friend = &friends[i];

    if (false == IS_FRIEND_FREE(_friend)) {
      
      // If we have a last seen date / time then we can increment the count

      if (0 != _friend->LastSeen)
        retVal++; 
    }
  }

  // Return the result

  return retVal;
}

Friend *GetFriendBySeenIndex(int requiredIndex) {

  Friend *retVal = NULL;

  // For all the friends we have

  int foundIndex = 0;
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Locate the friend
    
    struct Friend *_friend = &friends[i];

    if (false == IS_FRIEND_FREE(_friend)) {
      
      // If we have a last seen date / time then we can see if this is
      // the one we want.

      if (0 != _friend->LastSeen) {

        if (foundIndex == requiredIndex) {

          // This is the one we want

          retVal = _friend;
          break;
        }
        else
          foundIndex++;
      }
    }
  }

  // Return the result

  return retVal;
}

struct Friend *ProcessCall(char *callsign, int _speed, char *speedUnits, float frequency, char *_mode, char *spot) {

  // Initialise the return value. We don't save the pointer as we will
  // be sorting and when we do that we face the possibility of the data
  // being moved!
  
  int retValMemorySlot = -1;
     
  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    if (false == IS_FRIEND_FREE(_friend)) {

      if (0 != wildcmp((char*)&_friend->Callsign[0], callsign)) {

        // Update the return value memory slot

        retValMemorySlot = _friend->MemorySlot;
        
        Serial.printf("********************* %s\n",callsign);

        // Update the last seen as string

        StringCopyWithLimit((char*)&_friend->LastSeenAs[0],callsign,FRIEND_LAST_SEEN_AS_MAX_LENGTH);
        
        // Update the last seen time!

        time(&_friend->LastSeen);

        // Update the speed and units

        _friend->LastSpeed = _speed;
        StringCopyWithLimit((char*)&_friend->LastSpeedUnits[0],speedUnits,FRIEND_LAST_SPEED_UNITS_MAX_LENGTH);

        // Update the frequency

        _friend->LastFrequency = frequency;

        // Update the mode

        StringCopyWithLimit((char*)&_friend->LastMode[0],_mode,FRIEND_LAST_MODE_MAX_LENGTH);

        // Update the FriendExt entry

        //struct FriendExt *_friendExt = &friendsExt[_friend->MemorySlot];
        //StringCopyWithLimit((char*)&_friendExt->LastSpot[0],spot,FRIENDEXT_LAST_SPOT_MAX_LENGTH);
         
        // Mark the friend as dirty and flag that they need to be saved!

        SET_FRIEND_DIRTY(_friend);
        doSaveDirtyFriends = true;
      }
    }
  }

  // Now we need to go and find the return value - if we found a friend

  struct Friend *retVal = NULL;
  
  if (-1 != retValMemorySlot) {

    // Sort the friends

    SortFriends();

    // No go and find the friend to return
    
    for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

      struct Friend *_friend = &friends[i];

      if (retValMemorySlot == _friend->MemorySlot) {

        retVal = _friend;
        break;
      }
    }
  }

  // Return the result
  
  return retVal;
}

void SaveFriend(struct Friend *_friend) {

  unsigned int eepromBaseAddress = _friend->MemorySlot * sizeof(Friend);

  byte *p = (byte *)_friend;
  
  //for(unsigned int i = 0; i < sizeof(Friend); i++) {
    //writeEEPROM(EEPROM_ADDRESS, eepromBaseAddress + i,*p++); 
  //}

  writeEEPROMBlock(EEPROM_ADDRESS,eepromBaseAddress, p, sizeof(Friend));
}

void SaveFriends() {

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    char number[10];

    clearDisplayLineBuffer();
    sprintf(number,"%d",i + 1);
    addCentreString(number);
    writeDisplayLineBuffer(2);  

    SaveFriend(_friend);

    //ESP.wdtFeed();
    yield();
  }  
}

void LoadFriends() {

  byte *p = (byte *)friends;

  // For all the friends that we have
  
  for(unsigned int i = 0; i < MAX_FRIEND_COUNT; i++) {
    
    // Tell the user which friend we are loading
    
    char number[10];
    clearDisplayLineBuffer();
    sprintf(number,"%d",i + 1);
    addCentreString(number);
    writeDisplayLineBuffer(2);  

    // Locate the friend buffer to write to

    byte *outputPointer = (byte *)&friends[i];

    // EEPROM address from which we will read
    
    int eeaddress = i * sizeof(Friend);

    // Set the bytes to be read and start the loop
    
    int bytesToRead = sizeof(Friend);
    readEEPROMBlock(EEPROM_ADDRESS,eeaddress,outputPointer,bytesToRead);
    //while (bytesToRead > 0) {

      // The chunk that we read will be either the EEPROM page size or the remaining
      // bytes to required.
      
      //int chunk = (bytesToRead > EEPROM_PAGE_SIZE ? EEPROM_PAGE_SIZE : bytesToRead);

      // Read the chunk
      
      //readEEPROMBlock(EEPROM_ADDRESS,eeaddress,outputPointer,chunk);

      // Update the output pointer, EEPROM address and bytes remaining
      // by the chunk size.
      
      //outputPointer += chunk;
      //eeaddress += chunk;
      //bytesToRead -= chunk;
    //}
  }
}

/*
// This method is called to save our dirty friends back to the EEPROM. To keep things
// lively we only save one at a time. Writing to the EEPROM is not that quick. If we
// don't find any dirty friends then we clear down the doSaveDirtyFriends flag.
*/
void SaveDirtyFriends() {

  // For all the friends we have
  
  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    // Pull out the friend
    
    struct Friend *_friend = &friends[i];

    // Is it a dirty friend?
    
    if (true == IS_FRIEND_DIRTY(_friend)) {

      // Clear the dirty flag

      CLEAR_FRIEND_DIRTY(_friend);

      // Save the friend
      
      SaveFriend(_friend);

      return;
    }
  }  
  
  // If we get this far we have no dirty friends left!

  doSaveDirtyFriends = false;

  Serial.printf("**** Nothing more to save!\n");
}

/*
// This method is called to sort the friends array. This is quite complex as we want the
// sort order to change within the list! We want all the friends that we have seen to be
// first in the list ordered by the last seen date/time descending - most recent first.
// For the rest of the list we want to sort on the callsign so they are easy to locate.
*/
void SortFriends() {

  // The swap method is used by all sort requests.
  
  auto _swap = [](void *d, int e1, int e2) {

    // Turn the data pointer into the array of friends and
    // locate each friend.
    
    struct Friend *f = (struct Friend *)d;

    struct Friend *f1 = &f[e1];
    struct Friend *f2 = &f[e2];

    // Build a temporary friend
    
    struct Friend t;

    // Swap the friends over via the temporary friend.
    
    memcpy(&t, f1, sizeof(Friend));
    memcpy(f1, f2, sizeof(Friend));
    memcpy(f2, &t, sizeof(Friend));
  };

  // Start by sorting on whether the friend is free or not. We need to
  // get the free entries to the bottom of the array.
  
  SortAscending(
    (void *)&friends[0],
    [](void *d, int e1, int e2) {

      // Turn the data pointer into the array of friends and
      // locate each friend.
    
      struct Friend *f = (Friend *)d;

      struct Friend *f1 = &f[e1];
      struct Friend *f2 = &f[e2];

      if ((true == IS_FRIEND_FREE(f1)) && (true == IS_FRIEND_FREE(f2))) {

        // Both entries free, return 0

        return 0;
      }
      else if (true == IS_FRIEND_FREE(f1)) {

        // F1 if free but not F2

        return 1;
      }
      else {

        // F2 if free but not F1

        return -1;
      }
    },
    _swap,0,MAX_FRIEND_COUNT - 1);

  // Next we need to find the last non-free entry.

  int lastNonFreeEntry = 0;

  for(int i = 0; i < MAX_FRIEND_COUNT; i++) {

    struct Friend *_friend = &friends[i];

    if (false == IS_FRIEND_FREE(_friend)) {
      lastNonFreeEntry = i;
    }
    else
      break;
  }

  if (0 == lastNonFreeEntry)
    return;

  // We now know the extent of our true friends!
  
  // Next we sort on the last seen data. Most recent first!

  SortAscending(
    (void *)&friends[0],
    [](void *d, int e1, int e2) {

      // Turn the data pointer into the array of friends and
      // locate each friend.
      
      struct Friend *f = (Friend *)d;

      struct Friend *f1 = &f[e1];
      struct Friend *f2 = &f[e2];

      if ((0 == f1->LastSeen) && (0 == f2->LastSeen)) {

        // Neither entry has been seen, return 0

        return 0;
      }
      else if ((0 == f1->LastSeen) && (0 != f2->LastSeen)) {

        // F1 has not been seen but F2 has, return +1

        return 1;
      }
      else if ((0 != f1->LastSeen) && (0 == f2->LastSeen)) {

        // F1 has been seen but not F2, return -1

        return -1;
      }
      else {

        // We have valid last seen dates on both, we need to compare them

        if (f1->LastSeen == f2->LastSeen) {

          // Both the same, return 0

          return 0;
        }
        else if (f1->LastSeen > f2->LastSeen) {

          // F1 > F2, return -1

          return -1;
        }
        else {

          // F1 < F2, return 1

          return 1;
        }        
      }
    },
    _swap,0,lastNonFreeEntry);

  // Next we need to find the last entry that we have seen.

  int lastEntrySeen = 0;

  for(int i = 0; i < lastNonFreeEntry; i++) {

    struct Friend *_friend = &friends[i];

    if (0 != _friend->LastSeen) {
      lastEntrySeen = i;
    }
    else
      break;
  }

  if (0 == lastEntrySeen)
    return;    

  // We can now sort by Callsign

  SortAscending(
    (void *)&friends[0],
    [](void *d, int e1, int e2) {

      // Turn the data pointer into the array of friends and
      // locate each friend.
      
      Friend *f = (Friend *)d;

      struct Friend *f1 = &f[e1];
      struct Friend *f2 = &f[e2];

      // Return the standard string compare result
        
      return strcmp(&f1->Callsign[0],&f2->Callsign[0]);
    },
    _swap,lastEntrySeen + 1,lastNonFreeEntry);      
}

struct FriendExt *GetFriendExt(struct Friend *_friend) {

  struct FriendExt *retVal = &friendsExt[_friend->MemorySlot];

  return retVal;
        
}
