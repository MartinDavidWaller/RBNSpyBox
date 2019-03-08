/*
 *  WebServer.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */

#include "Arduino.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

#include <HardwareSerial.h>
#include <time.h>                      
#include <sys/time.h>         


#include "Configuration.h"
#include "FormatHelper.h"
#include "Friend.h"
#include "Friends.h"
#include "LCD20x4.h"
#include "RBNSpyBox.h"
#include "Sounds.h"
#include "XMLHelper.h"

#define SSID_NAME "RBNSpyBox"

AsyncWebServer server(80); 

bool waitingOnData;
extern struct Configuration configuration;

void onDoReboot(AsyncWebServerRequest *request){

  // Redirect to the index page
    
  request->redirect("friends.html");  

  ESP.restart();
}

void onDoAddFriend(AsyncWebServerRequest *request){

  AsyncWebParameter* callsignParam = request->getParam("addFriendCallsign");
  AsyncWebParameter* nameParam = request->getParam("addFriendName");
  AsyncWebParameter* commentParam = request->getParam("addFriendComment");
  
  AsyncWebParameter* soundSelectionParam = request->getParam("addFriendSoundSelection");

  bool soundEnabled = false;
  
  if(request->hasParam("addFriendSoundEnabled")) {
    
    AsyncWebParameter* soundEnabledParam = request->getParam("addFriendSoundEnabled");
    if (NULL != soundEnabledParam->value()) {
      soundEnabled = (0 == strcmp("on", soundEnabledParam->value().c_str()));
    }
  }
 
  bool friendAdded = AddNewFriend(
    callsignParam->value().c_str(),
    nameParam->value().c_str(),
    commentParam->value().c_str(),
    soundEnabled,
    atoi(soundSelectionParam->value().c_str()));

  // Redirect to the index page
    
  request->redirect("friends.html");  
}

void onDoRestoreFriendsBegin(AsyncWebServerRequest *request) {

  RemoveAllFriends(false);
   
  // Return OK

  request->send(200, "text/plain", "OK");          
}

void onDoRestoreFriendsCommit(AsyncWebServerRequest *request) {

  CommitAllFriends();
   
  // Return OK

  request->send(200, "text/plain", "OK");          
}

void onDoRestoreFriend(AsyncWebServerRequest *request) {
 
  AsyncWebParameter* memorySlotParam = request->getParam("MemorySlot");
  AsyncWebParameter* callsignParam = request->getParam("Callsign");
  AsyncWebParameter* nameParam = request->getParam("Name");
  AsyncWebParameter* commentParam = request->getParam("Comment");
  AsyncWebParameter* soundEnabledParam = request->getParam("SoundEnabled");
  AsyncWebParameter* soundIndexParam = request->getParam("SoundIndex");
  AsyncWebParameter* soundParam = request->getParam("Sound");
  AsyncWebParameter* lastSeenAsParam = request->getParam("LastSeenAs");
  AsyncWebParameter* lastSeenParam = request->getParam("LastSeen");
  AsyncWebParameter* lastSeenSecondsParam = request->getParam("LastSeenSeconds");
  AsyncWebParameter* lastSpeedParam = request->getParam("LastSpeed");
  AsyncWebParameter* lastSpeedUnitsParam = request->getParam("LastSpeedUnits");
  AsyncWebParameter* lastFrequencyParam = request->getParam("LastFrequency");
  AsyncWebParameter* lastModeParam = request->getParam("LastMode");

  Serial.printf("**** Memory Slot = %d\n",atoi(memorySlotParam->value().c_str()));
  Serial.printf("**** Callsign = %s\n",callsignParam->value().c_str());
  Serial.printf("**** Name = %s\n",nameParam->value().c_str());
  Serial.printf("**** Comment = %s\n",commentParam->value().c_str());
  Serial.printf("**** SoundEnabled = %s\n",soundEnabledParam->value().c_str());
  Serial.printf("**** SoundIndex = %s\n",soundIndexParam->value().c_str());
  Serial.printf("**** Sound = %s\n",soundParam->value().c_str());
  Serial.printf("**** Last Seen As = %s\n",lastSeenAsParam->value().c_str());
  Serial.printf("**** Last Seen = %s\n",lastSeenParam->value().c_str());
  Serial.printf("**** Last Seen Seconds = %s\n",lastSeenSecondsParam->value().c_str());
  Serial.printf("**** Last Speed = %s\n",lastSpeedParam->value().c_str());
  Serial.printf("**** Last Frequency = %s\n",lastFrequencyParam->value().c_str());
  Serial.printf("**** Last Mode = %s\n",lastModeParam->value().c_str());

  RestoreFriend(
    atoi(memorySlotParam->value().c_str()),
    callsignParam->value().c_str(), 
    nameParam->value().c_str(),
    commentParam->value().c_str(), 
    (0 == strcmp("on", soundEnabledParam->value().c_str())),
    atoi(soundIndexParam->value().c_str()),
    lastSeenAsParam->value().c_str(),
    atoi(lastSeenSecondsParam->value().c_str()),
    atoi(lastSpeedParam->value().c_str()),
    lastSpeedUnitsParam->value().c_str(),
    atof(lastFrequencyParam->value().c_str()),
    lastModeParam->value().c_str());
   
  // Return OK

  request->send(200, "text/plain", "OK");          
}

