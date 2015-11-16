/* Copyright (C) 2015 Christian Fischer */
#ifndef FILTER_LIBRARY_H__
#define FILTER_LIBRARY_H__

// The following ifdef block is the standard way of creating macros which make
// exporting from a DLL simpler. All files within this DLL are compiled with
// the FILTER_API symbol defined on the command line. This symbol
// should not be defined on any project that uses this DLL. This way any other
// project whose source files include this file see
// FILTER_API functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.
#ifndef __linux
#define FILTER_API __declspec(dllexport)
#else
#define FILTER_API
#endif


#include <vector>

#ifdef __cplusplus
extern "C" {
    namespace FilterAPI {
#endif

class FILTER_API Filter{
public:
    static void setFilter(const std::vector<float>& vFilter, unsigned int uNumThreads);

    static bool loadImage(const std::string& source);

    static void applyFilter();
private:
    static std::vector<float> mFilter;

    static std::string mImage;
};

#ifdef __cplusplus
}  //  namespace FILTER_API
}  //  extern "C" {
#endif

#endif // FILTER_LIBRABRY_H__
