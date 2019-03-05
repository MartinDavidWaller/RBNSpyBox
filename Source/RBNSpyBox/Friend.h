/*
 *  Friend.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#define FRIEND_CALLSIGN_MAX_LENGTH 20
#define FRIEND_LAST_SEEN_AS_MAX_LENGTH 20
#define FRIEND_COMMENT_MAX_LENGTH 40
#define FRIEND_NAME_MAX_LENGTH 30
#define FRIEND_LAST_MODE_MAX_LENGTH 8
#define FRIEND_LAST_SPEED_UNITS_MAX_LENGTH 8

#define IS_FRIEND_FREE(f) ((f->Flags & FRIENDFLAG_FREE) == FRIENDFLAG_FREE)
#define CLEAR_FRIEND_FREE(f) (f->Flags = (FriendFlags)((int)f->Flags & ~FRIENDFLAG_FREE))
#define SET_FRIEND_FREE(f) (f->Flags = (FriendFlags)((int)f->Flags | FRIENDFLAG_FREE))

#define IS_FRIEND_DIRTY(f) ((f->Flags & FRIENDFLAG_DIRTY) == FRIENDFLAG_DIRTY)
#define CLEAR_FRIEND_DIRTY(f) (f->Flags = (FriendFlags)((int)f->Flags & ~FRIENDFLAG_DIRTY))
#define SET_FRIEND_DIRTY(f) (f->Flags = (FriendFlags)((int)f->Flags | FRIENDFLAG_DIRTY))

#define IS_FRIEND_SOUND_ENABLED(f) ((f->Flags & FRIENDFLAG_SOUND) == FRIENDFLAG_SOUND)
#define CLEAR_FRIEND_SOUND_ENABLED(f) (f->Flags = (FriendFlags)((int)f->Flags & ~FRIENDFLAG_SOUND))
#define SET_FRIEND_SOUND_ENABLED(f) (f->Flags = (FriendFlags)((int)f->Flags | FRIENDFLAG_SOUND))

typedef enum {
  FRIENDFLAG_CLEAR = 0,           // No flags set
  FRIENDFLAG_FREE = 1 << 0,       // Friend is free
  FRIENDFLAG_DIRTY = 1 << 1,      // Friend is dirty
  FRIENDFLAG_SOUND = 1 << 2       // Sound is enabled
} FriendFlags;

struct Friend
{
  time_t LastEmailSent;
  time_t LastSeen;
  int LastSpeed;
  float LastFrequency;
  char Callsign[FRIEND_CALLSIGN_MAX_LENGTH + 1];
  char LastSeenAs[FRIEND_LAST_SEEN_AS_MAX_LENGTH + 1];
  char Comment[FRIEND_COMMENT_MAX_LENGTH + 1];
  char Name[FRIEND_NAME_MAX_LENGTH + 1];
  char LastMode[FRIEND_LAST_MODE_MAX_LENGTH + 1];
  char LastSpeedUnits[FRIEND_LAST_SPEED_UNITS_MAX_LENGTH + 1];
  FriendFlags Flags;
  uint8_t Sound;
  uint8_t MemorySlot;
};

struct FriendExt
{
  time_t LastAlert;
};

void DumpFriend(struct Friend *_friend);
