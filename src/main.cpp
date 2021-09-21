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
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <config.h>

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
const char* ssid = SSID;
const char* password = PASSWORD;

WebServer server(80);

const int led = 13;
int pm2p5value = 0;

//DHT variable
#define DHTPIN 25
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//waitFiveMins variables
unsigned long previousMillis = 0; 
const long interval = 300000;

//API variable
const char* API_URL = "https://staging.revolvair.org/api/revolvair/stations/nicolasfelix/measures";
const char* API_TOKEN = "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJhdWQiOiIxIiwianRpIjoiZGEyZTg0ZGVlYzAwZGM5MTA2ODMxMzY1OTM3YjgxOWMzNjliOGNjYTg5NTQ4NjlmMTQ3ZWI2MTllZGU0ZGI5MTcwMGIzMTI2NzZlNzRhZWQiLCJpYXQiOjE2MzIyNDgyNTIsIm5iZiI6MTYzMjI0ODI1MiwiZXhwIjoxNjYzNzg0MjUyLCJzdWIiOiIzNSIsInNjb3BlcyI6W119.HGqZ5HdAaFHq_GDdC4kzkwDURbc6KgFWxWIkrNI8gWExh_3JiwHvo2y8YI-kFbPWd_SS3luBOTY25kma6-ZHow3OeiH4RKPOKRHw1ZLf1tPscQfVU50og02oaDkkZlUwqS2MmpxbIgW5DAa_E6GiNGX6mfcLnT0Ehvngc6IfZcqrynrTkSRIUsMdgTGxLzqhxnNhxl9YCsm9KSvAws4uRGgKYt5lz48Rha1nl34mV-a3h8H2lMEkcYrgp0w5i-mqV1jxLTLUAJZJx_VpQEbHUHHya_MUMTZvizCKUR0wYnanzTLqdDRm8Pc9EQOyIcOB6La8GJvW-eTnZ7cN5_K3iTAX9OoWw1JNN2U78J5BWmHkoG0A3ekJ-A6tnWE7EeMXfeEhneOD7FlHrZeHmq5G8q4qFHQ6MgmmdrzcT5_Ufy6-AOcVOowDy6JieZeCze0IbB8dRenMDXDA9BCS6oj7BdKarzrDtPqyW1O9sPOYoyirloWskTAfvOQjWMUP_GEdw6hm5QOXBPOWdk6Ut__l5yJ1JhXtr1Iazoq6UkuJ69_mAmaqb52-bxX0E0r4DcGyqSEEA_DUHJdsXNniwmofMc9Vyl0ygvEJUzAuhPZV159cihKPIVcC13t32PzIYUsWo7LknUQnu6j8pKgo8AR7JHN5X1T5NhJo0e5ECWCWvA8";

void handleADC(){
  int a = analogRead(A0);
  a = map(a,0,1023,0,100);
  String adc = String(a);
  Serial.println(adc);
  server.send(200, "text/plain",adc);
}
void readPMS(){
  String pm2p5asString = String(pm2p5value);
  // Serial.println(pm2p5asString);
  server.send(200,"text/plain", pm2p5asString);
}

void handleRoot() {
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plain","");
}
bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.html";
  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
   else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) {
  dataType = "application/octet-stream";
 }
 if (server.streamFile(dataFile, dataType) != dataFile.size()) { }
 dataFile.close();
 return true;
}
void handleWebRequests(){
  if(loadFromSpiffs(server.uri())) { 
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

void postJSON(String& encodedJSON) {
    HTTPClient http;
    http.begin(API_URL);
    http.addHeader("Accept", "application/json");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(API_TOKEN));
    int httpCode = http.POST(encodedJSON);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
}

void sendPM25ValueToAPI(){
  DynamicJsonDocument doc(1024);
  doc["value"]  = pm2p5value;
  doc["unit"]   = "pm25_raw";
  String jsonPm25Package = "";
  serializeJson(doc, jsonPm25Package);
  postJSON(jsonPm25Package);  
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
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //Initialize Webserver
  server.on("/",handleRoot);
  server.on("/getADC",handleADC); //Reads ADC function is called from out index.html
  server.on("/readPMS",readPMS);
  server.onNotFound(handleWebRequests); //Set setver all paths are not found so we can handle as per URI
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
    if (pms.read(data))
    {
      delay(5000);
      pm2p5value = data.PM_AE_UG_2_5;
      Serial.print("PM 2.5 (ug/m3): ");
      Serial.println(data.PM_AE_UG_2_5);
      float h = dht.readHumidity();
      Serial.println("Humidity:");
      Serial.println(h);
      float t = dht.readTemperature();
      Serial.println("Temperature:");
      Serial.println(t);
      if(data.PM_AE_UG_2_5 <= 11){
        ledcWrite(1, 0);
        ledcWrite(2, 255); //GREEN
        ledcWrite(3, 0);
      } else if(data.PM_AE_UG_2_5 <= 34){
        ledcWrite(1, 255); 
        ledcWrite(2, 130); //YELLOW
        ledcWrite(3, 0);
      } else if(data.PM_AE_UG_2_5 <= 54){
        ledcWrite(1, 255);
        ledcWrite(2, 90); //Orange
        ledcWrite(3, 0); 
      } else{
        ledcWrite(1, 255);
        ledcWrite(2, 0); //RED
        ledcWrite(3, 0);
      }
    }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendPM25ValueToAPI();
  }
}



