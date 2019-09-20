/* ====================================================================
 * Copyright (c) 2019 Martin D. Waller - G0PJO.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by Martin D. Waller - G0PJO"
 *
 * 4. The names "RBNSpyBox" must not be used to endorse or promote 
 *    products derived from this software without
 *    prior written permission.
 *
 * 5. Products derived from this software may not be called "RBNSpyBox"
 *    nor may "RBNSpyBox" appear in their names without prior written
 *    permission of Martin D. Waller.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by Martin D. Waller - G0PJO"
 *
 * THIS SOFTWARE IS PROVIDED BY Martin D. Waller ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 */

//    Modified:   March 3rd, 2019
//                a) V0.8 delivered to Chris G3XVL.
//
//    Modified:   March 5th, 2019
//                a) Slight change to the Copyright notice as served
//                   by the WebServer. No version change.
//                b) Added a help page to the Web pages. No version
//                   change.
//                c) Reduced the demonstration friend list. Version
//                   changed to V0.9.
//
//    Modified:   March 8th, 2019
//                a) Improved the copyright text.
//                b) Added numbers to the friends page so you can see
//                   how many you have.
//
//    Modified:   March 11th, 2019
//                a) Moved the code that sets to hostname to just after the
//                   call to WiFi.begin. This seems to work better than doing
//                   it later.
//                b) Added MaxFriends to the FriendsForEdit XML.
//                c) Modified the Friends page to remove the Add New Friend 
//                   ability if you already have the maximum.
//                d) Added triple characters to the spot count.
//
//    Modified:   March 16th, 2019
//                a) Added a call to WiFi.setSleep(false) to see if it helps
//                   Chris.
//
//    Modified:   September 20th, 2019
//  		  a) With help from Mark, G6WRB, we've added code to turn the 
//                   backlight off between certain hours. This is fixed at compile
//                   to and not configurable in the web interface - yet!
//

// ToDo
//
//  - Add CW alert.
//  - Add email alerts.
//  - Add confirmation at the end of a Friend restore operation.
//  - After the reboot botton is pressed direct to the home page and not the friend page.
//  - Add ability to save default sound.
//  - Make use of schedule_function to flush changes to EEPROM etc.

// Related Web Links:
//
//  https://lastminuteengineers.com/esp32-arduino-ide-tutorial/
//  https://www.hackster.io/rayburne/esp32-in-love-with-both-cores-8dd948
//  https://www.freertos.org/FreeRTOS_Support_Forum_Archive/February_2015/freertos_Protecting_variable_types_using_mutexes_question_de18e96cj.html
//

#include "Arduino.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"

#include "RBNSpyBox.h"
#include "Configuration.h"
#include "Dump.h"
#include "FormatHelper.h"
#include "Friend.h"
#include "Friends.h"
#include "LCD20x4.h"
#include "Sounds.h"
#include "StringHelper.h"
#include "WebServer.h"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <time.h>                   

// The following manifests determine which row the various data parts
// are displayed

#define TITLE_ROW 1
#define BETA_ROW 2
#define COPYRIGHT_ROW 3

#define DELAY_SPLASH 4

// The following manifests are used to control the active state

typedef enum {
  
  STATE_BUILDING_ACCESS_POINT,
  STATE_CONNECTING_TO_WIFI,
  STATE_CONNECTING_TO_REVERSE_BEACON_NETWORK,
  STATE_RECIEVING_RBN_DATA
  
} RUNNING_STATE;

typedef enum {

  DISPLAY_FRIEND_ALERT,
  DISPLAY_DEVICE_INFO,
  DISPLAY_RBN_INFO,
  DISPLAY_FRIEND_SPOTS,
  DISPLAY_NO_FRIENDS_SPOTTED
  
} RBN_DISPLAY_STATE;

// The following manifests define the data order in the line recieved from
// the Reverse Beacon Network. A typical RBN spot line looks like:
//
// DX de K3LR-#:    14042.0  GB2RN          CW    12 dB  18 WPM  CQ      1350Z

#define RBN_FREQUENCY 3
#define RBN_CALLSIGN 4
#define RBN_MODE 5
#define RBN_SPEED 8
#define RBN_SPEED_UNITS 9