void onDoFriendUpdate(AsyncWebServerRequest *request) {

  // Pull out the parameters

  AsyncWebParameter* memorySlotParam = request->getParam("MemorySlot");
  AsyncWebParameter* callsignParam = request->getParam("Callsign");
  AsyncWebParameter* nameParam = request->getParam("Name");
  AsyncWebParameter* commentParam = request->getParam("Comment");
  AsyncWebParameter* soundEnabledParam = request->getParam("SoundEnabled");
  AsyncWebParameter* soundSelectedParam = request->getParam("SoundSelected"); // Integer value

  //Serial.printf("**** Memory Slot = %s\n",memorySlotParam->value().c_str());
  //Serial.printf("**** Callsign = %s\n",callsignParam->value().c_str());
  //Serial.printf("**** Name = %s\n",nameParam->value().c_str());
  //Serial.printf("**** Comment = %s\n",commentParam->value().c_str());
  //Serial.printf("**** Sound Enabled = %s\n",soundEnabledParam->value().c_str());
  //Serial.printf("**** Sound Selected = %s\n",soundSelectedParam->value().c_str());
  
  // Update the friend
  
  UpdateFriend(
    atoi(memorySlotParam->value().c_str()), 
    callsignParam->value().c_str(),
    nameParam->value().c_str(),
    commentParam->value().c_str(),
    (0 == strcmp("on", soundEnabledParam->value().c_str())),
    atoi(soundSelectedParam->value().c_str())
    );

  // Return OK

  request->send(200, "text/plain", "OK");
}

void onDoRemoveFriend(AsyncWebServerRequest *request){

  AsyncWebParameter* callsignParam = request->getParam("callsign");

  bool friendRemoved = RemoveFriend(callsignParam->value().c_str());

  if (true == friendRemoved)
   request->send(200, "text/plain", "Friend removed!");
  else
   request->send(200, "text/plain", "Friend not found?");
}

void onDoRemoveAllFriends(AsyncWebServerRequest *request){

  RemoveAllFriends(true);

  request->redirect("friends.html"); 
}

void onDoListen(AsyncWebServerRequest *request){

  AsyncWebParameter* soundParam = request->getParam("sound");

  PlaySound(atoi(soundParam->value().c_str()));
  //playSound(atoi(soundParam->value().c_str()));
    
  request->send(200, "text/plain", "OK");
}

void onDoListenWithVolume(AsyncWebServerRequest *request){

  AsyncWebParameter* volumeParam = request->getParam("volume");

  //doListenWithVolumeVolume = atoi(volumeParam->value().c_str());
  //doListenWithVolume = true;

  PlaySoundAtVolume(SOUND_NOTE,atoi(volumeParam->value().c_str()),configuration.Volume);
    
  request->send(200, "text/plain", "OK");
}

