
/*========================================
|Include all libraries needed for program|
========================================*/

// Include the jpeg decoder library
#include <TJpg_Decoder.h> 

// Include SPIFFS
#include <FS.h> 

//Include JSON
#include <ArduinoJson.h>

//Include base 64 encode
#include <base64.h>

// Include WiFi and http client
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecureBearSSL.h>

// Load tabs attached to this sketch
#include "List_SPIFFS.h"
#include "Web_Fetch.h"
#include "index.h"

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"
#include <TFT_eSPI.h>   

TFT_eSPI tft = TFT_eSPI();         // Invoke custom library
int imageOffsetX = 26, imageOffsetY = 20;
// TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

// // This next function will be called during decoding of the jpeg file to
// // render each block to the TFT.  If you use a different TFT library
// // you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);
 


  // Return 1 to decode next block
  return 1;
}

/*=========================
|User modifiable variables|
=========================*/
// WiFi credentials
#define WIFI_SSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"

// Spotify API credentials
#define CLIENT_ID "YOUR_CLIENT_ID"
#define CLIENT_SECRET "YOUR_CLIENT_SECRET"
#define REDIRECT_URI "http://YOUR_ESP_IP/callback"

/*=========================
|Non - modifiable variables|
==========================*/

String getValue(HTTPClient &http, String key) {
  bool found = false, look = false, seek = true;
  int ind = 0;
  String ret_str = "";

  int len = http.getSize();
  char char_buff[1];
  WiFiClient * stream = http.getStreamPtr();
  while (http.connected() && (len > 0 || len == -1)) {
    size_t size = stream->available();
    // Serial.print("Size: ");
    // Serial.println(size);
    if (size) {
      int c = stream->readBytes(char_buff, ((size > sizeof(char_buff)) ? sizeof(char_buff) : size));
      if (found) {
        if (seek && char_buff[0] != ':') {
          continue;
        } else if(char_buff[0] != '\n'){
            if(seek && char_buff[0] == ':'){
                seek = false;
                int c = stream->readBytes(char_buff, 1);
            }else{
                ret_str += char_buff[0];
            }
        }else{
            break;
        }
          
        // Serial.print("get: ");
        // Serial.println(get);
      }
      else if ((!look) && (char_buff[0] == key[0])) {
        look = true;
        ind = 1;
      } else if (look && (char_buff[0] == key[ind])) {
        ind ++;
        if (ind == key.length()) found = true;
      } else if (look && (char_buff[0] != key[ind])) {
        ind = 0;
        look = false;
      }
    }
  }
//   Serial.println(*(ret_str.end()));
//   Serial.println(*(ret_str.end()-1));
//   Serial.println(*(ret_str.end()-2));
  if(*(ret_str.end()-1) == ','){
    ret_str = ret_str.substring(0,ret_str.length()-1);
  }
  return ret_str;
}

//http response struct
struct httpResponse{
    int responseCode;
    String responseMessage;
};

struct songDetails{
    int durationMs;
    String album;
    String artist;
    String song;
    String Id;
    bool isLiked;
};

char *parts[10];

void printSplitString(String text,int maxLineSize, int yPos)
{
    int currentWordStart = 0;
    int spacedCounter = 0;
    int spaceIndex = text.indexOf(" ");
    
    while(spaceIndex != -1){
        // Serial.println(ESP.getFreeHeap());
        char *part = parts[spacedCounter]; 
        sprintf(part,text.substring(currentWordStart,spaceIndex).c_str());
        // Serial.println(ESP.getFreeHeap());
        // parts[spacedCounter] = part;
        currentWordStart = spaceIndex;
        spacedCounter++;
        spaceIndex = text.indexOf(" ",spaceIndex+1);
    }
    // Serial.println(ESP.getFreeHeap());
    char *part = parts[spacedCounter]; 
    sprintf(part,text.substring(currentWordStart,text.length()).c_str());
    // Serial.println(ESP.getFreeHeap());
    currentWordStart = spaceIndex;
    size_t counter = 0;
    currentWordStart = 0;
    while(counter <= spacedCounter){
        char printable[maxLineSize];
        char* printablePointer = printable;
        // sprintf in word at counter always
        sprintf(printablePointer,parts[counter]);
        //get length of first word
        int currentLen = 0;
        while(parts[counter][currentLen] != '\0'){
            currentLen++;
            printablePointer++;
        }
        counter++;
        while(counter <= spacedCounter){
            int nextLen = 0;
            while(parts[counter][nextLen] != '\0'){
                nextLen++;
            }
            if(currentLen + nextLen > maxLineSize)
                break;
            sprintf(printablePointer, parts[counter]);
            currentLen += nextLen;
            printablePointer += nextLen;
            counter++;
        }
        String output = String(printable);
        if(output[0] == ' ')
            output = output.substring(1);
        // Serial.println(output);
        tft.setCursor((int)(tft.width()/2 - tft.textWidth(output) / 2),tft.getCursorY());
        tft.println(output);
        // free(printable);
    }
    // Serial.println(ESP.getFreeHeap());
}

