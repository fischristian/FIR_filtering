/* Copyright (C) 2015 Christian Fischer */
#include <iostream>
#include "../include/Testcases.h"

void TestCases::RunAllTests() {
    unsigned int uNumErrors = 0;
    
    if (!TestCases::TestFunction_00_Invalid_Fir_Coefficients()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_01_Invalid_ImageSource()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_02_Invalid_ThreadNumber()) {
        ++uNumErrors;
    }

    std::cout << "All Tests finished. " << uNumErrors << " failed." << std::endl;
}

bool TestCases::TestFunction_00_Invalid_Fir_Coefficients() {
    return true;
}

bool TestCases::TestFunction_01_Invalid_ImageSource() {
    return true;
}

bool TestCases::TestFunction_02_Invalid_ThreadNumber() {
    return true;
}