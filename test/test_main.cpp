#include <Arduino.h>
#include <unity.h>
#include "aqiTest.h"
#include <..\lib\AqiScale\AqiScale.h>

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();
    
    RUN_TEST(getAQILabelFromPM25);
    
    UNITY_END();

}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}
