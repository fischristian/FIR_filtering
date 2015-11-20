/* Copyright (C) 2015 Christian Fischer */

#include <iomanip>
#include "../include/FilterAPI.h"
#include "../include/FilterStatistics.h"

        std::vector<float>FilterAPI::Filter::mFilter;
        unsigned char * FilterAPI::Filter::mImage = 0;
        unsigned int FilterAPI::Filter::uImageSize = 0;
        unsigned int FilterAPI::Filter::mNumThreads = 0;
        std::vector<std::thread*> FilterAPI::Filter::mWorkerThreads;

        // std::thread does not provide a termination flag
        bool bGlobalExit = false;

#ifdef __cplusplus
extern "C" {
    namespace FilterAPI {
#endif

        void ThreadRoutine(std::vector<float>filter_coeff) {
            // create a test image:
            // height = 480, width = 640
            unsigned int ImageSize = FilterAPI::Filter::getImageSize();
            unsigned char * pImage = new unsigned char[ImageSize];
            for (int i = 0; i < ImageSize; ++i) {
                pImage[i] = (unsigned char)(i % 255);
                // std::cout << "pImage: " << pImage[i] << "\n";
            }

            unsigned char * pFilteredImage = new unsigned char[ImageSize];
            memset((void*)pFilteredImage, 0, ImageSize);

            /*
            fircoeffs = { 1/11, 2/11, 5/11, 2/11, 1/11 }; // for example
            for each 'y' in column rows:
            filtered[y] = 0;
            for each 'i' in fircoeffs:
            if 'y+i' is in the original image boundaries:
            filtered[y] += original[y+i] * fircoeffs[i];
            */
            while (!bGlobalExit) {
                // refresh filter_coefficients
                // if filter_coefficients are changed, store resulting file with new name

                int iNumCoff = filter_coeff.size();
                for (int iPxl = 0; iPxl < ImageSize; ++iPxl) {
                    if (iPxl < (ImageSize - iNumCoff)) {
                        for (int i = 0; i < iNumCoff; ++i) {
                            pFilteredImage[iPxl] += pImage[iPxl + i] * filter_coeff[i];
                        }
                    }
                }
                FilterStatistics::IncreaseNumberOfProcessedImages();
            }

            delete[] pImage;
            pImage = NULL;
            delete[] pFilteredImage;
            pFilteredImage = NULL;
        }

        void Filter::configureFilter(const std::vector<float>& vFilter, unsigned int uNumThreads) {
            // std::cout << "Filter::setFilter called \n";
            if (vFilter.empty()){
                throw new std::string("Filter coefficients are empty");
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
            if (fCoeffientSum > (float)1.01) {
                throw new std::string("Sum of coefficients greater than 1.0 detected");
            }
            Filter::mFilter = vFilter;
            mNumThreads = uNumThreads;
        }

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

            char * charImageData = new char[iSize+1];
            memset(charImageData, 0, iSize+1);
            myFile.read((char*)charImageData, iSize);

            mImage = new unsigned char[iSize / 2];
            memset(mImage, 0, iSize/2);
            uImageSize = iSize / 2;

            // charImageData contains ascii values of hex values
            // goal: transform ascii to byte value and store it in mImage

            // ugly, but works.
            for (size_t i = 0; i < iSize; i += 2)
            {
                char pChar1 = charImageData[i];
                std::string s1(&pChar1, 1);
                char pChar2 = charImageData[i+1];
                std::string s2(&pChar2, 1);

                std::stringstream stdS1(s1);
                std::stringstream stdS2(s2);

                int16_t Value1 = 0;
                int16_t Value2 = 0;

                stdS1 >> std::hex >> Value1;
                stdS2 >> std::hex >> Value2;

                // shift left value1 by 4 bit and add value2
                mImage[i/2] = (unsigned char)((Value1 << 4) & 0xFF | Value2 & 0xFF);
            }

            std::cout << source.c_str() << " successfully opened\n" << std::endl;
            std::cout << iSize << " Bytes successfully read\n" << std::endl;

            delete[] charImageData;
            charImageData = nullptr;
            myFile.close();

            return true;
        }

        void Filter::Start() {
            if (mFilter.empty()) {
                throw new std::string("Invalid filter coefficients");
            }
            if (mImage == 0) {
                throw new std::string("Invalid image source");
            }

            for (int i = 0; i < mNumThreads; ++i){
                std::thread * first = new std::thread(ThreadRoutine, mFilter);
                mWorkerThreads.push_back(first);
            }
        }

        void Filter::Stop() {
            bGlobalExit = true;
            while (!mWorkerThreads.empty()) {
                std::thread * pThread = mWorkerThreads.back();
                pThread->join();
                mWorkerThreads.pop_back();
                delete pThread;
                if (mWorkerThreads.empty()) {
                    break;
                }
            }
            if (mImage) {
                delete[] mImage;
                mImage = NULL;
            }
         }

        long long Filter::getNumberOfPrcessedImages() {
            return FilterStatistics::GetNumberOfProcessedImages();
        }
#ifdef __cplusplus
}  //  namespace FilterAPI
}  //  extern "C" {
#endif
