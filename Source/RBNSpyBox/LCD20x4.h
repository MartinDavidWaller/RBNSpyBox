/*
 *  LCD20x4.h
 *  
 *  (C) 2019 Martin D. Waller - Martin@The-Wallers.net
 *  
 *  This code is licenced under the GPL.
 */
 
extern void addCentreString(const char* str);
extern void addCharToDisplayLineBuffer(const char c);
extern void addDoubleToDisplayLineBuffer(double v,int decimalPlaces);
extern void addStringToDisplayLineBuffer(const char* str);
extern void clearDisplay();
extern void clearDisplayLineBuffer();
extern void writeDisplayLineBuffer(int row);
extern void displayError(char *line1, char *line2, char *line3);
extern void padDisplayLineBufferTo(int length, char with);
extern void displayCentredTextInRow(char *text, int row);
