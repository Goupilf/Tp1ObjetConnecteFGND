#include "FlashFileReader.h"

FlashFileReader::FlashFileReader(){}

bool FlashFileReader::loadFromSpiffs(String path,WebServer* server) {
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
  if (server->hasArg("download")) {
  dataType = "application/octet-stream";
 }
 if (server->streamFile(dataFile, dataType) != dataFile.size()) { }
 dataFile.close();
 return true;
}