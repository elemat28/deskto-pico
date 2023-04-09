#include "unity.h"
#include <TempLib.h>  

void setUp(void) {

}

void tearDown(void) {
    
}

void test_getDisplayableName_should_return_given_name(void) {
    int ID = 1;
    char functionName[] = "My Setting Name!";
    TempLib setting = TempLib(functionName, ID);
    bool instanceNameEquals = (setting.getDisplayableName() == functionName);
    TEST_ASSERT_EQUAL(33,33);
}

int runUnityTests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_getDisplayableName_should_return_given_name);
    return UNITY_END();
}

int main(void) {
    return runUnityTests();
}