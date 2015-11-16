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
            return true;
        }

        void Filter::applyFilter(){
            if (mFilter.empty()) {
                throw -1;
            }
            if (mImage == "") {
                throw -2;
            }

            mFilter.clear();
            mImage = "";
        }
#ifdef __cplusplus
}  //  namespace FilterAPI
}  //  extern "C" {
#endif
