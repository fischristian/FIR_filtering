/* Copyright (C) 2015 Christian Fischer */
#ifndef FILTER_LIBRARY_H__
#define FILTER_LIBRARY_H__

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
    * \brief     Flags all running thread to stop their process. Also releases the source image buffer.
    * \return
    * \exception does not throw
    */
    static void Stop();

   /* \fn        getNumberOfPrcessedImages
    * \param     none
    * \brief     Get the total number of filtered images
    * \return    long long number of filtered images
    * \exception does not throw
    */
    static long long getNumberOfPrcessedImages();

   /* \fn        Release
    * \param     none
    * \brief     Release internally used resources
    * \return    none
    * \exception does not throw
    */
    static void Release();

private:
    static std::vector<float> mFilter;

    static unsigned char * mImage;

    static unsigned int uImageSize;

    static unsigned int mNumThreads;

    static bool bTerminateFlag;

    static bool bSettingsChanged;

    static std::vector<std::thread*>mWorkerThreads;

    static void ThreadRoutine();
};

#ifdef __cplusplus
}  //  namespace FILTER_API
}  //  extern "C" {
#endif

#endif // FILTER_LIBRABRY_H__
