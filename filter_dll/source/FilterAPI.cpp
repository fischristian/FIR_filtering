/* Copyright (C) 2015 Christian Fischer */
#include <vector>
#include <iostream>
#include "../include/FilterAPI.h"

        std::vector<float>FilterAPI::Filter::mFilter;
        std::string FilterAPI::Filter::mImage = "";

#ifdef __cplusplus
extern "C" {
    namespace FilterAPI {
#endif
        // std::vector<float>Filter::mFilter;
        // std::string Filter::mImage = "";

        void Filter::setFilter(const std::vector<float>& vFilter, unsigned int uNumThreads) {
            if (vFilter.empty()){
                throw - 1;
            }
            if (uNumThreads == 0){
                throw - 2;
            }
            std::cout << "Filter::setFilter called \n";
            Filter::mFilter = vFilter;
        }

        bool Filter::loadImage(const std::string& source) {
            if (source == ""){
                throw - 1;
            }
            std::cout << "Filter::loadImage called \n";
            mImage = source;
            return true;
        }

        void Filter::applyFilter(){
            if (mFilter.empty()) {
                throw -1;
            }
            if (mImage == "") {
                throw -2;
            }

            // create a test image:
            // height = 480, width = 640
            int ImageSize = 480 * 640;
            unsigned char * pImage = new unsigned char[ImageSize];
            for (int i = 0; i < ImageSize; ++i) {
                pImage[i] = (unsigned char)(i % 255);
                // std::cout << "pImage: " << pImage[i] << "\n";
            }
            std::vector<float>test_coff(Filter::mFilter);

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

            int iNumCoff = test_coff.size();
            for (int iPxl = 0; iPxl < ImageSize; ++iPxl) {
                if ((iPxl % 640) < (640 - iNumCoff)) {
                    // std::cout << "Current Pixel: "<< (iPxl % 640) << " \n";
                    // std::cout << "PixelValue = " << *(pImage + iPxl) << " \n";
                    for (int i = 0; i < iNumCoff; ++i) {
                        pFilteredImage[iPxl] += pImage[iPxl] * test_coff[i];
                    }
                }
            }


            mFilter.clear();
            mImage = "";
        }
#ifdef __cplusplus
}  //  namespace FilterAPI
}  //  extern "C" {
#endif
