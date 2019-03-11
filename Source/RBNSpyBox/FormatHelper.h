/*
 *  FormatHelper.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
char *FormatTimeAsDateTime(time_t _time);
char *FormatUptime(time_t *bootTime);
char *FormatFrequency(float frequency);
char *FormatIPAddress(IPAddress ipAddress);
char *FormatWithTriple(int v, char *tempBuffer);
