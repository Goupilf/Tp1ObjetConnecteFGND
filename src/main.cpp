#include <Arduino.h>
#include <Spiffs.h>
#include <FS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <PMS.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <../lib/TempReader/TempReader.h>
#include <../lib/FlashFileReader/FlashFileReader.h>
#include <../lib/PMSReader/PMSReader.h>
#include <../lib/RBGLedManager/RBGLedManager.h>
#include <../lib/RevolairAPI/RevolairAPI.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <config.h>

//char api_url = *API_URL;
FlashFileReader flashFileReader;
PMSReader pmsReader;
RGBLedManager rgbLedManager;
RevolairAPI revolvairAPI = RevolairAPI( API_URL,API_TOKEN);

PMS pms(Serial2); //  R32 : IO16
PMS::DATA data;

//Configuration for RGB led
uint8_t ledR = 12;  
uint8_t ledG = 13;
uint8_t ledB = 14; 

uint8_t ledArray[3] = {1, 2, 3}; 
const boolean invert = true;     // set true if common anode, false if common cathode

uint8_t color = 0;          // a value from 0 to 255 representing the hue
uint32_t R, G, B;           // the Red Green and Blue color components
uint8_t brightness = 255;

//See configuration.h file

WebServer server(80);

const int led = 13;
int pm2p5value = 0;

//DHT variable
#define DHTPIN 25
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void handleTemp() {
  server.sendHeader("Location", "/TempData.html",true);   //Redirect to our html web page
  server.send(302, "text/plain","");
}
void handleWifi() {
  server.sendHeader("Location", "/WifiData.html",true);   //Redirect to our html web page
  server.send(302, "text/plain","");
}
void readTemp() {
  String temp = String( dht.readTemperature());
  String hum = String( dht.readHumidity());
  server.send(200, "text/plain",temp + "," + hum );// trouver dans tempData.html facon de separer pour afficher correctement
}
void readWifi() {
  
  server.send(200, "text/plain",SSID );// 
}

//waitThreeMins variables
unsigned long previousMillis1 = 0; 
const long interval1 = 180000;

//waitFiveSeconds variables
unsigned long previousMillis2 = 0; 
const long interval2 = 5000;

//API variable


void readPMS(){
  pmsReader.readPMS(&server,pm2p5value);
}

void handleRoot() {
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plain","");
}

void handleWebRequests(){
  if(flashFileReader.loadFromSpiffs(server.uri(), &server)) { 
    return; 
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}



void setup(void) {
  delay(1000);
  Serial.begin(9600);  // Moniteur série pour l’affichage (println)
  Serial2.begin(9600); // Port de communication série avec le capteur PMS 
  Serial1.begin(9600);
  delay(2000);


  //Set up led
  ledcAttachPin(ledR, 1); // assign RGB led pins to channels
  ledcAttachPin(ledG, 2);
  ledcAttachPin(ledB, 3);
  // Initialize channels 
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  ledcSetup(3, 12000, 8);

  //Initialize File System
  SPIFFS.begin();
  Serial.println("File System Initialized");

  dht.begin();
  Serial.println("DHT initialized");
  
  //Connect to wifi Network
  WiFi.begin(SSID, PASSWORD);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //HotSpot starting
  WiFi.softAP(AP_SSID, AP_PASSWORD);  
  IPAddress myIP = WiFi.softAPIP();  
  Serial.print("AP IP address: ");  
  Serial.println(myIP);

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //Initialize Webserver
  server.on("/",handleRoot);
  server.on("/tempPage",handleTemp);
  server.on("/wifiPage",handleWifi);
  server.on("/readPMS",readPMS);
  server.on("/readTemp",readTemp);
  server.on("/readWifi",readWifi);

  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
    if (pms.read(data))
    {
      unsigned long currentMillis2 = millis();
      if (currentMillis2 - previousMillis2 >= interval2) {
        previousMillis2 = currentMillis2;
        pm2p5value = data.PM_AE_UG_2_5;
        Serial.print("PM 2.5 (ug/m3): ");
        Serial.println(data.PM_AE_UG_2_5);
        float h = dht.readHumidity();
        Serial.println("Humidity:");
        Serial.println(h);
        float t = dht.readTemperature();
        Serial.println("Temperature:");
        Serial.println(t);
        rgbLedManager.handleLed(data.PM_AE_UG_2_5);
        
      }
    }
  unsigned long currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 >= interval1) {
    previousMillis1 = currentMillis1;
    revolvairAPI.sendPM25ValueToAPI(pm2p5value);
  }
}



