#pragma once

#include <Arduino.h>
#include <unity.h>

void test3_1(void) {
    Serial.println("Test 3.1");
    TEST_ASSERT_EQUAL(2, 2);
}

void test3_2(void) {
    Serial.println("Test 3.3");
    TEST_ASSERT_EQUAL(2, 2);
}

void test3_3(void) {
    Serial.println("Test 3.3");
    TEST_ASSERT_EQUAL(2, 2);
}

void process_test3(void) {
    RUN_TEST(test3_1);
    RUN_TEST(test3_2);
    RUN_TEST(test3_3);
}
