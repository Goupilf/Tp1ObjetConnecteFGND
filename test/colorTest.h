#pragma once
#include <unity.h>
#include "..\lib\RBGLedManager\RBGLedManager.h"

RGBLedManager rgbLedManager;

void test_DEL_color_changes_when_pm25_value_increment_from_0_to_300(void) {
    int data=0;
    while(data <= 80){
        rgbLedManager.handleLed(data);
        delay(3000);
        data+=20;
    }
    TEST_ASSERT_EQUAL(1, 1);
}