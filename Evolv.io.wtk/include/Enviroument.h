#pragma once
#include <vector>
#include <bitset>

#include "raylib.h"

#include "ComponentPropertyKit.h"

#include "EnviroumentWaterService.h"
#include "EnviroumentTemperatureService.h"
#include "EnviroumentTerrainService.h"
#include "EnviroumentCloudService.h"

class Enviroument {
public:
    Enviroument(size_t sizeX, size_t sizeY);

    inline _NODISCARD EnviroumentWaterService& getWaterService() noexcept { return mEWaterService; }
    inline _NODISCARD EnviroumentTemperatureService& getTemperatureService() noexcept { return mETemperatureService; }
    inline _NODISCARD EnviroumentTerrainService& getTerrainService() noexcept { return mETerrainService; }
    inline _NODISCARD EnviroumentCloudService& getCloudService() noexcept { return mECloudService; }
    void setTempratureForPlantsRange(int16_t tempPlantMin, int16_t tempPlantMax) noexcept;
    void setGrowRatio(float ratio) noexcept;
    void setBoosters(uint16_t sunBoostr, uint16_t rainBooster) noexcept;
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

    size_t mWidth;
    size_t mHeight;
    Image mImage;

    std::bitset<4> mCheckList;

    float mOscillator = 0;

    EnviroumentTerrainService mETerrainService;
    EnviroumentWaterService mEWaterService;
    EnviroumentTemperatureService mETemperatureService;
    EnviroumentCloudService mECloudService;

    float mGrowRatio; // how fast a plant would grow if all conditions are satisfied (sun or rain, low height, perfect temperature)
    int16_t mMinTempForPlantsLiving;
    int16_t mMaxTempForPlantsLiving;
    uint16_t mSunBooster; // in procent
    uint16_t mRainBooster; // in procent
};
