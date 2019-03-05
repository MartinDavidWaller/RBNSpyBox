/*
 *  DFPlayer.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */

/*
 * This module defines manifets and methods applicable to the
 * DFPlayer device.
 */
 
#define DFPLAYER_BAUD_RATE 9600
#define DFPLAYER_DEFAULT_VOLUME 0x15
#define DFPLAYER_MINIMUM_VOLUME 0x00
#define DFPLAYER_MAXIMUM_VOLUME 0x1e

#define DFPLAYER_COMMAND_SET_VOLUME 0x06
#define DFPLAYER_COMMAND_PLAY 0x0f
#define DFPLAYER_COMMAND_INITIALIZE 0x3f

#define DFPLAYER_COMMAND_BUFFER_STARTBYTE 0
#define DFPLAYER_COMMAND_BUFFER_VERSION 1
#define DFPLAYER_COMMAND_BUFFER_LENGTH 2
#define DFPLAYER_COMMAND_BUFFER_COMMAND 3
#define DFPLAYER_COMMAND_BUFFER_REQUEST_ACK 4
#define DFPLAYER_COMMAND_BUFFER_HIBYTE_ARGUMENT 5
#define DFPLAYER_COMMAND_BUFFER_LOWBYTE_ARGUMENT 6
#define DFPLAYER_COMMAND_BUFFER_HIBYTE_CHECKSUM 7
#define DFPLAYER_COMMAND_BUFFER_LOWBYTE_CHECKSUM 8
#define DFPLAYER_COMMAND_BUFFER_ENDCODE 9
#define DFPLAYER_COMMAND_BUFFER_SIZE 10

// Defined methods

extern void DFPlayerInitialise(void writeHandler(uint8_t b)) ;
extern void DFPlayerPlay(void writeHandler(uint8_t b), int mp3ToPlay, bool withWait);
extern void DFPlayerSetVolume(void writeHandler(uint8_t b),uint8_t volume);
