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
    void setWind(int8_t windX, int8_t windY);
    
    bool isReady() const noexcept;

    void takeStep(float widness);

    size_t getSize() const noexcept;
    const std::vector<CloudnessValue>& getCloudness() const noexcept;
    const std::vector<uint8_t>& getRainForce() const noexcept;

private:
    float calculateWindForce(const std::vector<CloudnessValue>& image, size_t index, float mult) const noexcept;

    std::bitset<6> mCheckList;

    uint8_t mCloudHeight; // in procent
    uint8_t mTriggerRain;
    uint8_t mTriggerChainReaction;

    size_t mWidth;
    size_t mHeight;
    std::vector<size_t> mSeaCoord;
    std::vector<bool> mIsMountain;
    std::vector<CloudnessValue> mCloudness;
    std::vector<uint8_t> mRainForce;
    std::vector<size_t> mSpendCoord;

    int8_t mWindDirX;
    int8_t mWindDirY;

    float mRandWindX;
    float mRandWindY;

    component_property_kit_lib::PropertyList propList;

    const uint8_t cCloudIncome = 1;
};

