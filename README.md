# RBNSpyBox

Reverse Beacon Network Spy Box

# Installing the Software

Start by installing the Arduino IDE. This can be found at:

https://www.arduino.cc/en/Main/Software

For the purpose of this build the Windows version Arduino 1.8.8 was used. 
All default installation options were accepted.

Next install the ESP32 board manager.

- Open the Arduino IDE and navigate to File > Preferences
- Enter "https://dl.espressif.com/dl/package_esp32_index.json" into the box labelled
"Additional Boards manager URLs:" then click the "OK" button
- Open the boards manager, navigate to Tools > Board > Boards Manager...
- Search for ESP32 by typing "ESP32" into the top text box
- Cursor over the "esp32" entry and click the "Install" button. This displayed as version 1.0.1
- Once complete hit the "Close" button to close the dialog
- Close and re-open the Arduino IDE. On the Tools > Boards menu you should now see en entry labelled "ESP32 Dev Module"

Next install the ESP32 Fileystem Uploader.

- Open the realease page: https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
- Download ESP32FS-1.0.zip and open it
- Move to your Documents folder and then into the "Arduino" folder. Create, if necessary, a
folder called "tools" and copy the ESP32FS folder from the zip file to the "tools" folder
- Close and re-open the Arduino IDE. On the Tools menu you should now see an entry labelled "ESP32 Sketch Data Upload"

## SDCard

The SDCard holds the ring tones that can be associated with the Friends. The files total about 13 MB and must be stored on a FAT32 formatted SDCard. 
The file structure must reflect that of the SDCard folder that is the top-level folder must be "01" with the MP3 files stored below it.
