# RBNSpyBox

Reverse Beacon Network Spy Box

## Required Hardware

The circuit is based around the ESP32 ESP-WROOM-32 38-pin DevKitC breakout board. This was purchased via ebay
from the URL https://www.ebay.co.uk/itm/ESP32-Development-Board-Wireless-WiFi-Bluetooth-2-in1-Dual-Core-for-CP2102-ESP32/153034283929?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649
it mst be noted that other vendors supply this device too!

## Installing the Software

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

Next install the required libraries.

These reside in the the folder Arduino\libraries that sits under your users "Documents" folder.

Install ESPAsynchWebServer

- Open the URL https://github.com/me-no-dev/ESPAsyncWebServer
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "ESPAsyncWebServer-master" folder to the librraies folder mentioned above

Install AsyncTCP

- Open the URL https://github.com/me-no-dev/AsyncTCP
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "AsyncTCP-master" folder to the librraies folder mentioned above

Install Arduino-LiquidCrystal-I2C-library

- Open the URL https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "Arduino-LiquidCrystal-I2C-library-master" folder to the librraies folder mentioned above

## Compiling the source code

- Open the Arduino IDE and then open the RBNSpyBox.ino project file
- Navigate to Tools > Boards and select "ESP32 Dev Module"
- Navigate to Tools > Partition Scheme and select "No OTA (Large APP)"
- Click the "Verify" button (Round circle with a tick mark in it - top left), if successful you will see something similar to 
"Sketch uses 796430 bytes (37%) of program storage space. Maximum is 2097152 bytes.
Global variables use 59472 bytes (18%) of dynamic memory, leaving 268208 bytes for local variables. Maximum is 327680 bytes."
though the number may not be exact.

## SDCard

The SDCard holds the ring tones that can be associated with the Friends. The files total about 13 MB and must be stored on a FAT32 formatted SDCard. 
The file structure must reflect that of the SDCard folder that is the top-level folder must be "01" with the MP3 files stored below it.
