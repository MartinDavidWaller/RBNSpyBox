/*
 *  Sounds.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <Arduino.h>
#include "DFPlayer.h"
#include "Sounds.h"
#include "XMLHelper.h"

// The following array names all the available sounds. Sounds are numbered
// 1 upwards so we'll have an empty zero!

#define MAX_SOUND_INDEX 48

char *soundNames[MAX_SOUND_INDEX] = {
  "",
  "Apex",
  "Aurora",
  "Bamboo",
  "Beacon",
  "Bulletin",
  "By The Seaside",
  "Chimes",
  "Chord",
  "Circles",
  "Circuit",
  "complete",
  "Constellation",
  "Cosmic",
  "Crystals",
  "Dambusters",
  "Dambusters Wondering",
  "Hello",
  "Hillside",
  "Illuminate",
  "Input",
  "Intel Inside",
  "James Bond",
  "Keys",
  "Marimba",
  "Night Owl",
  "Nokia Standard",
  "Note",
  "Opening",
  "Original Nokia Tune",
  "Playtime",
  "Popcorn",
  "Presto",
  "Pulse",
  "Radar",
  "Radiate",
  "Reflection",
  "Ripples",
  "Sencha",
  "Signal",
  "Silk",
  "Slow Rise",
  "Stargaze",
  "Summit",
  "Synth",
  "Twinkle",
  "Uplift",
  "Waves"
};

QueueHandle_t soundQueue;
TaskHandle_t pxCreatedTask;

void soundActionTask(void *pvParameters)
{
  auto writefunction = [](uint8_t b) { Serial2.write(b); };
  
  for( ;; )
  {
    if(soundQueue != 0)
    {
      struct SoundAction soundAction;
      
      if (xQueueReceive(soundQueue,&(soundAction),(TickType_t)portMAX_DELAY))
      {
        
#if DEBUG_SOUND_QUEUE == 1
        Serial.printf("Type: ................ %d\n",soundAction.Type);
        Serial.printf("Track: ............... %d\n",soundAction.Track);
        Serial.printf("Volume: .............. %d\n",soundAction.Volume);
        Serial.printf("Restore Volume: ...... %d\n",soundAction.RestoreVolume);
#endif

        // Decode the request type
        
        switch(soundAction.Type)
        {
          case INITIALIZE:
            DFPlayerInitialise(writefunction);
            break;
            
          case SET_VOLUME:
            DFPlayerSetVolume(writefunction,soundAction.Volume);           
            break;
            
          case PLAY_TRACK:
            DFPlayerPlay(writefunction,soundAction.Track,true);
            break;
            
          case PLAY_TRACK_WITH_VOLUME:
  
            DFPlayerSetVolume(writefunction,soundAction.Volume);
            DFPlayerPlay(writefunction,soundAction.Track,true);  
            delay(2000);
            DFPlayerSetVolume(writefunction,soundAction.RestoreVolume);
            break;
            
          default:
            break;
        }
      }
    }
  }
}

void InitializeSound() {

  // Initialise the serial port talking to the DF Player
  
  Serial2.begin(DFPLAYER_BAUD_RATE);  // pin 16=RX, pin 17=TX

  // Build the sound action queue
  
  soundQueue = xQueueCreate(10,sizeof(struct SoundAction));  

  // Build the sound action task

  xTaskCreate(
    soundActionTask,            // Function that implements the task.
    "soundActionTask",          // Text name for the task. 
    2 * 1024,                   // Stack size in words, not bytes. 
    ( void * ) 1,               // Parameter passed into the task. 
    tskIDLE_PRIORITY,           // Priority at which the task is created. 
    &pxCreatedTask );           // Handle address

  // Queue the initialize
      
  struct SoundAction sa;
  sa.Type = INITIALIZE;
  xQueueSend(soundQueue,(void *)&sa,(TickType_t)0);    
}

void SetSoundVolume(const uint8_t volume) {

  struct SoundAction sa;
  sa.Type = SET_VOLUME;
  sa.Volume = volume;
  xQueueSend(soundQueue,(void *)&sa,(TickType_t)0);  
}

void PlaySound(const uint8_t sound) {

  struct SoundAction sa;
  sa.Type = PLAY_TRACK;
  sa.Track = sound;
  xQueueSend(soundQueue,(void *)&sa,(TickType_t)0);    
}

void PlaySoundAtVolume(const uint8_t sound,const uint8_t newVolume,const uint8_t oldVolume) {

  struct SoundAction sa;
  sa.Type = PLAY_TRACK_WITH_VOLUME;
  sa.Track = sound;
  sa.Volume = newVolume;
  sa.RestoreVolume = oldVolume;
  xQueueSend(soundQueue,(void *)&sa,(TickType_t)0);     
}

char *GetSoundNameByIndex(uint8_t index) {

  char *retVal = NULL;

  if (index < MAX_SOUND_INDEX)
    retVal = soundNames[index];
    
  return retVal;
}

int GetSoundsAsXML(void textHandler(char *s)) {
  
  textHandler("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  textHandler("<Sounds xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"> ");
  
  const int xmlBufferSize = 1 * 1024;
  
  static char xmlBuffer[xmlBufferSize];
  char t2[100];

  // For all the sounds we have
  
  for(uint8_t i = 1; i < MAX_SOUND_INDEX; i++) {

    // Empty the buffer

    xmlBuffer[0] = '\0';

    // Start off the sound
    
    strcat(xmlBuffer,"<Sound ");

    // Add the sound index

    sprintf(t2,"%d",i);
    addTagNameValue(xmlBuffer,"Index",t2);
       
    // Add the sound name
      
    addTagNameValue(xmlBuffer,"Name",GetSoundNameByIndex(i));

    // Finish off the sound
    
    strcat(xmlBuffer,"/>");

    // Pass on the XML

    textHandler(xmlBuffer); 
  }

  // Finish off the XML
  
  textHandler("</Sounds>");
}
