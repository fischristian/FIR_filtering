/* Copyright (C) 2015 Christian Fischer */
#include <vector>
#include <iostream>
#include <chrono>

#include "../include/TestApp.h"
#include "../../filter_dll/include/FilterAPI.h"

void printHelp() {
    std::cout << "Valid options: ..." << std::endl;
}

std::vector<float> getFilterCoefficients(const std::string& sUserInput){
    
    std::string delimiter = " ";
    std::stringstream sRes(sUserInput);
    std::vector<std::string>Svector;
    std::string sTok;

    while (std::getline(sRes, sTok, *delimiter.c_str())){
        Svector.push_back(sTok);
    }

    std::vector<float>user_fCoeff;
    for (size_t i = 0; i < Svector.size(); i++)
    {
        // atof has a no-throw guarantee. if the value is invalid, 0.0 will be set
        user_fCoeff.push_back(atof(Svector[i].c_str()));
    }

    return user_fCoeff;
}

#ifndef __linux
#include <tchar.h>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    try {
        // only test purpose
        std::string user_coeff = "0.1 0.2 0.5 0.2 0.1";

        std::string sourceImage = "";
        // std::string sourceImage = "C://temp//Test_Image.dat";

        std::vector<float>user_fCoeff;

        int iNumThreads = 0;


        if (argc <= 1) {
            std::cout << "No arguments passed. - Program in testmode with default parameters: .." << std::endl;
        }
        if ((argc > 1) && argv[1]) {
            if (strcmp(argv[1], "h") == 0)
            {
                printHelp;
            }
            else {
                sourceImage = argv[1];
            }
        }
        if ((argc > 2) && argv[2]) {
            user_fCoeff = getFilterCoefficients(argv[2]);
        }
        if ((argc > 3) && argv[3]) {
            iNumThreads = atoi(argv[3]);
        }

        FilterAPI::Filter::loadImage(sourceImage);

        FilterAPI::Filter::configureFilter(user_fCoeff, iNumThreads);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        FilterAPI::Filter::Start();
        FilterAPI::Filter::Stop();
        long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << "Duration: " << duration << " µs\n";

        std::cout << "Input any number to close the program:";
        int endKey = 0;
        std::cin >> endKey;
    }
    catch (std::string * ex){
        std::cout << "Exception caught: " << ex->c_str()  << "\n"
            << "Programm terminating." << std::endl;
    }
}
