/* Copyright (C) 2015 Christian Fischer */
#ifndef APPLICATION_H__
#define APPLICATION_H__
#include <string>
#include <vector>

namespace Application {
    /*
    * \brief main program routine with user given input
    */
    void RunMain(const std::string& ImageSource, const std::string& Filter_Coeff, const std::string& Num_Threads);

    /*
    * \brief if first parameter 'h' was passed print help menu to screen and exit program
    */
    void printHelp();
}

#endif // APPLICATION_H__