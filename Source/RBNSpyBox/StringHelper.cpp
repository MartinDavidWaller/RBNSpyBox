/*
 *  StringHelper.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include "Arduino.h"
#include "StringHelper.h"

char *tokenPointers[MAX_TOKEN_COUNT];

int SplitString(char* stringToSplit)
{
  char seperators[] = " ";
  char *token = NULL;
  char *nextToken = NULL;
  int tokenCount = 0;

  // Pull out the first token

  token = strtok_r(stringToSplit, seperators, &nextToken);

  // While we have tokens to process

  while ((token != NULL) && (tokenCount < MAX_TOKEN_COUNT))
  {
    // Save the away the token pointer

    tokenPointers[tokenCount++] = token;

    // Move to the next token

    token = strtok_r(NULL, seperators, &nextToken);
  }

  // Return the token count

  return tokenCount;
}

int SplitStringOnSpace(const char *stringToSplit, char **partsBuffer[])
{
  const int MAX_PARTS = 20;
  const int MAX_PART_LENGTH = 20;

  static char _00[MAX_PART_LENGTH + 1];
  static char _01[MAX_PART_LENGTH + 1];;
  static char _02[MAX_PART_LENGTH + 1];
  static char _03[MAX_PART_LENGTH + 1];
  static char _04[MAX_PART_LENGTH + 1];
  static char _05[MAX_PART_LENGTH + 1];
  static char _06[MAX_PART_LENGTH + 1];
  static char _07[MAX_PART_LENGTH + 1];
  static char _08[MAX_PART_LENGTH + 1];
  static char _09[MAX_PART_LENGTH + 1];
  static char _10[MAX_PART_LENGTH + 1];
  static char _11[MAX_PART_LENGTH + 1];
  static char _12[MAX_PART_LENGTH + 1];
  static char _13[MAX_PART_LENGTH + 1];
  static char _14[MAX_PART_LENGTH + 1];
  static char _15[MAX_PART_LENGTH + 1];
  static char _16[MAX_PART_LENGTH + 1];
  static char _17[MAX_PART_LENGTH + 1];
  static char _18[MAX_PART_LENGTH + 1];
  static char _19[MAX_PART_LENGTH + 1];

  static char *parts[] =
  {
    _00,_01,_02,_03,_04,_05,_06,_07,_08,_09,
    _10,_11,_12,_13,_14,_15,_16,_17,_18,_19
  };

  *partsBuffer = parts;

  int retVal = 0;

  if (NULL != stringToSplit) {

    uint8_t ci = 0;
    char *p = (char *)stringToSplit;
    while ((*p != '\0') && (retVal < MAX_PARTS))
    {
      if (*p == ' ') {

        // This is a space. Zero the current output buffer and move to the next
        // output buffer.

        parts[retVal][ci++] = '\0';
        retVal++;

        // Clear down the buffer index

        ci = 0;

        // Soak up all spaces

        while ((*p != '\0') && (*p == ' '))
          p++;

        // Back off by one

        p--;
      }
      else {

        // This is now space so we can simply copy it to the output buffer

        if (ci < MAX_PART_LENGTH) {
          parts[retVal][ci++] = *p;
          parts[retVal][ci] = '\0';
        }
      }

      // Move to the next character.

      p++;
    }
  }

  if (0 != retVal)
    retVal++;

  return retVal;
}

int wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string + 1;
    }
    else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    }
    else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

char *StringCopyWithLimit(char *dest, const char *source, int limit) {

  unsigned i;

  // Copy over all the characters ending when we see a null or
  // we hit the limit.
  
  for (i = 0; source[i] != '\0' && i < limit; ++i)
    dest[i] = source[i];

  // Ensure trailing null byte is copied
  dest[i]= '\0';

  // Return the detination pointer
  
  return dest;
}
