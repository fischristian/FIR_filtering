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

    /*
    * \brief Test input of invalid filter coefficients: empty vector, sum > 1.0, negative coefficients, all zeros
    * \brief Must throw an exception
    */
    bool TestFunction_00_Invalid_Fir_Coefficients();

    /*
    * \brief Test invalid image input: empty string, invalid string
    * \brief Must throw an exception
    */
    bool TestFunction_01_Invalid_ImageSource();

    /*
    * \brief Test invalid number of threads: zero
    * \brief Must throw an exception
    */
    bool TestFunction_02_Invalid_ThreadNumber();

    /*
    * \brief Test variable number of threads
    * \brief Fixed execution time, fixed vector of coefficients, fixed image size
    */
    bool TestFunction_10_Performance_NumberOfThreads();

    /*
    * \brief Test variable number of coefficients
    * \brief Fixed execution time, fixed number of threads, fixed image size
    */
    bool TestFunction_11_Performance_NumberOfCoefficients();

    /*
    * \brief Test variable execution time
    * \brief Fixed number of coefficients, fixed vector of coefficients, fixed image size
    */
    bool TestFunction_12_Performance_Duration();

    /*
    * \brief Test variable image size in bytes
    * \brief Fixed execution time, fixed vector of coefficients, fixed number of threads
    */
    bool TestFunction_13_Performance_ImageSize();
}

#endif // TESTCASES_H__