struct Configuration configuration;               // Configuration object
RUNNING_STATE currentState;                       // Current running state
RBN_DISPLAY_STATE currentRBNState;                // Current display state
int rbnStateCountDown;                            // RBN state count down
int friendOnDisplayIndex = 0;                     // Index of the friend currently displayed
LiquidCrystal_I2C lcd(0x27, 20, 4);               // LCD interface object
WiFiClient rbnClient;                             // Telnet client
bool doSaveDirtyFriends = false;                  // Save dirty flag
time_t lastSecond;                                // Last second time object
char rbnLineBuffer[2048 + 1];                     // Line buffer for the RBN data
struct RuntimeData runtimeData;                   // Runtime data structure

boolean startAP() {

  #define START_AP_ROW_0 0
  #define START_AP_ROW_2 2
  #define START_AP_DELAY 1000
  
  // Start the Access Point, keep the user informed.
  
  clearDisplay();

  // Put in the instructions

  displayCentredTextInRow("Starting AP:",START_AP_ROW_0);
  
  char line[20 + 1];  
  sprintf(line,"%s V%d.%d",PROGRAM_NAME,(int)configuration.MajorVersion,(int)configuration.MinorVersion);
  
  displayCentredTextInRow(line,START_AP_ROW_2);

  // Delay so the user can see the splash

  delay(START_AP_DELAY);

  // Start by kicking off the soft-AP
  
  boolean result = WiFi.softAP(PROGRAM_NAME);
  if (false == result) {

    // This has failed, tell the user
    
    displayError("Failed to open AP:", NULL, PROGRAM_NAME);
  }

  // Return the result
  
  return result;
}

boolean connectWiFi() {

  #define CONNECT_WIF_ROW_0 0
  #define CONNECT_WIF_ROW_2 2
  #define CONNECT_WIF_DELAY 1000
  
  // Connect to the WiFi, keep the user informed.
  
  clearDisplay();

  displayCentredTextInRow("Connecting to:",CONNECT_WIF_ROW_0);
  displayCentredTextInRow((char *)&configuration.WiFi_SSID[0],CONNECT_WIF_ROW_2);

  delay(CONNECT_WIF_DELAY);

  // Disconnect anything that we may have

  WiFi.disconnect();

  // Begin the connection
  
  WiFi.begin((char *)&configuration.WiFi_SSID[0],(char *)&configuration.WiFi_Password[0]);

  // Disable sleep
  
  WiFi.setSleep(false);

  // Apply any hostname that we may have
  
  if (strlen((char *)&configuration.Hostname[0]) > 0)
    WiFi.setHostname((char *)&configuration.Hostname[0]);
  else
    WiFi.setHostname(PROGRAM_NAME);    
    
  // Wait for the connection to be made.

  int maxTry = 10;
  while ((WiFi.status() != WL_CONNECTED) && (maxTry > 0)) {

    // Wait and update the try count.

    delay(1000);
    maxTry--;

    if (maxTry <=0) {
      displayError("Connecting to",(char *)&configuration.WiFi_SSID[0],"failed!");
      return false;
    }
  }

  // We are connected, display the IP address
 
  IPAddress ipAddress = WiFi.localIP();

  clearDisplay();

  displayCentredTextInRow("IP Address:",0);

  displayCentredTextInRow(FormatIPAddress(ipAddress),2);

  // Delay a short while
  
  delay(2 * 1000);

 // Kick off a WiFi Scan, we may need the SSID list later.

  WiFi.scanNetworks(true);

 return true;
}

