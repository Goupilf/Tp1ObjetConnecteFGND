#include "RevolairAPI.h"

RevolairAPI::RevolairAPI(const char* API_URL,const char* API_TOKEN){
    this->API_URL = API_URL;
    this->API_TOKEN = API_TOKEN;
}
void RevolairAPI::postJSON(String& encodedJSON) {
    HTTPClient http;
    http.begin(String(this->API_URL));
    http.addHeader("Accept", "application/json");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + String(this->API_TOKEN));
    int httpCode = http.POST(encodedJSON);
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    http.end();
}

void RevolairAPI::sendPM25ValueToAPI(int pm2p5value){
  DynamicJsonDocument doc(1024);
  doc["value"]  = pm2p5value;
  doc["unit"]   = "pm25_raw";
  String jsonPm25Package = "";
  serializeJson(doc, jsonPm25Package);
  postJSON(jsonPm25Package);  
}