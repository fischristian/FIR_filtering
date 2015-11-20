/* Copyright (C) 2015 Christian Fischer */
#include <iostream>
#include "../include/Testcases.h"
#include "../../filter_dll/include/FilterAPI.h"

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

    if (!TestCases::TestFunction_10_Performance_NumberOfThreads()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_11_Performance_NumberOfCoefficients()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_12_Performance_Duration()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_13_Performance_ImageSize()) {
        ++uNumErrors;
    }

    std::cout << "All Tests finished. " << uNumErrors << " failed." << std::endl;
}

bool TestCases::TestFunction_00_Invalid_Fir_Coefficients() {
    bool bReturn = true;
    try{
        std::vector<float>test00{};
        FilterAPI::Filter::configureFilter(test00, 1);
        bReturn = false;
        std::cout << "Expected exception: empty filter_coefficients but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    try{
        std::vector<float>test01{ (float)0.0, (float)0.1, (float)1.0 };
        FilterAPI::Filter::configureFilter(test01, 1);
        bReturn = false;
        std::cout << "Expected exception: sum filter_coefficients > 1.0 but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    try{
        std::vector<float>test02{ (float)0.0, (float)-0.1, (float)-1.0 };
        FilterAPI::Filter::configureFilter(test02, 1);
        bReturn = false;
        std::cout << "Expected exception: negative filter_coefficients but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    try{
        std::vector<float>test03{ 0.0, 0.0, 0.0 };
        FilterAPI::Filter::configureFilter(test03, 1);
        bReturn = false;
        std::cout << "Expected exception: all filter_coefficients zero but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    return bReturn;
}

bool TestCases::TestFunction_01_Invalid_ImageSource() {
    bool bReturn = true;
    try{
        std::string ImageSource00 = "";
        FilterAPI::Filter::loadImage(ImageSource00);
        bReturn = false;
        std::cout << "Expected exception: empty image source string but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    try{
        std::string ImageSource01 = "ThisDoesCertainlyNotExist";
        FilterAPI::Filter::loadImage(ImageSource01);
        bReturn = false;
        std::cout << "Expected exception: invalid image source string but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    return bReturn;
}

bool TestCases::TestFunction_02_Invalid_ThreadNumber() {
    bool bReturn = true;
    try{
        std::vector<float>test00{(float)0.1};
        FilterAPI::Filter::configureFilter(test00, 0);
        bReturn = false;
        std::cout << "Expected exception: zero threads but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
    }
    return bReturn;
}

bool TestCases::TestFunction_10_Performance_NumberOfThreads() {
    // for i = 1...10 threads and fixed coefficients & image & time compare number of output images

    return true;
}

bool TestCases::TestFunction_11_Performance_NumberOfCoefficients() {
    // for i = 1...10 number of coefficients & fixed threads & image & fixed time compare number of output images
    return true;
}

bool TestCases::TestFunction_12_Performance_Duration() {
    // for variable time & fixed threads and fixed coefficients & fixed image size compare number of output images
    return true;
}

bool TestCases::TestFunction_13_Performance_ImageSize() {
    // for variable image size & fixed threads and fixed coefficients & fixed time compare number of output images
    return true;
}