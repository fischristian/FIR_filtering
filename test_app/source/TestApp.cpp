/* Copyright (C) 2015 Christian Fischer */
#include <vector>
#include <iostream>
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
        std::string sourceImage = "";
        FilterAPI::Filter::loadImage(sourceImage);

        float OBE = (float)1 / (float)11;
        std::vector<float>test_vector({ OBE, 2*OBE, 5*OBE, 2*OBE, 1*OBE });
        FilterAPI::Filter::configureFilter(test_vector, 3);

        FilterAPI::Filter::Start();
        FilterAPI::Filter::Stop();
    }
    catch (std::string * ex){
        std::cout << "Exception caught: " << ex->c_str()  << "\n"
            << "Programm terminating." << std::endl;
    }
}
