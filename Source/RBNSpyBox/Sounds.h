/*
 *  Sounds.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
// The following manifests define all the available sounds.

#define SOUND_APEX 1
#define SOUND_AURORA 2
#define SOUND_BAMBOO 3
#define SOUND_BEACON 4
#define SOUND_BULLETIN 5
#define SOUND_BY_THE_SEASIDE 6
#define SOUND_CHIMES 7
#define SOUND_CHORD 8
#define SOUND_CIRCLES 9
#define SOUND_CIRCUIT 10
#define SOUND_COMPLETE 11
#define SOUND_CONSTELLATION 12
#define SOUND_COSMIC 13
#define SOUND_CRYSTALS 14
#define SOUND_DAMBUSTERS 15
#define SOUND_DAMBUSTERS_WONDERING 16
#define SOUND_HELLO 17
#define SOUND_HILLSIDE 18
#define SOUND_ILLUMINATE 19
#define SOUND_INPUT 20
#define SOUND_INTEL_INSIDE 21
#define SOUND_JAMES_BOND 22
#define SOUND_KEYS 23
#define SOUND_MARIMBA 24
#define SOUND_NIGHT_OWL 25
#define SOUND_NOKIA_STANDARD 26
#define SOUND_NOTE 27
#define SOUND_OPENING 28
#define SOUND_ORIGINAL_NOKIA_TUNE 29
#define SOUND_PLAYTIME 30
#define SOUND_POPCORN 31
#define SOUND_PRESTO 32
#define SOUND_PULSE 33
#define SOUND_RADAR 34
#define SOUND_RADIATE 35
#define SOUND_REFLECTION 36
#define SOUND_RIPPLES 37
#define SOUND_SENCHA 38
#define SOUND_SIGNAL 39
#define SOUND_SILK 40
#define SOUND_SLOW_RISE 41
#define SOUND_STARGAZE 42
#define SOUND_SUMMIT 43
#define SOUND_SYNTH 44
#define SOUND_TWINKLE 45
#define SOUND_UPLIFT 46
#define SOUND_WAVES 47

#define SOUND_DEFAULT_SEEN SOUND_NOTE

typedef enum {
  INITIALIZE,
  SET_VOLUME,
  PLAY_TRACK,
  PLAY_TRACK_WITH_VOLUME
} SoundActionType;

// The configuration structure holds the basic configuration data.

struct SoundAction
{
  SoundActionType Type;
  uint8_t Track;
  uint8_t Volume;
  uint8_t RestoreVolume;
};

extern void InitializeSound();
extern void SetSoundVolume(const uint8_t volume);
extern void PlaySound(const uint8_t sound);
extern void PlaySoundAtVolume(const uint8_t sound,const uint8_t newVolume,const uint8_t oldVolume);

extern int GetSoundsAsXML(void textHandler(char *s));
extern char *GetSoundNameByIndex(uint8_t index);
