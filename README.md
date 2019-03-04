# RBNSpyBox

Reverse Beacon Network Spy Box

# Installing the Software

Start by installing the Arduino IDE. This can be found at:

https://www.arduino.cc/en/Main/Software

For the purpose of this build the Windows version Arduino 1.8.8 was used. 
All default installation options were accepted.

Next install the ESP32 board manager.

- Open the Arduino IDE and navigate to File > Preferences.
- Enter "https://dl.espressif.com/dl/package_esp32_index.json" into the box labelled
"Additional Boards manager URLs:" then click the "OK" button.
- Open the boards manager, navigate to Tools > Board > Boards Manager...
- Search for ESP32 by typing "ESP32" into the top text box.
- Cursor ove rthe "esp32" entry and click the "Install" button. This displayed version 1.0.1.

## SDCard

The SDCard holds the ring tones that can be associated with the Friends. The files total about 13 MB and must be stored on a FAT32 formatted SDCard. 
The file structure must reflect that of the SDCard folder that is the top-level folder must be "01" with the MP3 files stored below it.
