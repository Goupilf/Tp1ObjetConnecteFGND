#pragma once
#include <Arduino.h>
#include <FS.h>
#include <Spiffs.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClient.h>
class PMSReader
{
public:
    PMSReader();
    void readPMS(WebServer* server,int pm2p5vlue);
    
private:
    
};