void setup() {

  // Initialise the Runtime Data

  runtimeData.LastSpotSeen = 0;
  runtimeData.SpotsSeen = 0;
  runtimeData.BootTime;
  runtimeData.RBNConnectionsMade = 0;

  pinMode(BOOT_PIN,INPUT);
  pinMode(LED_PIN,OUTPUT);
  
  // Initialise the Wire!

  Wire.begin();
  
  // Initialise the LCD display
  
  lcd.begin();
  lcd.backlight();
  
  // Set up the serial port so we can generate debug etc.
   
  Serial.begin(115200);\
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 

  // We are going to need access to the files stored so start up SPIFFS
  // and list out what we have.

  Serial.println("Contents of SPIFFS:");
  Serial.println("");

  if(false == SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  
  // Open the root object
  
  File root = SPIFFS.open("/");

  // Loop through the files
  
  File file = root.openNextFile();
  while (file) {
    if (true == file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());    
    } else {
      Serial.print("  FILE: ");
      Serial.println(file.name());
      Serial.println(file.size());
    }

    // Move to the next file
    
    file = root.openNextFile();
  }

  // Read the configuration data from the EEPROM

  Serial.println("");
  Serial.println("Read the Configuration Data");
  bool checkSumOK = readConfiguration(&configuration);

  Serial.println("");
  hexDump((char *)&configuration,sizeof(Configuration));

  if (true == checkSumOK)
    Serial.println("... CheckSum OK");
  else
    Serial.println("... CheckSum NOT OK");

  // If the checksum is not OK or if the version numbers
  // do not match then we must start again!

  if ((false == checkSumOK) || 
      (configuration.MajorVersion != PROGRAM_VERSION_MAJOR) || 
      (configuration.MinorVersion != PROGRAM_VERSION_MINOR) ||
      (0 == digitalRead(BOOT_PIN))) {

    if (true == checkSumOK) {
      Serial.println("");
      Serial.println("Different verison numbers.");
      Serial.print("...Found V");
      Serial.print(configuration.MajorVersion);
      Serial.print(".");
      Serial.println(configuration.MinorVersion);
      Serial.print("...Expected V");
      Serial.print(PROGRAM_VERSION_MAJOR);
      Serial.print(".");
      Serial.println(PROGRAM_VERSION_MINOR);    
      Serial.println("");        
    }
      
    // Initialise the configuration data

    memset(&configuration,0,sizeof(Configuration));
    configuration.MajorVersion = PROGRAM_VERSION_MAJOR;
    configuration.MinorVersion = PROGRAM_VERSION_MINOR;
    configuration.Flags = CLEAR;
    SET_LED_ENABLED((&configuration));
    SET_SOUND_ENABLED((&configuration));
    strcpy((char*)&configuration.Hostname[0],PROGRAM_NAME);
    strcpy((char*)&configuration.TimeZone[0],DEFAULT_TIMEZONE);
    configuration.Volume = DEFAULT_VOLUME;
    configuration.MinutesBetweenAlerts = DEFAULT_MINUTES_BETWEEN_ALERTS;
    configuration.FriendCycleCount = DEFAULT_FRIEND_CYCLE_COUNT;

    Serial.println("Writing new configuration");
    Serial.println("");
    hexDump((char *)&configuration,sizeof(Configuration));

    // Write it out
    
   writeConfiguration(&configuration); 
  }

  // Dump the current configuration
  
  dumpConfiguration(&configuration);

  // Clear the screen and display the splash

  clearDisplay();

  char line[20 + 1];
  sprintf(line,"%s V%d.%d",PROGRAM_NAME,(int)configuration.MajorVersion,(int)configuration.MinorVersion);
  displayCentredTextInRow(line,TITLE_ROW);
  displayCentredTextInRow(BETA_TEXT,BETA_ROW);
  displayCentredTextInRow(COPYRIGHT_TEXT,COPYRIGHT_ROW);

  // Delay so the user can see the splash

  delay(DELAY_SPLASH * 1000);
  
  // Set up the MP3 player

  InitializeSound();
  SetSoundVolume(configuration.Volume);

  // If we are running with sound then make one!
  
  if (true == IS_SOUND_ENABLED((&configuration))) {

    // Play a sound
    
    PlaySound(SOUND_INTEL_INSIDE);
  }
  
  // We need to decide if we have friend data or not. If we do then
  // we can read the friends from the EEPROM or we can build an empty
  // set.

  clearDisplay();
  clearDisplayLineBuffer();
  
  if (true == IS_EEPROM_VALID((&configuration))) {

    addCentreString("Loading friends...");
    writeDisplayLineBuffer(0);

    LoadFriends();
  }
  else {

    // We have no valid friends so we will build an empty set.

    addCentreString("Building friends...");
    writeDisplayLineBuffer(0);  
    
    BuildFriends();

    SaveFriends();

    SET_EEPROM_VALID((&configuration));
    
    // Write it out
    
    writeConfiguration(&configuration); 
  }

  SortFriends();
  DumpFriends();

  startAP();
  buildServer();

  configTime(0, 0, "pool.ntp.org");
  setenv("TZ", (char*)&configuration.TimeZone[0],1);
  time(&lastSecond);
  time(&runtimeData.BootTime);

  // Set the current state

  currentState = STATE_BUILDING_ACCESS_POINT;
  currentState = STATE_CONNECTING_TO_WIFI;
}

