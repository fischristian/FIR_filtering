/* Copyright (C) 2015 Christian Fischer */
#ifndef APPLICATION_H__
#define APPLICATION_H__
#include <string>
#include <vector>

namespace Application {
    void RunMain(const std::string& ImageSource, const std::string& Filter_Coeff, const std::string& Num_Threads);

    void printHelp();
}

#endif // APPLICATION_H__