//Create spotify connection class
class SpotConn {
public:
	SpotConn(){
        client = std::make_unique<BearSSL::WiFiClientSecure>();
        client->setInsecure();
    }
    // httpResponse makeSpotifyRequest(const char* URI, const char** headers, int numHeaders, const char* RequestBody){
    //     https.begin(*client,URI);
    //     for(;numHeaders>0;numHeaders--,headers += 2){
    //         https.addHeader(*headers,*(headers+1));
    //     }
    //     struct httpResponse res;
    //     res.responseCode = https.POST(RequestBody);
    //     res.responseMessage = https.getString()
    //     https.end();
    //     return res;
    // }
	bool getUserCode(String serverCode) {
        https.begin(*client,"https://accounts.spotify.com/api/token");
        String auth = "Basic " + base64::encode(String(CLIENT_ID) + ":" + String(CLIENT_SECRET));
        https.addHeader("Authorization",auth);
        https.addHeader("Content-Type","application/x-www-form-urlencoded");
        String requestBody = "grant_type=authorization_code&code="+serverCode+"&redirect_uri="+String(REDIRECT_URI);
        // Send the POST request to the Spotify API
        int httpResponseCode = https.POST(requestBody);
        // Check if the request was successful
        if (httpResponseCode == HTTP_CODE_OK) {
            String response = https.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);
            accessToken = String((const char*)doc["access_token"]);
            refreshToken = String((const char*)doc["refresh_token"]);
            tokenExpireTime = doc["expires_in"];
            tokenStartTime = millis();
            accessTokenSet = true;
            Serial.println(accessToken);
            Serial.println(refreshToken);
        }else{
            Serial.println(https.getString());
        }
        // Disconnect from the Spotify API
        https.end();
        return accessTokenSet;
    }
    bool refreshAuth(){
        https.begin(*client,"https://accounts.spotify.com/api/token");
        String auth = "Basic " + base64::encode(String(CLIENT_ID) + ":" + String(CLIENT_SECRET));
        https.addHeader("Authorization",auth);
        https.addHeader("Content-Type","application/x-www-form-urlencoded");
        String requestBody = "grant_type=refresh_token&refresh_token="+String(refreshToken);
        // Send the POST request to the Spotify API
        int httpResponseCode = https.POST(requestBody);
        accessTokenSet = false;
        // Check if the request was successful
        if (httpResponseCode == HTTP_CODE_OK) {
            String response = https.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);
            accessToken = String((const char*)doc["access_token"]);
            // refreshToken = doc["refresh_token"];
            tokenExpireTime = doc["expires_in"];
            tokenStartTime = millis();
            accessTokenSet = true;
            Serial.println(accessToken);
            Serial.println(refreshToken);
        }else{
            Serial.println(https.getString());
        }
        // Disconnect from the Spotify API
        https.end();
        return accessTokenSet;
    }
    bool getTrackInfo(){
        String url = "https://api.spotify.com/v1/me/player/currently-playing";
        https.useHTTP10(true);
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        int httpResponseCode = https.GET();
        bool success = false;
        String songId = "";
        bool refresh = false;
        // Check if the request was successful
        if (httpResponseCode == 200) {
                        // 

            String currentSongProgress = getValue(https,"progress_ms");
            currentSongPositionMs = currentSongProgress.toFloat();
            String imageLink = "";
            while(imageLink.indexOf("image") == -1){
                String height = getValue(https,"height");
                // Serial.println(height);
                if(height.toInt() > 300){
                    imageLink = "";
                    continue;
                }
                imageLink = getValue(https, "url");
                
                // Serial.println(imageLink);
            }
            // Serial.println(imageLink);
            
            
            String albumName = getValue(https,"name");
            String artistName = getValue(https,"name");
            String songDuration = getValue(https,"duration_ms");
            currentSong.durationMs = songDuration.toInt();
            String songName = getValue(https,"name");
            songId = getValue(https,"uri");
            String isPlay = getValue(https, "is_playing");
            isPlaying = isPlay == "true";
            Serial.println(isPlay);
            // Serial.println(songId);
            songId = songId.substring(15,songId.length()-1);
            // Serial.println(songId);
            https.end();
            // listSPIFFS();
            if (songId != currentSong.Id){
                
                if(SPIFFS.exists("/albumArt.jpg") == true) {
                    SPIFFS.remove("/albumArt.jpg");
                }
                // Serial.println("trying to get album art");
                bool loaded_ok = getFile(imageLink.substring(1,imageLink.length()-1).c_str(), "/albumArt.jpg"); // Note name preceded with "/"
                Serial.println("Image load was: ");
                Serial.println(loaded_ok);
                refresh = true;
                tft.fillScreen(TFT_BLACK);
            }
            currentSong.album = albumName.substring(1,albumName.length()-1);
            currentSong.artist = artistName.substring(1,artistName.length()-1);
            currentSong.song = songName.substring(1,songName.length()-1);
            currentSong.Id = songId;
            currentSong.isLiked = findLikedStatus(songId);
            success = true;
        } else {
            Serial.print("Error getting track info: ");
            Serial.println(httpResponseCode);
            // String response = https.getString();
            // Serial.println(response);
            https.end();
        }
        
        
        // Disconnect from the Spotify API
        if(success){
            drawScreen(refresh);
            lastSongPositionMs = currentSongPositionMs;
        }
        return success;
    }
    bool findLikedStatus(String songId){
        String url = "https://api.spotify.com/v1/me/tracks/contains?ids="+songId;
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        https.addHeader("Content-Type","application/json");
        int httpResponseCode = https.GET();
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 200) {
            String response = https.getString();
            https.end();
            return(response == "[ true ]");
        } else {
            Serial.print("Error toggling liked songs: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
            https.end();
        }

        
        // Disconnect from the Spotify API
        
        return success;
    }
    bool toggleLiked(String songId){
        String url = "https://api.spotify.com/v1/me/tracks/contains?ids="+songId;
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        https.addHeader("Content-Type","application/json");
        int httpResponseCode = https.GET();
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 200) {
            String response = https.getString();
            https.end();
            if(response == "[ true ]"){
                currentSong.isLiked = false;
                dislikeSong(songId);
            }else{
                currentSong.isLiked = true;
                likeSong(songId);
            }
            drawScreen(false,true);
            Serial.println(response);
            success = true;
        } else {
            Serial.print("Error toggling liked songs: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
            https.end();
        }

        
        // Disconnect from the Spotify API
        
        return success;
    }
    bool drawScreen(bool fullRefresh = false, bool likeRefresh = false){
        int rectWidth = 120;
        int rectHeight = 10;
        if(fullRefresh){
            if (SPIFFS.exists("/albumArt.jpg") == true) { 
                TJpgDec.setSwapBytes(true);
                TJpgDec.setJpgScale(4);
                TJpgDec.drawFsJpg(26, 5, "/albumArt.jpg");
            }else{
                TJpgDec.setSwapBytes(false);
                TJpgDec.setJpgScale(1);
                TJpgDec.drawFsJpg(0, 0, "/Angry.jpg");
            }
            tft.setTextDatum(MC_DATUM);
            tft.setTextWrap(true);
            tft.setCursor(0,85);
            printSplitString(currentSong.artist,20,85);
            // tft.drawString(currentSong.artist, tft.width() / 2, 10);
            tft.setCursor(0,110);

            printSplitString(currentSong.song,20,110);
            // tft.print(currentSong.song);
            // tft.drawString(currentSong.song, tft.width() / 2, 115);
            // tft.drawString(currentSong.song, tft.width() / 2, 125);
            
            tft.drawRoundRect(
                tft.width() / 2 - rectWidth / 2,
                140,
                rectWidth,
                rectHeight,
                4,
                TFT_DARKGREEN);
        }
        if(fullRefresh || likeRefresh){
            if(currentSong.isLiked){
                TJpgDec.setJpgScale(1);
                TJpgDec.drawFsJpg(128-20, 0, "/heart.jpg");
            //    tft.fillCircle(128-10,10,10,TFT_GREEN);
            }else{
                tft.fillRect(128-21,0,21,21,TFT_BLACK);
            }
        }
        if(lastSongPositionMs > currentSongPositionMs){
            tft.fillSmoothRoundRect(
                tft.width() / 2 - rectWidth / 2 + 2,
                140 + 2,
                rectWidth  - 4,
                rectHeight - 4,
                10,
                TFT_BLACK
            );
            lastSongPositionMs = currentSongPositionMs;
        }
        tft.fillSmoothRoundRect(
            tft.width() / 2 - rectWidth / 2 + 2,
            140 + 2,
            rectWidth * (currentSongPositionMs/currentSong.durationMs) - 4,
            rectHeight - 4,
            10,
            TFT_GREEN
        );
        // Serial.println(currentSongPositionMs);
        // Serial.println(currentSong.durationMs);
        // Serial.println(currentSongPositionMs/currentSong.durationMs);
        return true;
    }
    bool togglePlay(){
        String url = "https://api.spotify.com/v1/me/player/" + String(isPlaying ? "pause" : "play");
        isPlaying = !isPlaying;
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        int httpResponseCode = https.PUT("");
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 204) {
            // String response = https.getString();
            Serial.println((isPlaying ? "Playing" : "Pausing"));
            success = true;
        } else {
            Serial.print("Error pausing or playing: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        getTrackInfo();
        return success;
    }
    bool adjustVolume(int vol){
        String url = "https://api.spotify.com/v1/me/player/volume?volume_percent=" + String(vol);
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        int httpResponseCode = https.PUT("");
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 204) {
            // String response = https.getString();
            currVol = vol;
            success = true;
        }else if(httpResponseCode == 403){
             currVol = vol;
            success = false;
            Serial.print("Error setting volume: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        } else {
            Serial.print("Error setting volume: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        return success;
    }
    bool skipForward(){
        String url = "https://api.spotify.com/v1/me/player/next";
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        int httpResponseCode = https.POST("");
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 204) {
            // String response = https.getString();
            Serial.println("skipping forward");
            success = true;
        } else {
            Serial.print("Error skipping forward: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        getTrackInfo();
        return success;
    }
    bool skipBack(){
        String url = "https://api.spotify.com/v1/me/player/previous";
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        int httpResponseCode = https.POST("");
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 204) {
            // String response = https.getString();
            Serial.println("skipping backward");
            success = true;
        } else {
            Serial.print("Error skipping backward: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        getTrackInfo();
        return success;
    }
    bool likeSong(String songId){
        String url = "https://api.spotify.com/v1/me/tracks?ids="+songId;
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        https.addHeader("Content-Type","application/json");
        char requestBody[] = "{\"ids\":[\"string\"]}";
        int httpResponseCode = https.PUT(requestBody);
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 200) {
            // String response = https.getString();
            Serial.println("added track to liked songs");
            success = true;
        } else {
            Serial.print("Error adding to liked songs: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        return success;
    }
    bool dislikeSong(String songId){
        String url = "https://api.spotify.com/v1/me/tracks?ids="+songId;
        https.begin(*client,url);
        String auth = "Bearer " + String(accessToken);
        https.addHeader("Authorization",auth);
        // https.addHeader("Content-Type","application/json");
        // char requestBody[] = "{\"ids\":[\"string\"]}";
        int httpResponseCode = https.DELETE();
        bool success = false;
        // Check if the request was successful
        if (httpResponseCode == 200) {
            // String response = https.getString();
            Serial.println("removed liked songs");
            success = true;
        } else {
            Serial.print("Error removing from liked songs: ");
            Serial.println(httpResponseCode);
            String response = https.getString();
            Serial.println(response);
        }

        
        // Disconnect from the Spotify API
        https.end();
        return success;
    }
    bool accessTokenSet = false;
    long tokenStartTime;
    int tokenExpireTime;
    songDetails currentSong;
    float currentSongPositionMs;
    float lastSongPositionMs;
    int currVol;
private:
    std::unique_ptr<BearSSL::WiFiClientSecure> client;
    HTTPClient https;
    bool isPlaying = false;
    String accessToken;
    String refreshToken;
};
//Vars for keys, play state, last song, etc.
bool buttonStates[] = {1,1,1,1};
int debounceDelay = 50;
unsigned long debounceTimes[] = {0,0,0,0};
int buttonPins[] = {D1,D2,D0,D6};
//Func to establish connection
//Func to refresh connection 
//Funcs for all api calls

//Create screen control class
//Show a face
//Show currently playing
//Show volume change

//Object instances
ESP8266WebServer server(80); //Server on port 80
SpotConn spotifyConnection;

//Web server callbacks
void handleRoot() {
    Serial.println("handling root");
    char page[500];
    sprintf(page,mainPage,CLIENT_ID,REDIRECT_URI);
    server.send(200, "text/html", String(page)+"\r\n"); //Send web page
}

void handleCallbackPage() {
    if(!spotifyConnection.accessTokenSet){
        if (server.arg("code") == ""){     //Parameter not found
            char page[500];
            sprintf(page,errorPage,CLIENT_ID,REDIRECT_URI);
            server.send(200, "text/html", String(page)); //Send web page
        }else{     //Parameter found
            if(spotifyConnection.getUserCode(server.arg("code"))){
                server.send(200,"text/html","Spotify setup complete Auth refresh in :"+String(spotifyConnection.tokenExpireTime));
            }else{
                char page[500];
                sprintf(page,errorPage,CLIENT_ID,REDIRECT_URI);
                server.send(200, "text/html", String(page)); //Send web page
            }
        }
    }else{
        server.send(200,"text/html","Spotify setup complete");
    }
}
long timeLoop;
long refreshLoop;
bool serverOn = true;
/*==============
|Setup function|
==============*/
void setup(){
    Serial.begin(115200);
    // delay(1000);
    // Initialise SPIFFS
    if (!SPIFFS.begin()) {
        Serial.println("SPIFFS initialisation failed!");
        while (1) yield(); // Stay here twiddling thumbs waiting
    }
    Serial.println("\r\nInitialisation done.");

    // Initialise the TFT
    tft.begin();
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(2);
    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
    TJpgDec.setJpgScale(4);

    // The byte order can be swapped (set true for TFT_eSPI)
    TJpgDec.setSwapBytes(true);

    // The decoder must be given the exact name of the rendering function above
    TJpgDec.setCallback(tft_output);

    WiFi.begin(WIFI_SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi\n Ip is: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);      //Which routine to handle at root location
    server.on("/callback", handleCallbackPage);      //Which routine to handle at root location
    server.begin();                  //Start server
    Serial.println("HTTP server started");
    for(int i = 0 ; i < 4; i++){
        pinMode(buttonPins[i],INPUT_PULLUP);
    }
    for(int i = 0 ; i < 10; i++){
        parts[i] = (char*)malloc(sizeof(char) * 20);
    }

    tft.println(WiFi.localIP());
}
// * Sets up WiFi
// * Shows Ip on screen
// * Goes through spotify API handshake (SpotConn func)
// * Initializes screen
// * Checks to see if anything is currently playing (SpotCon func)
// * Shows cute face if needed

void loop(){
    if(spotifyConnection.accessTokenSet){
        if(serverOn){
            server.close();
            serverOn = false;
        }
        if((millis() - spotifyConnection.tokenStartTime)/1000 > spotifyConnection.tokenExpireTime){
            Serial.println("refreshing token");
            if(spotifyConnection.refreshAuth()){
                Serial.println("refreshed token");
            }
        }
        if((millis() - refreshLoop) > 5000){
            spotifyConnection.getTrackInfo();
            
            // spotifyConnection.drawScreen();
            refreshLoop = millis();
        }
        for(int i = 0; i < 4; i ++){
            int reading = digitalRead(buttonPins[i]);
            if( reading != buttonStates[i]){
                if(millis() - debounceTimes[i] > debounceDelay){
                    buttonStates[i] = reading;
                    if(reading == LOW){
                        switch (i)
                        {
                        case 0:
                            spotifyConnection.togglePlay();
                            break;
                        case 1:
                            spotifyConnection.toggleLiked(spotifyConnection.currentSong.Id);
                            break;
                        case 2:
                            spotifyConnection.skipForward();
                            break;
                        case 3:
                            spotifyConnection.skipBack();
                            break;
                        
                        default:
                            break;
                        }
                    }
                    debounceTimes[i] = millis();
                }
            }
        }
        
        int volRequest = map(analogRead(A0),0,1023,0,100);
        if(abs(volRequest - spotifyConnection.currVol) > 2){
            spotifyConnection.adjustVolume(volRequest);
        }
        timeLoop = millis();
    }else{
        server.handleClient();
    }
    // Serial.println(millis() - timeLoop);
    // timeLoop = millis();
}
//Loop function
// *Check for spotify connection refresh
// *Check inputs
// *update screen
