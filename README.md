# RBNSpyBox

Reverse Beacon Network Spy Box

## Required Hardware

The circuit is based around the ESP32 ESP-WROOM-32 38-pin DevKitC breakout board. This was purchased via ebay
from the URL https://www.ebay.co.uk/itm/ESP32-Development-Board-Wireless-WiFi-Bluetooth-2-in1-Dual-Core-for-CP2102-ESP32/153034283929?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649
it must be noted that other vendors supply this device too!

The LCD module is a Yellow IIC I2C TWI 2004 20x4 Serial LCD Module Display. Bought from eBay. A typical vendor being
https://www.ebay.co.uk/itm/With-Without-IIC-I2C-2004-20X4-Character-LCD-Module-Display-Blue-Green-Arduino/263128525818?hash=item3d43aeabfa:rk:5:pf:0&var&checksum=263128525818d2084b387f5a4bce874e21a98eafa17e&enc=AQADAAADAFjVrDbVsZ8oH%2F8PNHtt9VX4%2Fw7FZcmMuqsX8uaFEduVNikuAV%2FYybasnlPApClObG%2BQxNE19rDxiyNaRftM41q9vIBIyjiM6qEBc9K50kG%2Folz6Z8gLFsCm1%2F68KcmNMoV3R15zoQMTphBaUZTlhdt0oTQFi%2F1PhK4FanGiT7ZLevB57thPpxqUgp5vaLo1APgFTuLjEF2l9rMjyO9K8jbgtTJBirsApQOasrGvr1Y2c%2F4QP4jqnT5ArHqR5jYFp7LaWVPiFzeFwO1p7M2pnObvCbS%2B5W91Tpp279GE%2FC25ZgxrH2ywKUJVmPc3V7g8cSi7H8OyG1DrLV0HYbrqO%2F5jBgFfQB5KW1bHu0YFgTfSiirQaMPGLayEuifJ8uC1xW83wvc2laHu9PUQfD%2Fv8mj8pekCBoHLC9JULa39BmhGt4r7THGR10t4tEAiPo8N%2B9olJYnhismZI3GcqiZ4y8%2B%2BXgpeBy97KLPhbl49UvkZk4E%2FqczzcdxflU%2FxIjfscN90gjcZ6EmaWk%2FjdOcH%2F0VlVpg8KFAelIDMgKzD0HbxhUJq3cmNBT6xrhEedtbSqpKcBHdYvnLunAW83lUM%2B%2BYPe8AzXtmswtnplQOmFMfjgsKuHqUXoZOHsw1sCeklfR7%2BquWb6zkolsknZEkbBlgKXDVApiwj300oTwlUyimiIHCeEXUsHJwSwdRYHKHdzSSlwR4RWEAILNLBzn768rctI%2FtB88POJyCgpg%2FIPDkOSeP0JBpGazC9oxkNBEDTytLYgoSsjH3KMpdEVJS%2FUYf%2FBtdizvEe3T9Ht4%2BsuCuq%2F%2F%2B0si9WzrVwz7TqNLtGrM4e793qmiPsN0AfAUtQ2spPa7oc1fnzJBXMkjBmwxbvweM8j%2FIMCgMjJ2no03lXQyCCxOp%2BR3LGZU2VofH9%2FZV1CIFNiofU6QaiIZYD8wR1ur92PIMitj284MxJ0vLsUHXciNoTVbu6yg9wThnfSZMohTBNKe2xrROgIIjNa0MUSJoaV7rNaFPzEaDxFgVDBQ%3D%3D
it must be noted that other vendors supply this device too!

The DFPlayer was purchased via ebay from the URL https://www.ebay.co.uk/itm/DFplayer-Mini-MP3-Player-Module-For-Arduino-UNO-Module-Music-Audio-Voice-Board/263899839783?hash=item3d71a7fd27:rk:1:pf:0&checksum=263899839783568eeb3e45f04723a80e138821300135&enc=AQADAAAC8FjVrDbVsZ8oH%2F8PNHtt9VX4%2Fw7FZcmMuqsX8uaFEduV46LS4iVtuX%2FEhDE66dK62kidlwWlGksu4Kd9INvM6JfHvBbLqK4n4DG00h2iEqlRZtfPIncd1mi6HyYAKELuss9epmSGz3RMQf1EtKmTTGnxTQyFB6VkEu%2F4e%2F0VunxDjljkgTQJvU4rq7IthfNN09PTfnGSbVGr1eITR8nPJ6jbXi1S%2F6UlTXJL0xeUsxArS7uS1FbUl1PK8gi%2BOSKg3%2FVN9ZQ0x6vdwH5R5Zq3C3HPuxEU%2BjlM%2BWX9LCnfwjOqqReEm98O2CsRIE3M5cBtpjke4ZkZ%2BDO0hKBTkY8PhpU%2FNZ0RwbGBKP%2BmX1kf7JiNkN2aKwgmRck4coQsaQmT%2FTxx314AndJKzAv5xMuCx%2Fekx6pz%2Ft%2FVtO6ceghCjeMREneJnGHbJvDz2gmzPTo6QwCO8vQo2ILSwRrlHJTy38O%2Bj85cuIRGJy8Fp3fotbw8pUUWW8fQ5Hko8iFu7slg8WV6khVW%2Fwi2Au22IByoHgz52Fs8c%2BejC5GWQqLLJnCEo2YBRVnopp3%2FfNEAhfppyPBnz8Mh5nb4Jnto2MMLp65UAIpUNKB%2BJP%2BF6yz6zaeSSR5htjhvhXNSHDQERpNI3StaJRZs9LK5rJLdJQgYjBBB6XfqfcTSUKZ6PON676218%2BjISYmf2fwszUhcMUQN%2B1kKSUotS9QjTmNnmmX60OLKI93uIaJYwA4oQWbimaBnkre5LgVpvLN6nz1mqSAQa3MElFRo2pdbIWuQGkgZ%2BTV0KXiGo6W8L3dPLUmSorxOe1ZarpDRC4HZ2l9oS5WJ7hmIncQlvm5lhgAZ2lXZ8xyMdafO6UrrKZjKrSkAoAy%2B1U%2FWosfR7ycCyQREIpXFpQXT4VzEzloTSr1E%2BRGJyRBVS90t5gjsD%2BBn%2BAiWqTGU1LtmX5fyVkCHf3yx0ERdPOwBjkMg19AlBjbeDpWSQcA44%2Fjwzrgvl%2BgGYJe6opio
// it must be noted that other vendors supply this device too!

