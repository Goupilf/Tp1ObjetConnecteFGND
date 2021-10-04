#include "TempReader.h"


TempReader::TempReader(){
    
}
void TempReader::readTemp(WebServer* server,float tempVal,float humVal){
  String temp = String( tempVal);
  String hum = String( tempVal);
  server->send(200, "text/plain",temp + "," + hum );// trouver dans tempData.html facon de separer pour afficher correctement

}

