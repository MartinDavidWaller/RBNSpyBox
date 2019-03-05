/*
 *  RBNSpyBox.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
// The following manifests provide the product information

#define PROGRAM_NAME "RBNSpyBox"
#define PROGRAM_VERSION_MAJOR 0
#define PROGRAM_VERSION_MINOR 9
#define BETA_TEXT ""
#define COPYRIGHT_TEXT "(C) M.D.Waller G0PJO"

#define REVERSE_BEACON_NETWORK_ADDRESS "telnet.reversebeacon.net"
#define REVERSE_BEACON_NETWORK_PORT 7000

#define DEFAULT_TIMEZONE "GMT0BST,M3.5.0/01,M10.5.0/02"

#define EEPROM_ADDRESS 0x50

#define MAX_FRIEND_COUNT 50

#define DEBUG 0
#define DEBUG_ECHO_DISPLAY 0
#define DEBUG_SOUND_QUEUE 0

#define DEFAULT_VOLUME 8
#define DEFAULT_MINUTES_BETWEEN_ALERTS 5
#define DEFAULT_FRIEND_CYCLE_COUNT 5

#define MAX_SECONDS_BEFORE_RESTART 60

#define LED_PIN 14
#define BOOT_PIN 0
#define DFPLAYER_BUSY_PIN 34

// The following manifests define the hardware pins used by the
// DFPlayer module.

//define DFPlayerRXPin 14 
//define DFPlayerTXPin 12

struct RuntimeData
{
  time_t LastSpotSeen;
  long SpotsSeen;
  time_t BootTime;
  long RBNConnectionsMade;
};

// Defined methods / data

extern struct RuntimeData runtimeData;
extern bool doSaveDirtyFriends;
//extern bool doListenWithVolume;
//extern int doListenWithVolumeVolume;
//extern void playSound(const int soundToPlay);
//extern void setNewVolume();
