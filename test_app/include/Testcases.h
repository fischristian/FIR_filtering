/* Copyright (C) 2015 Christian Fischer */
#ifndef TESTCASES_H__
#define TESTCASES_H__


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