/* Copyright (C) 2015 Christian Fischer */
#ifndef TESTCASES_H__
#define TESTCASES_H__

#include <stdio.h>
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

#define DEFAULT_IMAGE_SIZE 1024*1024

#define DEFAULT_IMAGE_NAME "\\TestImage.bin"

#define DEFAULT_NUMBER_OF_THREADS 3

namespace TestCases{
    void RunAllTests();

    bool TestFunction_00_Invalid_Fir_Coefficients();

    bool TestFunction_01_Invalid_ImageSource();

    bool TestFunction_02_Invalid_ThreadNumber();

    bool TestFunction_10_Performance_NumberOfThreads();

    bool TestFunction_11_Performance_NumberOfCoefficients();

    bool TestFunction_12_Performance_Duration();

    bool TestFunction_13_Performance_ImageSize();
}

#endif // TESTCASES_H__