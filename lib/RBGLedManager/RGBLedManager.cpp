#include "RBGLedManager.h"

RGBLedManager::RGBLedManager(){}
char RGBLedManager::handleLed(int data){
    if(data <= 11){
          ledcWrite(1, 0);
          ledcWrite(2, 255); //GREEN
          ledcWrite(3, 0);
        } else if(data <= 34){
          ledcWrite(1, 255); 
          ledcWrite(2, 130); //YELLOW
          ledcWrite(3, 0);
        } else if(data <= 54){
          ledcWrite(1, 255);
          ledcWrite(2, 90); //Orange
          ledcWrite(3, 0); 
        } else{
          ledcWrite(1, 255);
          ledcWrite(2, 0); //RED
          ledcWrite(3, 0);
        }
}