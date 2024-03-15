# Penn State IEEE
## Spotify Controller Workshop
Occured on March 16, 2024

### About
Participants created a desktop Spotify Controller with media playback controls and a screen to show currently playing media
The device uses REST API calls to communicate with one's spotify account to send and receive data.

#### Soldering
1. Buttons
2. ESP8266 with spacers
	- USB goes face down on the same side as the IEEE logo
3. Display header
    - Bend pins connected to display ata  45 degree angle to improve viewing experience
4. Pot

#### Arduino code setup
1. Make sure that you have arduino IDE installed.
2. Download the code package from the repository.
4. Open the spotify_buddy.ino file from the spotify_buddy folder in arduino IDE.

#### Display
1. In arduino IDE add TFT_eSPI, TJpg_Decoder, and ArduinoJson libraries.
2. In a windows explorer navigate to "Documents\\Arduino\\libraries\\TFT_eSPI" and edit User_Setup_Select.h. 
	1. 2a. For MacOS navigate in Finder to ~/Documents/Arduino/libraries/TFT_eSPI.
3. Comment the default library adding "//" in front of the #, then uncomment #include <User_Setups/Setup2_ST7735.h>. Save and exit.
4. In a windows explorer navigate to "Documents\\Arduino\\libraries\\TFT_eSPI" and edit User_Setup.h. 
5. Replace "#define ST7735_REDTAB" with "#define ST7735_GREENTAB" this identifier could differ depending on the display used. Save and exit.
7. Now navigate to "Documents\Arduino\libraries\TFT_eSPI\User_Setups" and edit Setup2_ST7735.h.
8. Replace "#define ST7735_REDTAB" with "#define ST7735_GREENTAB" this identifier could differ depending on the display used. Save and exit.
6. Uncomment "#define TFT_RGB_ORDER TFT_RGB"

#### Developer Account
1. Go to [https://developer.spotify.com/documentation/web-api](https://developer.spotify.com/documentation/web-api) and follow the instructions there to create an app.
	1. Log into the [dashboard](https://developer.spotify.com/dashboard) using your Spotify account.
	2. [Create an app](https://developer.spotify.com/documentation/web-api/concepts/apps)Once you have created your app, you will have access to the app credentials. These will be required for API [authorization](https://developer.spotify.com/documentation/web-api/concepts/authorization) to obtain an [access token](https://developer.spotify.com/documentation/web-api/concepts/access-token).
		1. Name: IEEE Spotify Controller
		2. Description: IEEE Spotify Controller
		3. Website: blank
		4. Redirect URI: 1.1.1.1 (Temporary and replaced once launched for the first time)
		5. APIs planning to use: web API
	3. Use the [access token](https://developer.spotify.com/documentation/web-api/concepts/access-token) in your [API requests](https://developer.spotify.com/documentation/web-api/concepts/api-calls).
3. In the Spotify app settings there are client_ID as well as client_Secret, copy these to line 62 and 63 in the spotify_buddy.ino.
4. In the spotify_buddy.ino file edit lines 58 and 59 to your WIFI_SSID and PASSWORD. Note, SSIDs are case sensitive.
5. Plug in your assembled spotify buddy and in Arduino IDE select the port and board used. 5a. You may need to include an additional boards manager URL in arduino IDE. 5b. Go to file, preferences and at the bottom of the popup add "[http://arduino.esp8266.com/stable/package_esp8266com_index.json](http://arduino.esp8266.com/stable/package_esp8266com_index.json)"
6. In arduino IDE on the top bar go to tools, cpu frequency and select 160 MHz.
7. Push the code to your spotify buddy by clicking the arrow in the top left.
8. Once the spotify buddy has rebooted it will display its' personal IP. In arduino IDE edit line 64 and replace "YOUR_ESP_IP" with the displayed IP.
9. Now go to [https://developer.spotify.com/dashboard](https://developer.spotify.com/dashboard) select your app and edit the callback URI to be the same as line 64 of the spotify buddy code.
10. Push the code once more in arduino IDE and once your spotify buddy reboots navigate in a web browser to the IP displayed and follow the instructions.
11. Enjoy your spotify buddy!
