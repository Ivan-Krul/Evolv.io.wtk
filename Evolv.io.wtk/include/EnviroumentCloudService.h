#pragma once
#include <stdint.h>
#include <bitset>
#include <vector>

#include "EnviroumentTerrainService.h"
#include "EnviroumentWaterService.h"
#include "ComponentPropertyKit.h"

class EnviroumentCloudService {
public:
    typedef int16_t CloudnessValue;

    EnviroumentCloudService();
    void setPlain(size_t width, size_t height);
    void setCloudHeight(uint8_t cloudHeight);
    void setTopologyOfRegions(const EnviroumentTerrainService& terrainService, const EnviroumentWaterService& waterService);
    void setTriggerRain(uint8_t triggerForRain);
    void setTriggerChainReaction(uint8_t triggerForChainReaction);
    void setWind(float windX, float windY);
    
    bool isReady() const noexcept;

    void takeStep(float widness);

    size_t getSize() const noexcept;
    CloudnessValue* getCloudness() const noexcept;
    CloudnessValue* getRainForce() const noexcept;

    ~EnviroumentCloudService() {
        delete[] mIsSea, mIsMountain, mCloudness, mRainForce, mWindDir, mBorderGeneratorCoords;
    }
private:
    float calculateWindForce(size_t index, float mult) const noexcept;
    void generateBorders() noexcept;

    std::bitset<6> mCheckList;

    uint8_t mCloudHeight; // in procent
    uint8_t mTriggerRain;
    uint8_t mTriggerChainReaction;

    size_t mWidth;
    size_t mHeight;
    size_t mPlainSize;
    size_t mBorderSize;

    bool* mIsSea;
    bool* mIsMountain;
    CloudnessValue* mCloudness;
    CloudnessValue* mCloudnessBuffer;
    CloudnessValue* mRainForce;
    Vector2* mWindDir;
    size_t* mBorderGeneratorCoords;

    component_property_kit_lib::PropertyList mPropList;

    const uint8_t cCloudIncome = 5;
};

