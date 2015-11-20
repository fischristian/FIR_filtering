/* Copyright (C) 2015 Christian Fischer */
#ifndef TESTAPP_H__
#define TESTAPP_H__
#include <string>
#include <vector>

namespace DefaultSettings{
    /*!
     * /brief delimiter for the filter_coefficient input: "0.1;0.2; .." etc
     */
    std::string Delimiter = ";";

    /*!
    * /brief number of threads if user input is invalid
    */
    unsigned int uNumThreads = 3;

    /*!
    * /brief filter coefficients
    */
    std::vector<float>Filter_Coefficients({ 0.5, 0.5 });
}

#endif // TESTAPP_H__