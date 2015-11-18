/* Copyright (C) 2015 Christian Fischer */
#ifndef FILTER_STATISTICS_H__
#define FILTER_STATISTICS_H__

#include <mutex>


class FilterStatistics {
public:
    static void IncreaseNumberOfProcessedImages();

    static void ResetNumberOfProcessedImages();

    static long long GetNumberOfProcessedImages();
private:
    static long long lNumberOfProcessedImages;

    static std::mutex mLock;
};

#endif // FILTER_STATISTICS_H__
