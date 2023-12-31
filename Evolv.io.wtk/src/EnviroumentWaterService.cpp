#include "EnviroumentWaterService.h"

EnviroumentWaterService::EnviroumentWaterService() {
    mCheckList = 0;
    mSeaLevel = 0;
    mTideAmplitude = 0;
    mTideFrequency = 0;
}

void EnviroumentWaterService::setSeaLevel(uint16_t seaLevel) noexcept {
    mSeaLevel = seaLevel;
    mCheckList[0] = true;
}

void EnviroumentWaterService::setTides(float tideAmplitude, float tideFrequency) noexcept {
    mTideAmplitude = tideAmplitude;
    mTideFrequency = tideFrequency;
    mCheckList[1] = true;
}

bool EnviroumentWaterService::isReady() const noexcept {
    return !(~mCheckList.count());
}

uint16_t EnviroumentWaterService::calculateSeaLevel() const noexcept {
    return mSeaLevel + mTideRatio * mTideAmplitude;
}

uint16_t EnviroumentWaterService::getHighestLevel() const noexcept {
    return mTideAmplitude + mSeaLevel;
}

uint16_t EnviroumentWaterService::getLowestLevel() const noexcept {
    return mSeaLevel - mTideAmplitude;
}

void EnviroumentWaterService::takeStep(float oscillator) {
    mTideRatio = sin(oscillator * PI * mTideFrequency);
}
