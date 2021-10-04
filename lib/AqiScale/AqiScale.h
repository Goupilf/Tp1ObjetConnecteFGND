#pragma once

#include <Arduino.h>

class AqiScale
{
public:
    AqiScale();
    String getAqi( int pmsVal);
    String getAqiLabel(int pmsVal);
    
private:
    
};