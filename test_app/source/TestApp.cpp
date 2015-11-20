/* Copyright (C) 2015 Christian Fischer */
#include <iostream>
#include <chrono>
#include <algorithm>

#include "../include/TestApp.h"
#include "../../filter_dll/include/FilterAPI.h"

void printHelp() {
    std::cout << "Valid options: ..." << std::endl;
}

std::vector<float> getFilterCoefficients(const std::string& sUserInput){
    std::string delimiter = DefaultSettings::Delimiter;
    // convert string to stringstream
    std::stringstream sRes(sUserInput);
    std::vector<std::string>Svector;
    // buffer for found token
    std::string sTok;

    // seperate user input with the default delmiter
    while (std::getline(sRes, sTok, *delimiter.c_str())){
        Svector.push_back(sTok);
    }

    // resulting output vector of floats
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
        std::string sourceImage = "";
        int iNumThreads = 0;
        bool bTestMode = false;

        if (argc <= 1) {
            std::cout << "No arguments passed. - Program in testmode with default parameters: ..." << std::endl;
        }
        if ((argc > 1) && argv[1]) {
            if (strcmp(argv[1], "h") == 0)
            {
                printHelp;
                return 0;
            }
            else if (strcmp(argv[1], "test") == 0)
            {
                printHelp;
                bTestMode = true;
            }
            else {
                sourceImage = argv[1];
            }
        }

        try{
            FilterAPI::Filter::loadImage(sourceImage);
        }
        catch (std::string * ex){
            std::cout << "Exception caught: " << ex->c_str() << "\n " << std::endl;
            std::cout << "Default image will be used instead: " << std::endl;
        }

        std::vector<float>user_fCoeff;
        if ((argc > 2) && argv[2]) {
            user_fCoeff = getFilterCoefficients(argv[2]);
            // check if input is valid (e.g. has at least one element > 0)
            if (std::all_of(user_fCoeff.begin(), user_fCoeff.end(), [](float i){return i == 0.0; })) {
                std::cout << "Filter coefficients are invalid; DefaultSettings are used instead: \n" << std::endl;
                for (size_t i = 0; i < DefaultSettings::Filter_Coefficients.size(); ++i) {
                    std::cout << "Default " << i << ": " << DefaultSettings::Filter_Coefficients[i] << std::endl;
                }
                user_fCoeff = DefaultSettings::Filter_Coefficients;
            }
        }

        if ((argc > 3) && argv[3]) {
            iNumThreads = atoi(argv[3]);
            if (iNumThreads <= 0) {
                std::cout << "Invalid number of threads: " << iNumThreads << "\n" << std::endl;
                std::cout << "Default threads will be used instead: " << DefaultSettings::uNumThreads << std::endl;
            }
        }

        FilterAPI::Filter::configureFilter(user_fCoeff, iNumThreads);

        // configuration finished, switch to main operation
        if (bTestMode){

        }
        else {

        }

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
