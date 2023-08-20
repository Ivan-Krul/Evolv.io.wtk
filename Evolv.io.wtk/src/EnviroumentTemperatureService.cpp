#include "EnviroumentTemperatureService.h"
#include "Utils.h"
#include "raylib.h"

void EnviroumentTemperatureService::setSeasonFrequency(float seasonFrequency) noexcept {
    mSeasonFrequency = seasonFrequency;
    mCheckList[0] = true;
}

void EnviroumentTemperatureService::setTempratureRange(int16_t tempMinInLowest, uint16_t heightTempDifference, int16_t tempMaxInLowest) noexcept {
    mTempMinInLowest = tempMinInLowest;
    mTempMaxInLowest = tempMaxInLowest;
    mHeightDifference = heightTempDifference;
    calculateIceCapLevel();
    mCheckList[1] = true;
}

bool EnviroumentTemperatureService::isReady() const noexcept {
    return !(~mCheckList.count());
}

void EnviroumentTemperatureService::takeStep(float oscillator) {
    mSeasonRatio = fit<float>(sin(oscillator * PI * mSeasonFrequency), -1, 1);
    calculateIceCapLevel();
}

uint16_t EnviroumentTemperatureService::getIceCapLevel() const noexcept {
    return mIceCapLevel;
}

int16_t EnviroumentTemperatureService::getCurrentTemperature(uint8_t height) {
    float heightDiff = (float)height / 256.f;
    uint16_t currentLowest = scale<float>(mSeasonRatio, mTempMinInLowest, mTempMaxInLowest);
    return scale<float>(heightDiff, currentLowest, currentLowest - mHeightDifference);
}

void EnviroumentTemperatureService::calculateIceCapLevel() {
    float currentLowest = scale<float>(mSeasonRatio, mTempMinInLowest, mTempMaxInLowest);
    float height = findZeroFromScale<float>(currentLowest, currentLowest - mHeightDifference) * 256.f;
    mIceCapLevel = slice<int16_t>(height, 0, 256);
}
