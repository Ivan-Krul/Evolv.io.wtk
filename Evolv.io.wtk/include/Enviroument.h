#pragma once
#include <vector>
#include <bitset>

#include "raylib.h"

#include "ComponentPropertyKit.h"

#include "EnviroumentWaterService.h"
#include "EnviroumentTemperatureService.h"

class Enviroument {
public:
    struct Tile { // because of budget cuts
        uint8_t height; // in procents
        uint8_t plantFillness; // in procents
        uint8_t steepnessLevel; // in procents
    };

    Enviroument(size_t sizeX, size_t sizeY);
    inline _NODISCARD EnviroumentWaterService& getWaterService() noexcept { return mEWaterService; }
    inline _NODISCARD EnviroumentTemperatureService& getTemperatureService() noexcept { return mETemperatureService; }
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
    bool chechIsUnfinishedCheckList() const noexcept;
    void handleUnfinishedCheckList() const noexcept;

    void calculateSteepnessLevel();

    size_t mWidth;
    size_t mHeight;
    std::vector<Tile> mTileMap;
    Image mImage;

    std::bitset<6> mCheckList;

    float mOscillator = 0;

    EnviroumentWaterService mEWaterService;
    EnviroumentTemperatureService mETemperatureService;

    float mGrowRatio; // how fast a plant would grow if all conditions are satisfied (sun or rain, low height, perfect temperature)
    int16_t mMinTempForPlantsLiving;
    int16_t mMaxTempForPlantsLiving;
    uint16_t mSunBooster; // in procent
    uint16_t mRainBooster; // in procent
    uint16_t mCloudHeight; // in procent
};
