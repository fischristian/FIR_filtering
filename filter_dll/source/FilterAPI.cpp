/* Copyright (C) 2015 Christian Fischer */

#include <iomanip>
#include <algorithm>
#include <mutex>
#include "../include/FilterAPI.h"
#include "../include/FilterStatistics.h"

#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

std::vector<float>FilterAPI::Filter::mFilter;
unsigned char * FilterAPI::Filter::mImage = 0;
unsigned int FilterAPI::Filter::uImageSize = 0;
unsigned int FilterAPI::Filter::mNumThreads = 0;
bool FilterAPI::Filter::bTerminateFlag = false;
bool FilterAPI::Filter::bSettingsChanged = true;
std::vector<std::thread*> FilterAPI::Filter::mWorkerThreads;

std::mutex coefficients_lock;

std::string CurrentPath() {
    char cCurrentpath[FILENAME_MAX];
    if (!GetCurrentDir(cCurrentpath, sizeof(cCurrentpath))) {
        // error handling
        std::cout << "Could not get current working dir\n" << std::endl;
    }
    return std::string(cCurrentpath);
}

#ifdef __cplusplus
extern "C" {
    namespace FilterAPI {
#endif        
        /*****************************************************************************/
        void Filter::configureFilter(const std::vector<float>& vFilter, unsigned int uNumThreads) {
            // std::cout << "Filter::setFilter called \n";
            if (vFilter.empty()){
                throw new std::string("Filter coefficients are empty");
            }
            if (std::all_of(vFilter.begin(), vFilter.end(), [](float i){return i == 0.0; })) {
                throw new std::string("Filter coefficients are all zero");
            }
            if (uNumThreads == 0){
                throw new std::string("Number of threads is zero");
            }
            float fCoeffientSum = 0.0;
            for (size_t i = 0; i < vFilter.size(); i++)
            {
                if (vFilter[i] < (float)0.0) {
                    throw new std::string("Filter coefficients must be positive");
                }
                fCoeffientSum += vFilter[i];
            }
            if (fCoeffientSum > (float)1.001) {
                throw new std::string("Sum of coefficients greater than 1.0 detected");
            }
            coefficients_lock.lock();
            Filter::mFilter = vFilter;
            mNumThreads = uNumThreads;
            coefficients_lock.unlock();
        }
        /*****************************************************************************/
        bool Filter::loadImage(const std::string& source) {
            if (source == ""){
                throw new std::string("Invalid Image source");
            }
            // std::cout << "Filter::loadImage called \n";
            std::ifstream myFile;
            myFile.open(source.c_str(), std::ifstream::binary);
            if (!myFile.is_open()) {
                throw new std::string("Image File could not be opened");
            }
            myFile.seekg(0, myFile.end);
            size_t iSize = myFile.tellg();
            if (iSize == 0) {
                throw new std::string("Image File is empty");
            }
            myFile.seekg(0, myFile.beg);

            // Reset Image memory, if it was already initialized
            coefficients_lock.lock();
            if (mImage != nullptr) {
                delete[] mImage;
                mImage = nullptr;
            }

            try{
                mImage = new unsigned char[iSize];
            }
            catch (...){
                throw new std::string("Could not allocate memory to store Image file");
                coefficients_lock.unlock();
            }
            memset(mImage, 0, iSize);
            myFile.read((char*)mImage, iSize);

            uImageSize = static_cast<unsigned int>(iSize);

            coefficients_lock.unlock();
            
            std::cout << source.c_str() << " successfully opened\n" << std::endl;
            std::cout << iSize << " Bytes successfully read\n" << std::endl;

            myFile.close();

            return true;
        }
        /*****************************************************************************/
        void Filter::Start() {
            bTerminateFlag = false;
            bSettingsChanged = true;
            FilterStatistics::ResetNumberOfProcessedImages();
            if (mFilter.empty()) {
                throw new std::string("Invalid filter coefficients");
            }
            if (mImage == 0) {
                throw new std::string("Invalid image source");
            }

            for (unsigned int i = 0; i < mNumThreads; ++i){
                std::thread * first = new std::thread(Filter::ThreadRoutine);
                mWorkerThreads.push_back(first);
            }
        }
        /*****************************************************************************/
        void Filter::Stop() {
            bTerminateFlag = true;
            bSettingsChanged = false;
            while (!mWorkerThreads.empty()) {
                std::thread * pThread = mWorkerThreads.back();
                pThread->join();
                mWorkerThreads.pop_back();
                delete pThread;
                if (mWorkerThreads.empty()) {
                    break;
                }
            }
         }
        /*****************************************************************************/
        void Filter::Release() {
            if (mImage) {
                delete[] mImage;
                mImage = nullptr;
            }
        }
        /*****************************************************************************/
        long long Filter::getNumberOfPrcessedImages() {
            return FilterStatistics::GetNumberOfProcessedImages();
        }

        void Filter::ThreadRoutine() {
            unsigned char * pFilteredImage = nullptr;

            try {
                pFilteredImage = new unsigned char[uImageSize];
            }
            catch (...){
                std::cout << "ThreadRoutine error: could not allocate memory for resulting image" << std::endl;
                return;
            }
            memset((void*)pFilteredImage, 0, uImageSize);

            std::string sPath = CurrentPath();
            std::stringstream sImageName;
            sImageName << "\\Output_" << uImageSize << ".bin";
            sPath.append(sImageName.str());
            std::ofstream TestImage;
            TestImage.open(sPath, std::ios::out | std::ios::binary);

            /*
            fircoeffs = { 1/11, 2/11, 5/11, 2/11, 1/11 }; // for example
            for each 'y' in column rows:
            filtered[y] = 0;
            for each 'i' in fircoeffs:
            if 'y+i' is in the original image boundaries:
            filtered[y] += original[y+i] * fircoeffs[i];
            */

            // keep filter coefficients and image / imagesize threadsafe
            // yet not be modifiable during filtering algorithm
            coefficients_lock.lock();
            std::vector<float>tmpFilter = mFilter;
            unsigned char * tmpImage = mImage;
            unsigned int tmpImageSIze = uImageSize;
            coefficients_lock.unlock();

            while (!bTerminateFlag) {
                size_t iNumCoff = tmpFilter.size();
                for (unsigned int iPxl = 0; iPxl < tmpImageSIze; ++iPxl) {
                    if (iPxl < (tmpImageSIze - iNumCoff)) {
                        for (int i = 0; i < iNumCoff; ++i) {
                            pFilteredImage[iPxl] += static_cast<unsigned char>(static_cast<float>(tmpImage[iPxl + i]) * tmpFilter[i]);
                        }
                    }
                }
                FilterStatistics::IncreaseNumberOfProcessedImages();
                if (bSettingsChanged) {
                    TestImage.write((const char*)pFilteredImage, tmpImageSIze);
                    bSettingsChanged = false;
                }
            }
            TestImage.close();
            delete[] pFilteredImage;
            pFilteredImage = NULL;
        }
#ifdef __cplusplus
}  //  namespace FilterAPI
}  //  extern "C" {
#endif
