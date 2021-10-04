#include "PMSReader.h"

PMSReader::PMSReader(){}

void PMSReader::readPMS(WebServer* server,int pm2p5value){
  String pm2p5asString = String(pm2p5value);
  // Serial.println(pm2p5asString);
  server->send(200,"text/plain", pm2p5asString);
}