void onDoSettingsUpdate(AsyncWebServerRequest *request){

  AsyncWebParameter* hostnameInputParam = request->getParam("hostnameInput");
  AsyncWebParameter* passwordInputParam = request->getParam("passwordInput");
  AsyncWebParameter* ssidInputParam = request->getParam("ssidInput");
  AsyncWebParameter* callsignInputParam = request->getParam("callsignInput");

  AsyncWebParameter* ledEnabledInputParam = NULL;
  if(request->hasParam("ledEnabledInput"))
    ledEnabledInputParam = request->getParam("ledEnabledInput");
      
  AsyncWebParameter* soundEnabledInputParam = NULL;
  if(request->hasParam("soundEnabledInput"))
    soundEnabledInputParam = request->getParam("soundEnabledInput");
    
  AsyncWebParameter* volumeInputParam = request->getParam("volumeInput");
  AsyncWebParameter* friendCycleCountInputParam = request->getParam("friendCycleCountInput");
  
  Serial.print("...hostnameInput = ");
  Serial.println(hostnameInputParam->value().c_str());
      
  Serial.print("...passwordInputParam = ");
  Serial.println(passwordInputParam->value().c_str());
      
  Serial.print("...ssidInputParam = ");
  Serial.println(ssidInputParam->value().c_str());

  Serial.print("...callsignInputParam = ");
  Serial.println(callsignInputParam->value().c_str());

  strcpy((char*)&configuration.Hostname[0],hostnameInputParam->value().c_str());
  strcpy((char*)&configuration.WiFi_SSID[0],ssidInputParam->value().c_str());
  strcpy((char*)&configuration.WiFi_Password[0],passwordInputParam->value().c_str());
  strcpy((char*)&configuration.Callsign[0],callsignInputParam->value().c_str());

  // Update LED enabled
  
  bool ledEnabled = false;
  if (NULL != ledEnabledInputParam) {
    ledEnabled = (0 == strcmp("on", ledEnabledInputParam->value().c_str()));
  }
    
  if (true == ledEnabled) {
   SET_LED_ENABLED((&configuration)); 
  }
  else {
    CLEAR_LED_ENABLED((&configuration));
  }
  
  // Update Sound enabled
  
  bool soundEnabled = false;
  if (NULL != soundEnabledInputParam) {
    soundEnabled = (0 == strcmp("on", soundEnabledInputParam->value().c_str()));
  }
    
  if (true == soundEnabled) {
   SET_SOUND_ENABLED((&configuration)); 
  }
  else {
    CLEAR_SOUND_ENABLED((&configuration));
  }
  
  configuration.Volume = atoi(volumeInputParam->value().c_str());
  configuration.FriendCycleCount = atoi(friendCycleCountInputParam->value().c_str());

  //setNewVolume();
  SetSoundVolume(configuration.Volume);

  // Write it out
    
  writeConfiguration(&configuration); 
  
  waitingOnData = false;  

  // Redirect to the settings page
    
  request->redirect("settings.html");  
}

