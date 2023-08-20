#pragma once
#include <vector>
#include <bitset>

#include "raylib.h"

#include "ComponentPropertyKit.h"

class Enviroument {
public:
    struct Tile { // because of budget cuts
        uint8_t height; // in procents
        uint8_t plantFillness; // in procents
        uint8_t steepnessLevel; // in procents
    };

    Enviroument(size_t sizeX, size_t sizeY);
    void setSeaLevel(uint16_t seaLevel) noexcept;
    void setTides(uint16_t tideAmplitude, uint16_t tideFrequency) noexcept;
    void setSeasonFrequency(float seasonFrequency) noexcept;
    void setTempratureRange(int16_t tempMinInLowest, uint16_t heightTempDifference, int16_t tempMaxInLowest) noexcept;
    void setTempratureForPlantsRange(int16_t tempPlantMin, int16_t tempPlantMax) noexcept;
    void setGrowRatio(float ratio) noexcept;
    void setBoosters(uint16_t sunBoostr, uint16_t rainBooster) noexcept;
    void setCloudHeight(uint16_t cloudHeight) noexcept;
    void step(float wideness);

    void generateImage();
    float getOscillator();
    _NODISCARD Image& getImage();

    ~Enviroument() {
        UnloadImage(mImage);
    }
private:
    void handleUnfinishedCheckList() noexcept;

    template<typename T>
    T scale(float x, T min, T max);
    template<typename T>
    T findZeroFromScale(T min, T max);
    template<typename T>
    T fit(T x, T min, T max);
    template<typename T>
    inline T findZeroFromFit(T min, T max) { return min; }
    template<typename T>
    T slice(T x, T min, T max) const noexcept;

    void calculateIceCapLevel();
    int16_t getCurrentTemprature(uint8_t height);
    void calculateSteepnessLevel();

    size_t mWidth;
    size_t mHeight;
    std::vector<Tile> mTileMap;
    Image mImage;

    std::bitset<8> mCheckList;

    uint16_t mSeaLevel; // in procents
    uint16_t mTideFrequency;
    uint16_t mTideAmplitude;
    float mTideRatio = 0.5;

    float mSeasonFrequency;
    float mOscillator = 0;
    float mSeasonRatio = 0.5;
    uint16_t mHeightDifference; // in temprature from the lowest point
    int16_t mTempMaxInLowest;
    int16_t mTempMinInLowest;
    int16_t mIceCapLevel;
    
    float mGrowRatio; // how fast a plant would grow if all conditions are satisfied (sun or rain, low height, perfect temperature)
    int16_t mMinTempForPlantsLiving;
    int16_t mMaxTempForPlantsLiving;
    uint16_t mSunBooster; // in procent
    uint16_t mRainBooster; // in procent
    uint16_t mCloudHeight; // in procent
};

template<typename T>
inline T Enviroument::scale(float x, T min, T max) {
    return (T)(x * (max - min)) + min;
}

template<typename T>
inline T Enviroument::findZeroFromScale(T min, T max) {
    return -min / (max - min);
}

template<typename T>
inline T Enviroument::fit(T x, T min, T max) {
    return (x - min) / (max - min);
}

template<typename T>
inline T Enviroument::slice(T x, T min, T max) const noexcept {
    return std::min(std::max(x, min), max);
}
