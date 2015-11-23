/* Copyright (C) 2015 Christian Fischer */
#ifndef FILTER_STATISTICS_H__
#define FILTER_STATISTICS_H__

#include <mutex>


class FilterStatistics {
public:
    /* \fn       IncreaseNumberOfProcessedImages
    * \param     none
    * \brief     Increase threadsafe the number of processed images
    * \return    void
    * \exception std::string * ex if the filter coefficients are in sum > 1, length of filter coefficients or number of threads is zero
    */
    static void IncreaseNumberOfProcessedImages();

    /* \fn       ResetNumberOfProcessedImages
    * \param     none
    * \brief     Reset the number of processed images to zero
    * \return    void
    * \exception std::string * ex if the filter coefficients are in sum > 1, length of filter coefficients or number of threads is zero
    */
    static void ResetNumberOfProcessedImages();

    /* \fn       GetNumberOfProcessedImages
    * \param     none
    * \brief     Get threadsafe the current number of processed images
    * \return    void
    * \exception std::string * ex if the filter coefficients are in sum > 1, length of filter coefficients or number of threads is zero
    */
    static long long GetNumberOfProcessedImages();
private:
    static long long lNumberOfProcessedImages;

    static std::mutex mLock;
};

#endif // FILTER_STATISTICS_H__
