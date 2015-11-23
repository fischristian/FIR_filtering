/* Copyright (C) 2015 Christian Fischer */
#include <chrono>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "../include/Application.h"
#include "../../filter_dll/include/FilterAPI.h"


namespace DefaultSettings {
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
/*****************************************************************************/
void Application::printHelp() {
    std::cout << "Valid options:" << std::endl;
    std::cout << std::endl;
    std::cout << " 'h'" << std::endl;
    std::cout << "   - read help \n\n" << std::endl;
    std::cout << " 'test'" << std::endl;
    std::cout << "   - execute program in test mode \n\n" << std::endl;
    std::cout << " 'arg1 arg2 arg3'" << std::endl;
    std::cout << "   - arg1: Image source, e.g. C:\\temp\\myImage.bin" << std::endl;
    std::cout << "   - arg2: filter coefficients separated by ';', e.g. 0.1;0.2;0.1" << std::endl;
    std::cout << "   - arg3: number of threads > 0, e.g. 5 \n\n" << std::endl;
    std::cout << std::endl << std::endl;
}
/*****************************************************************************/
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
/*****************************************************************************/
void Application::RunMain(const std::string& ImageSource, const std::string& Filter_Coeff, const std::string& Num_Threads) {
    std::vector<float>user_fCoeff;
    int iNumThreads = 0;
    try{
        FilterAPI::Filter::loadImage(ImageSource);
    }
    catch (std::string * ex){
        std::cout << "Exception caught: " << ex->c_str() << "\n " << std::endl;
        std::cout << "Input a valid image file." << std::endl;
        return;
    }
    if (Filter_Coeff != "") {
        user_fCoeff = getFilterCoefficients(Filter_Coeff);
        // check if input is valid (e.g. has at least one element > 0)
        if (std::all_of(user_fCoeff.begin(), user_fCoeff.end(), [](float i){return i == 0.0; })) {
            std::cout << "Filter coefficients are invalid; DefaultSettings are used instead: \n" << std::endl;
            for (size_t i = 0; i < DefaultSettings::Filter_Coefficients.size(); ++i) {
                std::cout << "Default " << i << ": " << DefaultSettings::Filter_Coefficients[i] << std::endl;
            }
            user_fCoeff = DefaultSettings::Filter_Coefficients;
        }
    }

    if (Num_Threads != "") {
        iNumThreads = atoi(Num_Threads.c_str());
        if (iNumThreads <= 0) {
            std::cout << "Invalid number of threads: " << iNumThreads << "\n" << std::endl;
            std::cout << "Default threads will be used instead: " << DefaultSettings::uNumThreads << std::endl;
        }
    }

    FilterAPI::Filter::configureFilter(user_fCoeff, iNumThreads);

    FilterAPI::Filter::Start();
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Threads started. Press any key to stop filtering..";
    int endKey = 0;
    while (endKey == 0){
        std::cin >> endKey;
    }
    FilterAPI::Filter::Stop();
    long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << "Duration: " << duration << " microsec\n";
    std::cout << "Processed Images: " << NumberOfProcessedImages << "\n";
    FilterAPI::Filter::Release();
}