The AT24C64 EEPROM was purchased via ebay from the URL https://www.ebay.co.uk/itm/LC-Technology-AT24C64-EEPROM-Memory-Module-LC-AT24-C64-I2C-Arduino-Flux-Workshop/112081460945?ssPageName=STRK%3AMEBIDX%3AIT&_trksid=p2057872.m2749.l2649 
it must be noted that other vendors supply this device too! This is a breakout board that was originally used for development purposes. The chip itself was later extracted and used on the PCB.

## Installing the Build Software

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
- Close and re-open the Arduino IDE. On the Tools > Boards menu you should now see an entry labelled "ESP32 Dev Module"

Next install the ESP32 Filesystem Uploader.

- Open the release page: https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
- Download ESP32FS-1.0.zip and open it
- Move to your Documents folder and then into the "Arduino" folder. Create, if necessary, a
folder called "tools" and copy the ESP32FS folder from the zip file to the "tools" folder
- Close and re-open the Arduino IDE. On the Tools menu you should now see an entry labelled "ESP32 Sketch Data Upload"

Next install the required libraries.

These reside in the the folder Arduino\libraries that sits under your users "Documents" folder.

Install ESPAsynchWebServer

- Open the URL https://github.com/me-no-dev/ESPAsyncWebServer
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "ESPAsyncWebServer-master" folder to the libraries folder mentioned above

Install AsyncTCP

- Open the URL https://github.com/me-no-dev/AsyncTCP
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "AsyncTCP-master" folder to the libraries folder mentioned above

Install Arduino-LiquidCrystal-I2C-library

- Open the URL https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
- Click on the green "Clone or download" button and select "Download ZIP"
- Open the downloaded zip file and copy the folder "Arduino-LiquidCrystal-I2C-library-master" folder to the libraries folder mentioned above

## Compiling the source code

- Open the Arduino IDE and then open the RBNSpyBox.ino project file
- Navigate to Tools > Boards and select "ESP32 Dev Module"
- Navigate to Tools > Partition Scheme and select "No OTA (Large APP)"
- Click the "Verify" button (Round circle with a tick mark in it - top left), if successful you will see something like 
"Sketch uses 796430 bytes (37%) of program storage space. Maximum is 2097152 bytes.
Global variables use 59472 bytes (18%) of dynamic memory, leaving 268208 bytes for local variables. Maximum is 327680 bytes."
though the number may not be exact.

## SD Card

The SD Card holds the ring tones that can be associated with the Friends. The files total about 13 MB and must be stored on a FAT32 formatted SDCard. 
The file structure must reflect that of the SD Card folder that is the top-level folder must be "01" with the MP3 files stored below it.

## Installing the RBNSpyBox software

Once construction is complete as USB cable is required to join the ESP32 DevKitC board to a USB socket on your PC. 
Plug the device in.
You may get a pop-up message identifying the new associated COM port. If not then the Windows Device Manager can be
used to confirm the associated COM port.
Open the Arduino IDE and then open the RBNSpyBox.ino project file. Navigate to Tools > Port and select the new COM port.

Next, we will populate the ESP32 SPIFFS area with the HTML pages. Select Tools > "ESP32 Sketch Data Upload". 
When the IDE window says "Connecting" press the button for a couple of seconds and release. You should more progress
in the IDE window and it should complete.

Next, we will compile and upload the software. Click the right pointing arrow at the top of the IDE under the word "Edit".
The compilation may take a minute or two. Again, look out for the "Connecting", press the button for a couple of seconds and release.

If successful, the 20 x 4 display should eventually now read:

<code>
Connect to:
      RBNSpyBox
and browse to:
http://192.168.4.1
</code>

## Configuration

To configure the RBNSpyBox you will need a PC, tablet, phone, or other Wi-Fi enabled device. This device
should be connected to the Wi-Fi access point RBNSpyBox, no password required. Once connected open a browser
and navigate to the URL http://192.168.4.1

At the top of the page you will see a series of buttons, click the "Settings" button.
If prompted by a dialog saying "No SSIDS visible", dismiss it. You will need to populate
the boxes labelles "WiFi SSID:" and "WiFi Password:" with those of your Wi-Fi network.
You will also need to provide your callsign, this will be used to login to the Reverse Beacon Network.
Once complete click "Update".

The RBNSpyBox should now connect to the configured Wi-Fi network and start processing data.

Click on the "Help" button at the top of the page for more help.
