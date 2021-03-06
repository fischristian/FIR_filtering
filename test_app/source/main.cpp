/* Copyright (C) 2015 Christian Fischer */
#include <iostream>
#include "../include/Testcases.h"
#include "../include/Application.h"

#ifndef __linux
#include <tchar.h>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
#else
#include <cstring>
int main(int argc, char* argv[])
#endif
{
    std::cout << std::endl;
    std::cout << "############################################" << std::endl;
    std::cout << "# FIR_Filtering Test Application           #" << std::endl;
    std::cout << "############################################" << std::endl;
    std::cout << std::endl << std::endl;

    try {
        if (argc <= 1) {
            std::cout << "No arguments passed. Call with 'h' to see help menu." << std::endl;
        }
        if ((argc > 1) && argv[1]) {
            if (strcmp(argv[1], "h") == 0)
            {
                std::cout << "Help: \n" << std::endl;
                Application::printHelp();
                return 0;
            }
            else if (strcmp(argv[1], "test") == 0)
            {
                std::cout << "Executing in test mode..Running all tests \n" << std::endl;
                TestCases::RunAllTests();
                return 0;
            }
            // argv[1] - expected Image Source
            // argv[1] - expected Filter-Coefficients
            // argv[1] - expected Number of Threads
            if (argc > 3) {
                Application::RunMain(argv[1], argv[2], argv[3]);
            }
        }
    }
    catch (std::string * ex){
        std::cout << "Exception caught: " << ex->c_str()  << "\n"
            << "Programm terminating." << std::endl;
        return -1;
    }
    return 0;
}
