/* Copyright (C) 2015 Christian Fischer */
#include <vector>
#include <iostream>
#include <chrono>

#include "../include/TestApp.h"
#include "../../filter_dll/include/FilterAPI.h"


#ifndef __linux
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    try {
        std::string sourceImage = "C://temp//Test_Image.dat";
        FilterAPI::Filter::loadImage(sourceImage);


        float OBE = (float)1 / (float)11;
        std::vector<float>test_vector({ OBE, 2*OBE, 5*OBE, 2*OBE, 1*OBE });
        FilterAPI::Filter::configureFilter(test_vector, 3);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        FilterAPI::Filter::Start();
        FilterAPI::Filter::Stop();
        long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

        std::cout << "Duration: " << duration << " µs\n";
        std::cout << "OK";
    }
    catch (std::string * ex){
        std::cout << "Exception caught: " << ex->c_str()  << "\n"
            << "Programm terminating." << std::endl;
    }
}
