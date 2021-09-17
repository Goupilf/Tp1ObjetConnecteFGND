#include <Arduino.h>
#include <Spiffs.h>
#include <FS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <PMS.h>   

PMS pms(Serial2); //  R32 : IO16
PMS::DATA data;

#include <config.h>

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

void handleADC(){
  int a = analogRead(A0);
  a = map(a,0,1023,0,100);
  String adc = String(a);
  Serial.println(adc);
  server.send(200, "text/plane",adc);
}

void handleRoot() {
  server.sendHeader("Location", "/index.html",true);   //Redirect to our html web page
  server.send(302, "text/plane","");
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

void setup(void) {
  delay(1000);
  Serial.begin(9600);  // Moniteur série pour l’affichage (println)
  Serial2.begin(9600); // Port de communication série avec le capteur PMS 
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
      Serial.println();
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
}