void displayFriendAlert(struct Friend *_friend) {
                      
  // Clear the display
  
  clearDisplay();

  // Display the data

  displayCentredTextInRow("Alert!",0);
  displayCentredTextInRow((char *)&_friend->LastSeenAs[0],1);
  displayCentredTextInRow(FormatFrequency(_friend->LastFrequency),2);

  char line[20 + 1];
  sprintf(line,"%d %s / %s",_friend->LastSpeed,(char *)&_friend->LastSpeedUnits[0],(char *)&_friend->LastMode[0]);
  displayCentredTextInRow(line,3);  
  
  // Set the update interval

  rbnStateCountDown = 5;                 
}

/*
 * This method is called to display friend details. Initially it
 * gets past the index of the friend in the display order 1, 2, 3, 4, etc
 * and a pointer to the frind object. Follow on calls can be made with
 * arguments 0 and NULL. It will swap the name for the comment and the
 * frequency for the mode and speed as further calls are made.
 */
void displayFriendSpot(int index, Friend *_friend) {

  static struct Friend *lastFriend = NULL;
  static bool oddRow;

  // Build a line buffer
  
  char line[20 + 1];

  // Is this a new display or an update of what we already have?

  if (NULL != _friend) {
  
    // Clear the display
  
    clearDisplay();

    // Set the odd row flag and save the friend

    oddRow = true;
    lastFriend = _friend;

    // Display the callsign that we processed
    
    sprintf(line,"%d. %s",index,(char*)&_friend->LastSeenAs[0]);
    displayCentredTextInRow(line,0);

    // Display the last seen date / time
    
    displayCentredTextInRow(FormatTimeAsDateTime(_friend->LastSeen),3);
  
    // Set the update interval

    rbnStateCountDown = 3;    
  }

  // Finally fill in line 2!

  if (true == oddRow) {

    displayCentredTextInRow((char*)&lastFriend->Name[0],1);
    sprintf(line,"%s",FormatFrequency(lastFriend->LastFrequency));
  }
  else {
    if (0 != strlen((char*)&lastFriend->Comment[0]))
      displayCentredTextInRow((char*)&lastFriend->Comment[0],1);
    else
      displayCentredTextInRow((char*)&lastFriend->Name[0],1);
    sprintf(line,"%s / %d %s",(char*)&lastFriend->LastMode[0],lastFriend->LastSpeed,(char*)&lastFriend->LastSpeedUnits[0]);
  }
  displayCentredTextInRow(line,2);
  
  oddRow = !oddRow;
}

void displayNoFriendsSpotted() {

  // Clear the display
  
  clearDisplay();

  // Display the text
  
  displayCentredTextInRow("No friends spotted!",1);
}

void displayDeviceInfo() {

  // Clear the display
  
  clearDisplay();

  // Line buffer
  
  char line[20 + 1];
  
  // Put in the name and version

  clearDisplayLineBuffer();
  sprintf(line,"%s V%d.%d",PROGRAM_NAME,PROGRAM_VERSION_MAJOR,PROGRAM_VERSION_MINOR);
  displayCentredTextInRow(line,0);

  // Put in the uptime.
  
  clearDisplayLineBuffer();
  sprintf(line,"Up: %s",FormatUptime(&runtimeData.BootTime));
  displayCentredTextInRow(line,2);

  // Put in the IP address
  
  clearDisplayLineBuffer();
  sprintf(line,"IP: %s",FormatIPAddress(WiFi.localIP()));
  displayCentredTextInRow(line,3);
        
  // Set the update interval

  rbnStateCountDown = 2; 
}

