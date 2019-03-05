/*
 *  XMLHelper.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include "Arduino.h"

char *escapeXML(char *s) {

  static char b[1024];
  b[0] = '\0';

  for(int i = 0; i < strlen(s); i++) {

    switch(s[i]) {

      case '\"': strcat(b,"&quot;"); break;   
      case '&':  strcat(b,"&amp;");  break;   
      case '<':  strcat(b,"&lt;");   break;   
      case '>':  strcat(b,"&gt;");   break;   
              
      default:

        int l = strlen(b);
        b[l] = s[i];
        b[l + 1] = '\0';
        break;
    }
  }
}

void addTagNameValue(char *b, char *_name, char *value) {

  strcat(b,_name);
  strcat(b,"=\"");
  strcat(b,value);
  strcat(b,"\" ");      
}