void onGetSettingsData(AsyncWebServerRequest *request){

  AsyncResponseStream *response = request->beginResponseStream("text/xml");

  response->printf("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  response->printf("<SettingsData ");

  response->printf("Hostname=\"%s\" ",&configuration.Hostname[0]);
  response->printf("WiFiSSID=\"%s\" ",&configuration.WiFi_SSID[0]);
  response->printf("WiFiPassword=\"%s\" ",&configuration.WiFi_Password[0]);
  response->printf("Callsign=\"%s\" ",&configuration.Callsign[0]); 
  response->printf("FriendCycleCount=\"%d\" ",configuration.FriendCycleCount);
  response->printf("LEDEnabled=\"%s\" ",IS_LED_ENABLED((&configuration)) ? "true" : "false");
  response->printf("SoundEnabled=\"%s\" ",IS_SOUND_ENABLED((&configuration)) ? "true" : "false");
  response->printf("Volume=\"%d\" ",configuration.Volume); 
  
  response->printf("/>");

  request->send(response);
}

AsyncResponseStream *response;

void onGetFriends(AsyncWebServerRequest *request){

  response = request->beginResponseStream("text/xml");
  
  GetFriendsAsXML([] (char *xmlLine) 
  {
    response->printf("%s",escapeXML(xmlLine));
  });
    
  request->send(response);
}

void onGetFriendsForEdit(AsyncWebServerRequest *request){

  response = request->beginResponseStream("text/xml");
  
  GetFriendsForEditAsXML([] (char *xmlLine) 
  {
    response->printf("%s",escapeXML(xmlLine));
  });
    
  request->send(response);
}

void onGetSounds(AsyncWebServerRequest *request){

  response = request->beginResponseStream("text/xml");
  
  GetSoundsAsXML([] (char *xmlLine) 
  {
    response->printf("%s",xmlLine);
  });
    
  request->send(response);
}

void onGetNameVersion(AsyncWebServerRequest *request){

  AsyncResponseStream *response = request->beginResponseStream("text/xml");

  response->printf("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  response->printf("<IndexNameVersion ");

  response->printf("Name=\"%s\" ",PROGRAM_NAME);
  response->printf("Version=\"V%d.%d\" ",PROGRAM_VERSION_MAJOR,PROGRAM_VERSION_MINOR); 
  response->printf("Copyright=\"2019 M.D.Waller G0PJO. All rights reserved.\"");

  response->printf("/>");

  request->send(response);
}

void onGetRuntime(AsyncWebServerRequest *request){

  AsyncResponseStream *response = request->beginResponseStream("text/xml");

  response->printf("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  response->printf("<Runtime xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">");

  response->printf("<ConfigurationSize>%d</ConfigurationSize>",sizeof(Configuration)); 
  response->printf("<CpuFreqMHz>%d</CpuFreqMHz>",ESP.getCpuFreqMHz()); 
  response->printf("<FreeHeap>%d</FreeHeap>",ESP.getFreeHeap());
  response->printf("<FriendSize>%d</FriendSize>",sizeof(Friend)); 
  response->printf("<MaxFriendCount>%d</MaxFriendCount>",MAX_FRIEND_COUNT); 
  response->printf("<FriendMemoryRequirement>%d</FriendMemoryRequirement>",MAX_FRIEND_COUNT * sizeof(Friend)); 
  response->printf("<RBNConnectionsMade>%d</RBNConnectionsMade>",runtimeData.RBNConnectionsMade); 
  response->printf("</Runtime>");

  request->send(response);
}

void onGetUpTimeAndSpotData(AsyncWebServerRequest *request){

  AsyncResponseStream *response = request->beginResponseStream("text/xml");

  response->printf("<?xml version=\"1.0\" encoding=\"utf-16\"?>");
  response->printf("<UpTimeAndSpotData ");
  
  response->printf("LastSpotSeen=\"%s\" ",FormatTimeAsDateTime(runtimeData.LastSpotSeen));
  response->printf("SpotsSeen=\"%d\" ",runtimeData.SpotsSeen); 
  response->printf("UpTime=\"%s\"",FormatUptime(&runtimeData.BootTime));
  
  response->printf("/>");

  request->send(response);
}

void onGetSIDDs(AsyncWebServerRequest *request){

  char b[1024];
  b[0] = '\0';

  Serial.println("");
  Serial.println("Scanning for SSIDs");
  
  // We need to return a blob of XML containing the visible SSIDs
  
  strcpy(b,"<SSIDs>");

//**************

  int n = WiFi.scanComplete();
  if(n == -2){
    WiFi.scanNetworks(true);
  } else if(n){
    for (int i = 0; i < n; ++i){
    // Add the SSID to the result
      
    strcat(b,"<SSID Name = \"");
    strcat(b,WiFi.SSID(i).c_str());
    strcat(b,"\" />");

    Serial.println("... " + WiFi.SSID(i));
    }
    WiFi.scanDelete();
    if(WiFi.scanComplete() == -2){
      WiFi.scanNetworks(true);
    }
  }

  //for (int i = 0; i < n; ++i) 
  //{
    // Add the SSID to the result
      
    //strcat(b,"<SSID Name = \"");
    //strcat(b,WiFi.SSID(i).c_str());
    //strcat(b,"\" />");

    //Serial.println("... " + WiFi.SSID(i));
  //}


  // Complete the XML
  
  strcat(b,"</SSIDs>");

  // Send it to the server
  
  request->send(200, "text/xml", b);
}

void onGetBackup(AsyncWebServerRequest *request){

  response = request->beginResponseStream("text/xml");

  char filnameBuffer[100];
  
  struct tm *timeInfo;

      time_t now; 
    time(&now);
    
  timeInfo = localtime(&now);
  
  sprintf(filnameBuffer,"attachment; filename=\"%s_%04d%02d%02d_%02d%02d%02d.xml\";",
    PROGRAM_NAME,
    timeInfo->tm_year + 1900,
    timeInfo->tm_mday,
    timeInfo->tm_mon + 1,
    timeInfo->tm_hour,
    timeInfo->tm_min,
    timeInfo->tm_sec); 

  response->addHeader("Content-Disposition",filnameBuffer);

  response->printf("<?xml version=\"1.0\" encoding=\"utf-16\"?>\n");
  response->printf("<%s Major=\"%d\" Minor=\"%d\">\n",PROGRAM_NAME,PROGRAM_VERSION_MAJOR,PROGRAM_VERSION_MINOR);

  //response->printf("    <Configuration ");
  
  //response->printf("Hostname=\"%s\" ",&configuration.Hostname[0]);
  //response->printf("WiFiSSID=\"%s\" ",&configuration.WiFi_SSID[0]);
  //response->printf("WiFiPassword=\"%s\" ",&configuration.WiFi_Password[0]);
  //response->printf("Callsign=\"%s\" ",&configuration.Callsign[0]); 
  //response->printf("FriendCycleCount=\"%d\" ",configuration.FriendCycleCount);
  //response->printf("SoundEnabled=\"%s\" ",IS_SOUND_ENABLED((&configuration)) ? "true" : "false");
  //response->printf("Volume=\"%d\" ",configuration.Volume); 
  
  //response->printf(" />\n");

  GetFriendsForBackup([] (char *xmlLine) 
  {
    response->printf("    %s\n",xmlLine);
  });
  
  response->printf("</%s>\n",PROGRAM_NAME);

  request->send(response);
}

/*
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){

  static File f; //= SPIFFS.open("/f.txt", "w");
  
  if(!index){
    Serial.printf("UploadStart: %s\n", filename.c_str());

    f = SPIFFS.open("/restore.xml", "w");
  }
  for(size_t i=0; i<len; i++){
    Serial.write(data[i]);
    f.write(data[i]);
  }
  if(final){
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    f.close();
  }
}
*/

extern void buildServer()
{
  // We can now configure and start the server

  server.on("/doAddFriend", HTTP_GET, onDoAddFriend);
  server.on("/doFriendUpdate", HTTP_GET, onDoFriendUpdate);
  server.on("/doListen", HTTP_GET, onDoListen);  
  server.on("/doListenWithVolume", HTTP_GET, onDoListenWithVolume);  
  server.on("/doReboot", HTTP_GET, onDoReboot);
  server.on("/doRemoveFriend", HTTP_GET, onDoRemoveFriend);
  server.on("/doRemoveAllFriends", HTTP_GET, onDoRemoveAllFriends);
  server.on("/doRestoreFriend", HTTP_GET, onDoRestoreFriend);
  server.on("/doRestoreFriendsBegin", HTTP_GET, onDoRestoreFriendsBegin);
  server.on("/doRestoreFriendsCommit", HTTP_GET, onDoRestoreFriendsCommit);
  server.on("/doSettingsUpdate", HTTP_GET, onDoSettingsUpdate);  
  server.on("/getBackup", HTTP_GET, onGetBackup);
  server.on("/getFriends", HTTP_GET, onGetFriends);
  server.on("/getFriendsForEdit", HTTP_GET, onGetFriendsForEdit);
  server.on("/getNameVersion", HTTP_GET, onGetNameVersion);
  server.on("/getRuntime", HTTP_GET, onGetRuntime);
  server.on("/getSettingsData", HTTP_GET, onGetSettingsData);
  server.on("/getSounds", HTTP_GET, onGetSounds);
  server.on("/getSSIDs", HTTP_GET, onGetSIDDs);
  server.on("/getUpTimeAndSpotData", HTTP_GET, onGetUpTimeAndSpotData);
    
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  
  server.begin();
}
