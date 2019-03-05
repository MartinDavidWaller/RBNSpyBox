/*
 *  StringHelper.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#define MAX_TOKEN_COUNT 14

extern char *tokenPointers[];
extern int SplitString(char* stringToSplit);
extern int SplitStringOnSpace(const char *stringToSplit, char **partsBuffer[]);
extern int wildcmp(const char *wild, const char *string);
extern char *StringCopyWithLimit(char *dest, const char *source, int limit);
