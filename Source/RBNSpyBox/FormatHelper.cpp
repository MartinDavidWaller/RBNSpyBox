/*
 *  FormatHelper.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <time.h>                      
#include <sys/time.h>                   
#include <HardwareSerial.h>
#include <WiFi.h>

char *moy[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

char *FormatFrequency(float frequency) {

  static char formatBuffer[20];  
  
  sprintf(formatBuffer,"%.4f Mhz",frequency);

  return formatBuffer;
}

char *FormatTimeAsDateTime(time_t _time) {

  static char formatBuffer[20];
  
  struct tm *timeInfo;
  
  timeInfo = localtime(&_time);
  
  sprintf(formatBuffer,"%02d-%s-%03d %02d:%02d:%02d",
    timeInfo->tm_mday,
    moy[timeInfo->tm_mon],
    timeInfo->tm_year + 1900,
    timeInfo->tm_hour,
    timeInfo->tm_min,
    timeInfo->tm_sec);
  
  return formatBuffer;
}

char *FormatIPAddress(IPAddress ipAddress) {

  static char formatBuffer[20] = {0};

  sprintf(formatBuffer,"%d.%d.%d.%d",ipAddress[0],ipAddress[1],ipAddress[2],ipAddress[3]);

  return formatBuffer;  
}

#define SECONDS_PER_MINUTE (60)
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * 60)
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * 24)

char *FormatUptime(time_t *bootTime) {

  static char formatBuffer[20] = {0};
  
  struct tm *timeInfo;
  
  timeInfo = localtime(bootTime);

  if (timeInfo->tm_year + 1900 < 2018) {
    time(bootTime);
  }
  else {

    time_t now; 
    time(&now);

    time_t upTimeSeconds;
    upTimeSeconds = now - *bootTime;
    
    int daysUp = upTimeSeconds / SECONDS_PER_DAY;
    upTimeSeconds = upTimeSeconds - (daysUp * SECONDS_PER_DAY);

    int hoursUp = upTimeSeconds / SECONDS_PER_HOUR;
    upTimeSeconds = upTimeSeconds - (hoursUp * SECONDS_PER_HOUR);

    int minutesUp = upTimeSeconds / SECONDS_PER_MINUTE;
    upTimeSeconds = upTimeSeconds - (minutesUp * SECONDS_PER_MINUTE);  

    sprintf(formatBuffer,"%d %02d:%02d:%02d",
      daysUp,
      hoursUp,
      minutesUp,
      upTimeSeconds);
  }
  
  return formatBuffer;   
}