void displayRBNInfo() {

  // Clear the display
  
  clearDisplay();

  // Put in the spot count

  clearDisplayLineBuffer();
  displayCentredTextInRow("Spots Seen:",0);

  // Create a line buffer

  char line[20 + 1];

  // Put in the no of spots seen
  
  clearDisplayLineBuffer();
  displayCentredTextInRow(FormatWithTriple(runtimeData.SpotsSeen,line),1);

  // Put in the last spot seen at

  clearDisplayLineBuffer();
  displayCentredTextInRow("Last At:",2);

  // Put in the last spot time
  
  displayCentredTextInRow(FormatTimeAsDateTime(runtimeData.LastSpotSeen),3);
        
  // Set the update interval

  rbnStateCountDown = 2; 
}

void loop() {
  
  // What we do here depend on what state we are in.

  switch(currentState) {

    case STATE_BUILDING_ACCESS_POINT:

      if (true == startAP()) {
        currentState = STATE_CONNECTING_TO_WIFI;
      }
      else {
        
      }
      
      break;
      
    case STATE_CONNECTING_TO_WIFI:

      // We are about to connect to the WiFi. We need to ensure that we
      // have the necessary data to try.

      if (0 == strlen((char *)&configuration.WiFi_SSID[0])) {

        // We need to prompt the user to enter the necessary details
        // to connect to the WiFi
      
        clearDisplay();
        
        clearDisplayLineBuffer();
        addStringToDisplayLineBuffer("Connect to:");
        writeDisplayLineBuffer(0);

        clearDisplayLineBuffer();
        addCentreString(PROGRAM_NAME);
        writeDisplayLineBuffer(1);

        clearDisplayLineBuffer();
        addStringToDisplayLineBuffer("and browse to:");
        writeDisplayLineBuffer(2);
    
        clearDisplayLineBuffer();
        IPAddress ipAddress = WiFi.softAPIP();
        addStringToDisplayLineBuffer("http://");
        addStringToDisplayLineBuffer(ipAddress.toString().c_str());
        writeDisplayLineBuffer(3);

        delay(5000);
      }
      else {

        // We have a SSID, etc try to make the connection
        
        if (true == connectWiFi()) {

          // We are connected!
          
          currentState = STATE_CONNECTING_TO_REVERSE_BEACON_NETWORK;
        }
        else {
        
        }
      }
      
      break;

    case STATE_CONNECTING_TO_REVERSE_BEACON_NETWORK:

      // Make sure that we are connected to the WiFi

      if (WiFi.status() != WL_CONNECTED)
      {
        // Revert to connecting to the WiFi
        
        currentState = STATE_CONNECTING_TO_WIFI;
      }
      else
      {
        // Tell the user we are connetcing to the Reverse Beacon Network
      
        clearDisplay();

        displayCentredTextInRow("Connecting to:",0);
        displayCentredTextInRow("Reverse Beacon",2);
        displayCentredTextInRow("Network",3);
        
        delay(1000);

        rbnClient.connect(REVERSE_BEACON_NETWORK_ADDRESS, REVERSE_BEACON_NETWORK_PORT);
        if(true == rbnClient.connected()) {

          // Update the current state to receiving RBN data
        
          currentState = STATE_RECIEVING_RBN_DATA;

          // Update the connection count

          runtimeData.RBNConnectionsMade++;

          // Set the RBN state to DISPLAY_DEVICE_INFO and update the display

          currentRBNState = DISPLAY_DEVICE_INFO;
          displayDeviceInfo();
        }
      }
        
      break;

    case STATE_RECIEVING_RBN_DATA:

      // Check to see if we are still connected

      if (true == rbnClient.connected()) {

        // Do we have data avaiable to read?
        
        if (rbnClient.available()) {

          // If the LED is anbaled then tuen it on
          
          if (true == IS_LED_ENABLED((&configuration)))
            digitalWrite(LED_PIN,HIGH);

          // Yes, clear the buffer and read until the end of line
          
          int bi = 0;
          memset(rbnLineBuffer,0,2048 + 1);
          while ((rbnClient.connected()) && (rbnClient.available()) && (bi < 2048)) {

            // Serial.printf("%s\n",rbnLineBuffer);
            
            // Read the character and save it away
            
            char c = rbnClient.read();
            rbnLineBuffer[bi++] = c;

            // If we are the end of line then break the loop
                    
            if (('\n' == c) || ('\r' == c))
              break;
          }

          //Serial.write(rbnLineBuffer);
          
          // Do we have something worth processing
          
          if (strlen(rbnLineBuffer) > 2) {
            
            // Is this the please enter request?

            if (NULL != strstr(rbnLineBuffer,"Please")) {

              // Send back the callsign
              
              rbnClient.println((char*)&configuration.Callsign[0]);
            }
            else {
              
              // We might have a real spot to process
              
              //Serial.write(rbnLineBuffer);

              // Split the string up

              char **parts;

              int n = SplitStringOnSpace(rbnLineBuffer, &parts);

              // Do we have the correct number of parts
              
              if ((n == 12) || (n == 13)) {

                // Update the callsign count
                
                runtimeData.SpotsSeen++;
                
                // Update the last spot seen date/time

                time(&runtimeData.LastSpotSeen);
                
                // Process the callsign
                
                struct Friend *_friend = ProcessCall(
                  parts[RBN_CALLSIGN],
                  atoi(parts[RBN_SPEED]),
                  parts[RBN_SPEED_UNITS],
                  atof(parts[RBN_FREQUENCY]) / 1000.0f,
                  parts[RBN_MODE],
                  rbnLineBuffer
                  );
                
                // Did we find a friend?
                
                if (NULL != _friend) {

                  // Yes, we have just seen a friend. We need to locate the FriendExt record
                  // and decide if we need to make a sound!

                  struct FriendExt *friendExt = GetFriendExt(_friend);

                  // We are going to need to the time to decide if we sound an alert
                  // and to update the FriendExt record

                  time_t now;
                  time(&now);

                  // Do we action this alert

                  if (now - friendExt->LastAlert > configuration.MinutesBetweenAlerts * 60) {

                    // Yes we action the alert.

                    currentRBNState = DISPLAY_FRIEND_ALERT;
                    displayFriendAlert(_friend);  
  
                    // Do we want to make a noise?

                    if ((true == IS_SOUND_ENABLED((&configuration))) &&
                        (true == IS_FRIEND_SOUND_ENABLED(_friend))) {

                      // Yes we do, we need to sound an alert.

                      PlaySound(_friend->Sound);                    
                    }

                    // Update the last alert time

                    friendExt->LastAlert = now;                    
                  }                    
                }
              }
            }
          }
          else {

            // ?
        
          }
        }

        digitalWrite(LED_PIN,LOW);
      }
      else {

        // We seem to have lost connection to the Reverse Beacon Network. Try
        // to connect again!
        
        currentState = STATE_CONNECTING_TO_REVERSE_BEACON_NETWORK;
      }
      
      break;
      
    default:
      //delay(100);
      break;  
  }

  // Do we need to save our dirty friends

  if (true == doSaveDirtyFriends) {

    // Save the dirty friends. This gets done one at a time and the
    // routine called clears the flag when all dirty friends are 
    // saved away.
    
    SaveDirtyFriends();
  }

  // Next we process the things we need to do every second, or multiple there of!

  time_t now;
  time(&now);
  
  if (now - lastSecond > 1) {

    // If we are in the RBN Running state we need to update the count down and see 
    // if we need to update the display

    if (STATE_RECIEVING_RBN_DATA == currentState) {

      // Yes, we are receiving RBN data. Reduce the state count down by one

      rbnStateCountDown--;

      if (rbnStateCountDown <= 0) {
        
        switch(currentRBNState) {
          
          case DISPLAY_FRIEND_ALERT:
  
            //// We are currently displaying a friend alert. We need to set
            //// the new state to DISPLAY_RUNTIME_INFO.

            ////currentRBNState = DISPLAY_DEVICE_INFO;  
            ////displayDeviceInfo(); 

            // Skip the runtime info and move to the first friend.
            
            currentRBNState = DISPLAY_RBN_INFO;
            break;

          case DISPLAY_DEVICE_INFO:

            // We are currently displaying DEVICE information. We will move on to
            // display RBN information.

            currentRBNState = DISPLAY_RBN_INFO;  
            displayRBNInfo();
            break;
                        
          case DISPLAY_RBN_INFO:

            // We are currently displaying RBN information. We will move on to
            // display friend spots if we have any.

            if (0 == FriendsSeenCount()) {

              // We have not seen any friends so we need to set the new
              // state to DISPLAY_NO_FRIENDS_SPOTTED.

              currentRBNState = DISPLAY_NO_FRIENDS_SPOTTED;  
              displayNoFriendsSpotted();
              rbnStateCountDown = 3;              
            }
            else {

              // We have some friends to display so we need to set the new
              // state to DISPLAY_FRIEND_SPOTS and display the first friend.

              currentRBNState = DISPLAY_FRIEND_SPOTS; 
              friendOnDisplayIndex = 0;
              struct Friend *f = GetFriendBySeenIndex(friendOnDisplayIndex);
              if (NULL != f) {

                displayFriendSpot(friendOnDisplayIndex + 1,f);
                rbnStateCountDown = 3;
              }     
            }
            break;
            
          case DISPLAY_FRIEND_SPOTS:

            // We are currently display friend spots. We need to increment the friend
            // on display and display it if we have it. If not cycle to the display the
            // runtime information.

            friendOnDisplayIndex++;
            if ((friendOnDisplayIndex < FriendsSeenCount()) && (friendOnDisplayIndex < configuration.FriendCycleCount)) { 

              // We have a valid friend to display

              struct Friend *f = GetFriendBySeenIndex(friendOnDisplayIndex);
              if (NULL != f) {

                displayFriendSpot(friendOnDisplayIndex + 1,f);
                rbnStateCountDown = 3;
              }               
            }
            else {

              // We have hit the end of the friends, set the new state to
              // DISPLAY_DEVICE_INFO.

              currentRBNState = DISPLAY_DEVICE_INFO;  
              displayDeviceInfo();
              rbnStateCountDown = 3;    
            }
            break;

          case DISPLAY_NO_FRIENDS_SPOTTED:

            // We are currently no friends spotted. We need to set
            // the new state to DISPLAY_DEVICE_INFO.

            currentRBNState = DISPLAY_DEVICE_INFO;  
            displayDeviceInfo();
            rbnStateCountDown = 3;               
            break;
        }
      }
      else {

        if (DISPLAY_FRIEND_SPOTS == currentRBNState) {

          displayFriendSpot(0,NULL);
        }
      }
    }
                    
    // Update lastSecond

    lastSecond = now;
  }

#if TURN_BACK_LIGHT_OFF == TURN_BACK_LIGHT_OFF_YES

  // We also need to turn the backlight on and off as required.

  // Decide which way round we are? Is the on time before the off time or
  // the other way round?

  int onMinutes = BACK_LIGHT_ON_HOUR * 60 + BACK_LIGHT_ON_MINUTE;
  int offMinutes = BACK_LIGHT_OFF_HOUR * 60 + BACK_LIGHT_OFF_MINUTE;

  // What if the time now - in minutes

  struct tm *timeInfo;
  timeInfo = localtime(&now);
    
  int nowMinutes = timeInfo->tm_hour * 60 + timeInfo->tm_min;
              
  if (onMinutes < offMinutes) {

    // Typically this would be lights between 09:00 and 21:00
                
    if ((nowMinutes >= onMinutes) && (nowMinutes < offMinutes)) {

      // We want the light on

      lcd.backlight();
    }
    else {

      // We want the light off

      lcd.noBacklight();                  
    }
  }
  else {

    // Typically this would be lights between 21:00 and 09:00

    if ((nowMinutes >= onMinutes) || (nowMinutes < offMinutes)) {

      // We want the light on

      lcd.backlight();
    }
    else {

      // We want the light off

      lcd.noBacklight();                  
    }                
  }
  
#endif                
  
  // Finally we need to check to see if all is well. There are various conditions
  // upon which we will reset.

  if (STATE_RECIEVING_RBN_DATA == currentState) {
    
    IPAddress wifiAddress = WiFi.localIP();
    if (((0 != runtimeData.LastSpotSeen) && (now - runtimeData.LastSpotSeen > MAX_SECONDS_BEFORE_RESTART)) ||
        (WiFi.status() != WL_CONNECTED) ||
        ((0 == wifiAddress[0]) && 0 == (wifiAddress[1]) && (0 == wifiAddress[2]) && (0 == wifiAddress[3]))
      ) {
      ESP.restart();
    }
  }
}
