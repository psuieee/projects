# Penn State IEEE
## Spotify Controller Workshop
Occured on March 16, 2024

### About
Participants created a desktop Spotify Controller with media playback controls and a screen to show currently playing media
The device uses REST API calls to communicate with one's spotify account to send and receive data.

#### Preqreuisites
1. [Arduino IDE](https://www.arduino.cc/en/software)
	1. Install Legacy version 1.8.19
2. [Visual Studio Code](https://code.visualstudio.com/download)
3. [Spotify Account](https://open.spotify.com/)
4. Windows only: [ESP8266 Driver](https://www.wemos.cc/en/latest/ch340_driver.html)
	1. Download the zip folder, then install the driver inside of it

#### Components
- 1x PCB
- 4x Push buttons (2 pin)
- 1x Potentiometer
- 2x Short male header pin sections (for Arduino)
- 2x Header pin spacers
- 1x 1.8 inch TFT Display
- 1x Socket header pin section (for display)

#### Soldering
1. Buttons
2. ESP8266 with spacers between bottom of header pin plastic and the top of PCB
	- USB goes face down on the same side as the IEEE logo
3. Display header
    - Bend pins connected to display at a 45 degree angle to improve viewing experience before inserting
4. Potentiometer

#### Arduino code setup
1. Download the code package from the repository.
2. Open the spotify_buddy.ino file from the spotify_buddy folder in arduino IDE.
3. Install the TFT_eSPI, TJpg_Decoder, and ArduinoJson libraries.

#### Display
1. In a file explorer navigate to `Documents\\Arduino\\libraries` on Windows or `~/Documents/Arduino/libraries` on Mac
2. Replace the TFT_eSPI folder with the one downloaded from this repository

   **OR** (Only do these steps if the above did not work)
   
1. In a windows explorer navigate to `Documents\\Arduino\\libraries\\TFT_eSPI` and edit `User_Setup_Select.h`. 
	1. Comment the default library adding `//` in front of the `#`, then uncomment `#include <User_Setups/Setup2_ST7735.h>`. Save and exit.
2. In a windows explorer navigate to `Documents\\Arduino\\libraries\\TFT_eSPI` and edit `User_Setup.h`. 
	1. Replace `#define ST7735_REDTAB` with `#define ST7735_GREENTAB`. Save and exit.
3. Now navigate to `Documents\Arduino\libraries\TFT_eSPI\User_Setups` and edit `Setup2_ST7735.h`.
	1. Replace `#define ST7735_REDTAB` with `#define ST7735_GREENTAB`.
	2. Uncomment `#define TFT_RGB_ORDER TFT_RGB`. Save and exit.

#### Spotify Developer Account
1. Go to [https://developer.spotify.com/documentation/web-api](https://developer.spotify.com/documentation/web-api) and follow the instructions there to create an app.
	1. Log into the [dashboard](https://developer.spotify.com/dashboard) using your Spotify account.
	2. [Create an app](https://developer.spotify.com/documentation/web-api/concepts/apps). Once you have created your app, you will have access to the app credentials. These will be required for API [authorization](https://developer.spotify.com/documentation/web-api/concepts/authorization) to obtain an [access token](https://developer.spotify.com/documentation/web-api/concepts/access-token).
		1. Name: IEEE Spotify Controller
		2. Description: IEEE Spotify Controller
		3. Website: blank
		4. Redirect URI: 1.1.1.1 (Temporary and replaced once launched for the first time)
		5. APIs planning to use: web API
	3. Use the [access token](https://developer.spotify.com/documentation/web-api/concepts/access-token) in your [API requests](https://developer.spotify.com/documentation/web-api/concepts/api-calls).
2. In the Spotify app settings there are client_ID as well as client_Secret, copy these to line 62 and 63 in the spotify_buddy.ino.
3. In the spotify_buddy.ino file edit lines 58 and 59 to your WIFI_SSID and PASSWORD. Note, SSIDs are case sensitive.
	1. During the workshop, use your mobile hotspot SSID and password as the device cannot easily connect to PSU wifi
 	2. Once home, replace these with your home SSID and password, then reupload the code
4. Plug in your assembled spotify buddy and in Arduino IDE select the port and board used. 
 	1. You may need to include an additional boards manager URL in arduino IDE. Go to file, preferences and at the bottom of the popup add "[http://arduino.esp8266.com/stable/package_esp8266com_index.json](http://arduino.esp8266.com/stable/package_esp8266com_index.json)"
5. In arduino IDE on the top bar go to tools, cpu frequency and select 160 MHz.
6. Push the code to your spotify buddy by clicking the arrow in the top left.
7. Once the spotify buddy has rebooted it will display its' personal IP. In arduino IDE edit line 64 and replace "YOUR_ESP_IP" with the displayed IP.
8. Now go to [https://developer.spotify.com/dashboard](https://developer.spotify.com/dashboard) select your app and edit the callback URI to be the same as line 64 of the spotify buddy code.
9. Push the code once more in arduino IDE and once your spotify buddy reboots navigate in a web browser to the IP displayed and follow the instructions to log in.
10. Enjoy your spotify buddy!
