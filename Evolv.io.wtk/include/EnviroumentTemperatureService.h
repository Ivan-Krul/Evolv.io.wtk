#pragma once
#include <stdint.h>
#include <bitset>

class EnviroumentTemperatureService {
public:
    void setSeasonFrequency(float seasonFrequency) noexcept;
    void setTempratureRange(int16_t tempMinInLowest, uint16_t heightTempDifference, int16_t tempMaxInLowest) noexcept;
    
    bool isReady() const noexcept;

    void takeStep(float oscillator);
    _NODISCARD uint16_t getIceCapLevel() const noexcept;
    _NODISCARD int16_t getCurrentTemperature(uint8_t height);

private:
    void calculateIceCapLevel();

    std::bitset<3> mCheckList;

    float mSeasonFrequency;
    float mSeasonRatio = 0.5;
    uint16_t mHeightDifference; // in temprature from the lowest point
    int16_t mTempMaxInLowest;
    int16_t mTempMinInLowest;
    int16_t mIceCapLevel;
};

