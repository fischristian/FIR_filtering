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

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <string.h>
#include <vector>

#ifdef __cplusplus
extern "C" {
    namespace FilterAPI {
#endif

/*!
 * class filter
 */
class FILTER_API Filter{
public:
    /* \fn        configureFilter
     * \param
     * \brief
     * \return
     * \exception std::string * ex if the filter coefficients are in sum > 1, length of filter coefficients or number of threads is zero
     */
    static void configureFilter(const std::vector<float>& vFilter, unsigned int uNumThreads);

   /* \fn         loadImage
    * \param
    * \brief
    * \return
    * \remark     File must be raw hex data, both lower / upper case possible; any non-hex value is ignored and substituted by 0
    * \exception std::string * ex if image file is invalid, or is empty
    */
    static bool loadImage(const std::string& source);

   /* \fn         Start
    * \param
    * \brief
    * \return
    * \exception std::string * ex if filter and image was not initialized before
    */
    static void Start();

   /* \fn         Stop
    * \param
    * \brief
    * \return
    * \exception does not throw
    */
    static void Stop();

   /* \fn         Stop
    * \param
    * \brief
    * \return
    * \exception does not throw
    */
    static long long getNumberOfPrcessedImages();
private:
    static std::vector<float> mFilter;

    static unsigned char * mImage;

    static unsigned int mNumThreads;

    static std::vector<std::thread*>mWorkerThreads;
};

#ifdef __cplusplus
}  //  namespace FILTER_API
}  //  extern "C" {
#endif

#endif // FILTER_LIBRABRY_H__
