/*
 *  LCD20x4.cpp
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>

#include "RBNSpyBox.h"

#define DISPLAY_LINE_BUFFER_LENGTH 20
#define DELAY_ERROR 4

extern LiquidCrystal_I2C lcd;

char displayLineBuffer[DISPLAY_LINE_BUFFER_LENGTH + 1];
int displayLineBufferIndex;

void clearDisplay() {

  Serial.println();
  
  lcd.clear();
}

void clearDisplayLineBuffer()
{
  for(int i = 0; i < DISPLAY_LINE_BUFFER_LENGTH; i++)
    displayLineBuffer[i] = ' ';
    
  displayLineBuffer[DISPLAY_LINE_BUFFER_LENGTH] = '\0';
  displayLineBufferIndex = 0;
}

void addCharToDisplayLineBuffer(const char c)
{
  if (displayLineBufferIndex < DISPLAY_LINE_BUFFER_LENGTH)
    displayLineBuffer[displayLineBufferIndex++] = c;
}

void addDoubleToDisplayLineBuffer(double v,int decimalPlaces)
{
  char buffer[20];
  char *p = dtostrf(v,10,decimalPlaces,buffer);

  while (*p != 0 && displayLineBufferIndex < DISPLAY_LINE_BUFFER_LENGTH)
  {
    if (*p != ' ')
      displayLineBuffer[displayLineBufferIndex++] = *p;
    p++;
  }
}

// This method pads the line buffer to the given length
// with the given character.

void padDisplayLineBufferTo(int length, char with)
{
  while (displayLineBufferIndex < length)
    displayLineBuffer[displayLineBufferIndex++] = with;
}

void addStringToDisplayLineBuffer(const char* str)
{
  char *p = (char *)str;
  while (*p != 0 && displayLineBufferIndex < DISPLAY_LINE_BUFFER_LENGTH)
    displayLineBuffer[displayLineBufferIndex++] = *p++;
}

void addCentreString(const char* str)
{
  char *p = (char *)str;

  // Work out how much indent we need
  
  int stringLength = strlen(p);
  int requiredIndent = (20 - stringLength) / 2;

  // Pad out the line

  padDisplayLineBufferTo(requiredIndent,' ');
  
  addStringToDisplayLineBuffer(str);
}

void writeDisplayLineBuffer(int row)
{
  
#if DEBUG_ECHO_DISPLAY == 1
  Serial.print("LCD[");
  Serial.print(row);
  Serial.print("] = |");
  Serial.print(displayLineBuffer);
  Serial.println("|");
#endif

  lcd.setCursor(0,row); 
  lcd.print(displayLineBuffer);
}

// This method is called to write out an error. It takes three
// three strings for lines 1,2, and 3. If passed as NULL then
// no output is generated for that line.

void displayError(char *line1, char *line2, char *line3)
{

  // Build the lines
  
  char *lines[] = {"Error:", line1, line2, line3};
  
  // Clear the display

  lcd.clear();

  // Display the lines

  for(int i = 0; i < 4; i++) {

    if (NULL != lines[i]) {
      clearDisplayLineBuffer();
      addStringToDisplayLineBuffer(lines[i]);
      writeDisplayLineBuffer(i);
    }
  }
  
  // Delay

  delay(DELAY_ERROR * 1000);
}

void displayCentredTextInRow(char *text, int row) {

  clearDisplayLineBuffer();
  addCentreString(text);
  writeDisplayLineBuffer(row); 
}
