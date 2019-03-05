/*
 *  Friends.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
extern void BuildFriends();
extern void UpdateFriend(const uint8_t memorySlot, const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundSelection);
extern void DumpFriends();
extern int GetFriendsAsXML(void textHandler(char *s));
extern int GetFriendsForEditAsXML(void textHandler(char *s));
extern int GetFriendsForBackup(void textHandler(char *s));
extern struct Friend *ProcessCall(char *callsign, int _speed, char *speedUnits, float frequency, char *_mode, char *spot);
extern void SaveFriends();
extern void LoadFriends();
extern void SaveDirtyFriends();
extern void SortFriends();
extern bool AddNewFriend(const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundSelection);
extern bool RemoveFriend(const char *callsign);
extern void RemoveAllFriends(bool setDirty);
extern void CommitAllFriends();
extern int FriendsSeenCount();
extern struct Friend *GetFriendBySeenIndex(int requiredIndex);
extern struct FriendExt *GetFriendExt(struct Friend *_friend);
//extern char *GetFriendsForEditAsXML();

extern void RestoreFriend(const uint8_t memorySlot, const char *callsign, const char *_name, const char *comment, const bool soundEnabled, const int soundIndex, const char *lastSeenAs, const time_t lastSeen, const int lastSpeed, const char *lastSpeedUnits, const float lastFrequency, const char *lastMode);
