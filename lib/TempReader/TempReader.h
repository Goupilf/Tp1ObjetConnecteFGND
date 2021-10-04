#pragma once
#include <WebServer.h>

class TempReader
{
public:
    TempReader();
    void readTemp(WebServer* server,float tempVal,float humVal);
    
private:
    
};

