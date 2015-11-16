/* Copyright (C) 2015 Christian Fischer */

#include "../include/FilterAPI.h"

        std::vector<float>FilterAPI::Filter::mFilter;
        std::string FilterAPI::Filter::mImage = "";
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
            int ImageSize = 480 * 640;
            unsigned char * pImage = new unsigned char[ImageSize];
            for (int i = 0; i < ImageSize; ++i) {
                pImage[i] = (unsigned char)(i % 255);
                // std::cout << "pImage: " << pImage[i] << "\n";
            }
            std::vector<float>test_coff(filter_coeff);

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
                int iNumCoff = test_coff.size();
                for (int iPxl = 0; iPxl < ImageSize; ++iPxl) {
                    if ((iPxl % 640) < (640 - iNumCoff)) {
                        // std::cout << "Current Pixel: "<< (iPxl % 640) << " \n";
                        // std::cout << "PixelValue = " << *(pImage + iPxl) << " \n";
                        for (int i = 0; i < iNumCoff; ++i) {
                            pFilteredImage[iPxl] += pImage[iPxl + i] * test_coff[i];
                        }
                    }
                }
            }

            delete[] pImage;
            pImage = NULL;
            delete[] pFilteredImage;
            pFilteredImage = NULL;
        }

        void Filter::configureFilter(const std::vector<float>& vFilter, unsigned int uNumThreads) {
            if (vFilter.empty()){
                throw new std::string("Filter coefficients are empty");
            }
            if (uNumThreads == 0){
                throw new std::string("Number of threads is zero");
            }
            std::cout << "Filter::setFilter called \n";
            Filter::mFilter = vFilter;
            mNumThreads = uNumThreads;
        }

        bool Filter::loadImage(const std::string& source) {
            if (source == ""){
                throw new std::string("Invalid Image source");
            }
            std::cout << "Filter::loadImage called \n";
            mImage = source;
            return true;
        }

        void Filter::Start() {
            if (mFilter.empty()) {
                throw new std::string("Invalid filter coefficients");
            }
            if (mImage == "") {
                throw new std::string("Invalid image source");
            }

            for (int i = 0; i < mNumThreads; ++i){
                std::thread * first = new std::thread(ThreadRoutine, mFilter);
                mWorkerThreads.push_back(first);
            }

            mFilter.clear();
            mImage = "";
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
         }
#ifdef __cplusplus
}  //  namespace FilterAPI
}  //  extern "C" {
#endif
