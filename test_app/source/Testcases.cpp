/* Copyright (C) 2015 Christian Fischer */
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include "../include/Testcases.h"
#include "../../filter_dll/include/FilterAPI.h"

std::string CurrentPath() {
    char cCurrentpath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentpath, sizeof(cCurrentpath))) {
        // error handling
        std::cout << "Could not get current working dir\n" << std::endl;
    }
    return std::string(cCurrentpath);
}
/*****************************************************************************/
unsigned char * CreateTestImage(size_t uImageSize) {
    // create a test image with the given size
    unsigned char * pTestImage = nullptr;
    try{
        pTestImage = new unsigned char[uImageSize];
    }
    catch (...) {
        std::cout << "Could not create test image of size " << uImageSize << "\n" << std::endl;
    }
    if (pTestImage) {
        memset((void*)pTestImage, 0, uImageSize);
        for (size_t i = 0; i < uImageSize; i++)
        {
            pTestImage[i] = ((i % 5) * 50);
        }
    }
    return pTestImage;
}
/*****************************************************************************/
void TestCases::RunAllTests() {
    unsigned int uNumErrors = 0;
    
    if (!TestCases::TestFunction_00_Invalid_Fir_Coefficients()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_01_Invalid_ImageSource()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_02_Invalid_ThreadNumber()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_10_Performance_NumberOfThreads()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_11_Performance_NumberOfCoefficients()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_12_Performance_Duration()) {
        ++uNumErrors;
    }

    if (!TestCases::TestFunction_13_Performance_ImageSize()) {
        ++uNumErrors;
    }

    std::cout << "All Tests finished. " << uNumErrors << " failed." << std::endl;
}
/*****************************************************************************/
bool TestCases::TestFunction_00_Invalid_Fir_Coefficients() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    bool bReturn = true;
    try{
        std::vector<float>test00{};
        FilterAPI::Filter::configureFilter(test00, 1);
        bReturn = false;
        std::cout << "Expected exception: empty filter_coefficients but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    try{
        std::vector<float>test01{ (float)0.0, (float)0.1, (float)1.0 };
        FilterAPI::Filter::configureFilter(test01, 1);
        bReturn = false;
        std::cout << "Expected exception: sum filter_coefficients > 1.0 but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    try{
        std::vector<float>test02{ (float)0.0, (float)-0.1, (float)-1.0 };
        FilterAPI::Filter::configureFilter(test02, 1);
        bReturn = false;
        std::cout << "Expected exception: negative filter_coefficients but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    try{
        std::vector<float>test03{ 0.0, 0.0, 0.0 };
        FilterAPI::Filter::configureFilter(test03, 1);
        bReturn = false;
        std::cout << "Expected exception: all filter_coefficients zero but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_01_Invalid_ImageSource() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    bool bReturn = true;
    try{
        std::string ImageSource00 = "";
        FilterAPI::Filter::loadImage(ImageSource00);
        bReturn = false;
        std::cout << "Expected exception: empty image source string but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    try{
        std::string ImageSource01 = "ThisDoesCertainlyNotExist";
        FilterAPI::Filter::loadImage(ImageSource01);
        bReturn = false;
        std::cout << "Expected exception: invalid image source string but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_02_Invalid_ThreadNumber() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    bool bReturn = true;
    try{
        std::vector<float>test00{(float)0.1};
        FilterAPI::Filter::configureFilter(test00, 0);
        bReturn = false;
        std::cout << "Expected exception: zero threads but did not throw\n" << std::endl;
    }
    catch (std::string * ex) {
        // ok by design
        (void*)ex;
    }
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_10_Performance_NumberOfThreads() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    // for i = 1...10 threads and fixed coefficients & image & time compare number of output images
    bool bReturn = true;
    unsigned char * pImage = CreateTestImage(DEFAULT_IMAGE_SIZE);
    if (pImage != nullptr) {
        std::string sPath = CurrentPath();
        sPath.append(DEFAULT_IMAGE_NAME);
        std::ofstream TestImage;
        TestImage.open(sPath, std::ios::out | std::ios::binary);
        TestImage.write((const char*)pImage, DEFAULT_IMAGE_SIZE);
        std::vector<float>Filter_Coefficients = { (float)0.5, (float)0.5 };

        try{
            FilterAPI::Filter::loadImage(sPath);
        }
        catch (std::string * ex) {
            bReturn = false;
            std::cout << "TestFunction_10_Performance_NumberOfThreads: Could not load image; \n"
                << ex << "\n" << std::endl;
        }

        for (unsigned int iNumThreads = 1; bReturn == true && iNumThreads <= 10; iNumThreads++)
        {
            try{
                FilterAPI::Filter::configureFilter(Filter_Coefficients, iNumThreads);
            }
            catch (std::string * ex) {
                bReturn = false;
                std::cout << "TestFunction_10_Performance_NumberOfThreads: Could not configure filter; \n"
                    << ex << "\n" << std::endl;
                break;
            }
            FilterAPI::Filter::Start();
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            FilterAPI::Filter::Stop();
            long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            std::cout << "Number of threads: " << iNumThreads << "; Execution time: " << duration << "; Processed Images: " << NumberOfProcessedImages << "\n" << std::endl;
        }

        TestImage.close();
        if (remove(sPath.c_str()) != 0) {
            std::cout << "Could not remove the created Test image \n" << std::endl;
        }
        FilterAPI::Filter::Release();

        if (pImage) {
            delete[] pImage;
            pImage = nullptr;
        }
    }
    else {
        bReturn = false;
    }

    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_11_Performance_NumberOfCoefficients() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    // for i = 1...10 number of coefficients & fixed threads & image & fixed time compare number of output images
    bool bReturn = true;
    unsigned char * pImage = CreateTestImage(DEFAULT_IMAGE_SIZE);
    if (pImage != nullptr) {
        std::string sPath = CurrentPath();
        sPath.append(DEFAULT_IMAGE_NAME);
        std::ofstream TestImage;
        TestImage.open(sPath, std::ios::out | std::ios::binary);
        TestImage.write((const char*)pImage, DEFAULT_IMAGE_SIZE);

        try{
            FilterAPI::Filter::loadImage(sPath);
        }
        catch (std::string * ex) {
            bReturn = false;
            std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not load image; \n"
                << ex << "\n" << std::endl;
        }

        for (size_t iNumCoefficients = 1; bReturn == true && iNumCoefficients <= 10; iNumCoefficients++)
        {
            std::vector<float>Filter_Coefficients;
            for (size_t i = 0; i < iNumCoefficients; i++)
            {
                Filter_Coefficients.push_back((float)0.1);
            }
            try{
                FilterAPI::Filter::configureFilter(Filter_Coefficients, DEFAULT_NUMBER_OF_THREADS);
            }
            catch (std::string * ex) {
                bReturn = false;
                std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not configure filter; \n"
                    << ex << "\n" << std::endl;
                break;
            }
            FilterAPI::Filter::Start();
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            FilterAPI::Filter::Stop();
            long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            std::cout << "Number of coefficients: " << iNumCoefficients << "; Execution time: " << duration << "; Processed Images: " << NumberOfProcessedImages << "\n" << std::endl;
        }

        FilterAPI::Filter::Release();

        TestImage.close();
        if (remove(sPath.c_str()) != 0) {
            std::cout << "Could not remove the created Test image \n" << std::endl;
        }

        if (pImage) {
            delete[] pImage;
            pImage = nullptr;
        }
    }
    else {
        bReturn = false;
    }
    
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_12_Performance_Duration() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    // for variable time & fixed threads and fixed coefficients & fixed image size compare number of output images
    bool bReturn = true;
    unsigned char * pImage = CreateTestImage(DEFAULT_IMAGE_SIZE);
    if (pImage != nullptr) {
        std::string sPath = CurrentPath();
        sPath.append(DEFAULT_IMAGE_NAME);
        std::ofstream TestImage;
        TestImage.open(sPath, std::ios::out | std::ios::binary);
        TestImage.write((const char*)pImage, DEFAULT_IMAGE_SIZE);
        std::vector<float>Filter_Coefficients = { (float)0.5, (float)0.5 };

        try{
            FilterAPI::Filter::loadImage(sPath);
        }
        catch (std::string * ex) {
            bReturn = false;
            std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not load image; \n"
                << ex << "\n" << std::endl;
        }

        for (size_t iWaitTime = 100; bReturn == true && iWaitTime <= 1000; iWaitTime += 100)
        {
            try{
                FilterAPI::Filter::configureFilter(Filter_Coefficients, DEFAULT_NUMBER_OF_THREADS);
            }
            catch (std::string * ex) {
                bReturn = false;
                std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not configure filter; \n"
                    << ex << "\n" << std::endl;
                break;
            }
            FilterAPI::Filter::Start();
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            std::this_thread::sleep_for(std::chrono::milliseconds(iWaitTime));
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            FilterAPI::Filter::Stop();
            long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();

            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
            std::cout << "Wait time: " << iWaitTime << "; Execution time: " << duration << "; Processed Images: " << NumberOfProcessedImages << "\n" << std::endl;
        }

        FilterAPI::Filter::Release();

        TestImage.close();
        if (remove(sPath.c_str()) != 0) {
            std::cout << "Could not remove the created Test image \n" << std::endl;
        }

        if (pImage) {
            delete[] pImage;
            pImage = nullptr;
        }
    }
    else {
        bReturn = false;
    }
    
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}
/*****************************************************************************/
bool TestCases::TestFunction_13_Performance_ImageSize() {
    std::cout << "Run " << __FUNCTION__ << std::endl;
    // for variable image size & fixed threads and fixed coefficients & fixed time compare number of output images
    bool bReturn = true;
    std::vector<float>Filter_Coefficients = { (float)0.5, (float)0.5 };    

    for (size_t iImageSize = 100; bReturn == true && iImageSize <= 1000; iImageSize += 100)
    {
        unsigned char * pImage = CreateTestImage(DEFAULT_IMAGE_SIZE);
        if (pImage == nullptr) {
            bReturn = false;
            break;
        }
        std::string sPath = CurrentPath();
        sPath.append(DEFAULT_IMAGE_NAME);
        std::ofstream TestImage;
        TestImage.open(sPath, std::ios::out | std::ios::binary);
        TestImage.write((const char*)pImage, (iImageSize * iImageSize));

        try{
            FilterAPI::Filter::loadImage(sPath);
        }
        catch (std::string * ex) {
            bReturn = false;
            std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not load image; \n"
                << ex << "\n" << std::endl;
            break;
        }

        try{
            FilterAPI::Filter::configureFilter(Filter_Coefficients, DEFAULT_NUMBER_OF_THREADS);
        }
        catch (std::string * ex) {
            bReturn = false;
            std::cout << "TestFunction_11_Performance_NumberOfCoefficients: Could not configure filter; \n"
                << ex << "\n" << std::endl;
            break;
        }
        FilterAPI::Filter::Start();
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        FilterAPI::Filter::Stop();
        long long NumberOfProcessedImages = FilterAPI::Filter::getNumberOfPrcessedImages();

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::cout << "ImageSize: " << iImageSize * iImageSize << "; Execution time: " << duration
            << "; Processed Images: " << NumberOfProcessedImages << "\n" << std::endl;

        FilterAPI::Filter::Release();

        TestImage.close();
        if (remove(sPath.c_str()) != 0) {
            std::cout << "Could not remove the created Test image \n" << std::endl;
        }

        if (pImage) {
            delete[] pImage;
            pImage = nullptr;
        }
    }
    if (bReturn) {
        std::cout << __FUNCTION__ << " - passed\n\n";
    }
    else {
        std::cout << __FUNCTION__ << " - failed\n\n";
    }
    return bReturn;
}