/* Copyright (C) 2015 Christian Fischer */

#include "../include/FilterStatistics.h"

long long FilterStatistics::lNumberOfProcessedImages = 0;

std::mutex FilterStatistics::mLock;
/*****************************************************************************/
void FilterStatistics::IncreaseNumberOfProcessedImages() {
    mLock.lock();
    lNumberOfProcessedImages++;
    mLock.unlock();
}
/*****************************************************************************/
void FilterStatistics::ResetNumberOfProcessedImages() {
    mLock.lock();
    lNumberOfProcessedImages = 0;
    mLock.unlock();
}
/*****************************************************************************/
long long FilterStatistics::GetNumberOfProcessedImages() {
    long long tmpNumberOfProcessedImages = 0;
    mLock.lock();
    tmpNumberOfProcessedImages = lNumberOfProcessedImages;
    mLock.unlock();
    return tmpNumberOfProcessedImages;
}