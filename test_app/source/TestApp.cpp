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
    std::string sourceImage = "abc.bin";
    FilterAPI::Filter::loadImage(sourceImage);

    float OBE = (float)1 / (float)11;
    std::vector<float>test_vector({ OBE, 2*OBE, 5*OBE, 2*OBE, 1*OBE });
    FilterAPI::Filter::setFilter(test_vector, 1);

    try{
        FilterAPI::Filter::applyFilter();
    }
    catch (...){
        std::cout << "Exception caught during filter";
    }
}
