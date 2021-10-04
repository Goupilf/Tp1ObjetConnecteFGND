#include <Arduino.h>
#include <unity.h>

#include "colorTest.h"
#include "aqiTest.h"

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    UNITY_BEGIN();

    RUN_TEST(test_DEL_color_changes_when_pm25_value_increment_from_0_to_300);
    RUN_TEST(aqiTest);
    
    UNITY_END();

}

void loop() {
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
    delay(500);
}
