/*
 *  Configuration.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */

#define EEPROM_SIZE 512

#define HOSTNAME_SIZE 15
#define WIFI_SSID_SIZE 32
#define WIFI_PASSWORD_SIZE 64
#define CALLSIGN_SIZE 32
#define TIMEZONE_SIZE 30

#define IS_EEPROM_VALID(c) ((c->Flags & EEPROM_VALID) == EEPROM_VALID)
#define CLEAR_EEPROM_VALID(c) (c->Flags = (ConfigurationFlags)((int)c->Flags & ~EEPROM_VALID))
#define SET_EEPROM_VALID(c) (c->Flags = (ConfigurationFlags)((int)c->Flags | EEPROM_VALID))

#define IS_SOUND_ENABLED(c) ((c->Flags & SOUND_ENABLED) == SOUND_ENABLED)
#define CLEAR_SOUND_ENABLED(c) (c->Flags = (ConfigurationFlags)((int)c->Flags & ~SOUND_ENABLED))
#define SET_SOUND_ENABLED(c) (c->Flags = (ConfigurationFlags)((int)c->Flags | SOUND_ENABLED))

#define IS_LED_ENABLED(c) ((c->Flags & LED_ENABLED) == LED_ENABLED)
#define CLEAR_LED_ENABLED(c) (c->Flags = (ConfigurationFlags)((int)c->Flags & ~LED_ENABLED))
#define SET_LED_ENABLED(c) (c->Flags = (ConfigurationFlags)((int)c->Flags | LED_ENABLED))

typedef enum {
  CLEAR = 0,                    // No flags set
  EEPROM_VALID = 1 << 0,        // EEPROM contains valid friends.
  SOUND_ENABLED = 1 << 1,       // Sound enabled.
  LED_ENABLED = 1 << 2          // LED enabled.
} ConfigurationFlags;

// The configuration structure holds the basic configuration data.

struct Configuration
{
  uint8_t MajorVersion;
  uint8_t MinorVersion;
  ConfigurationFlags Flags;
  char Hostname[HOSTNAME_SIZE + 1];
  char WiFi_SSID[WIFI_SSID_SIZE + 1];
  char WiFi_Password[WIFI_PASSWORD_SIZE + 1];
  char Callsign[CALLSIGN_SIZE + 1];
  char TimeZone[TIMEZONE_SIZE + 1];
  uint8_t Volume;
  uint8_t MinutesBetweenAlerts;
  uint8_t FriendCycleCount;
};

// The EPPROM_DATA structure wraps the configuration structure and adds a checksum
// to it. The checkum will be validated on reads and updated on writes.

struct EEPROM_DATA
{
  struct Configuration Configuration;
  uint16_t Size;
  uint16_t CheckSum;
};

// The following methods are used to manipulate the configuration information

bool readConfiguration(struct Configuration *configuration);
void writeConfiguration(struct Configuration *configuration);
void dumpConfiguration(struct Configuration *configuration);
