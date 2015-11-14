/* Copyright (C) 2015 Christian Fischer */
#include <vector>
#include <iostream>
#include "../include/TestApp.h"
#include "../../filter_dll/include/FilterAPI.h"

#ifndef _GNULINUX
#include <tchar.h>
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, _TCHAR* argv[])
#endif
{
    std::string sourceImage = "abc.bin";
    FilterAPI::Filter::loadImage(sourceImage);

    std::vector<float>test_vector;
    FilterAPI::Filter::setFilter(test_vector, 1);

    try{
        FilterAPI::Filter::applyFilter();
    }
    catch (...){
        std::cout << "Exception caught during filter";
    }
}