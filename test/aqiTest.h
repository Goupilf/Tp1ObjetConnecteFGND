#pragma once
#include <unity.h>
#include <string.h>
#include <..\lib\AqiScale\AqiScale.h>

AqiScale aqiscale;

void getAQILabelFromPM25(void) {
    int number1=11;
    String expectedLabel1="Bon";
    String returnedValue1= aqiscale.getAqiLabel(number1);
    TEST_ASSERT_TRUE(returnedValue1.compareTo(expectedLabel1)!=0 );
    /*int number2=34;
    String expectedLabel2="Acceptable";
    String returnedValue2 = aqiscale.getAqiLabel(number2);
    TEST_ASSERT_EQUAL_STRING(returnedValue2, expectedLabel2);
    int number3=54;
    String expectedLabel3= "Mauvais pour les groupes sensibles";
    String returnedValue3 = aqiscale.getAqiLabel(number3);
    TEST_ASSERT_EQUAL_STRING(returnedValue3, expectedLabel3);
    int number4=55;
    String expectedLabel4="Mauvais pour la santé";
    String returnedValue4 = aqiscale.getAqiLabel(number4);
    TEST_ASSERT_EQUAL_STRING(returnedValue4, expectedLabel4);*/
}