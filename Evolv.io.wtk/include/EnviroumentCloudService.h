#pragma once
#include <stdint.h>
#include <bitset>
#include <vector>

#include "EnviroumentTerrainService.h"
#include "EnviroumentWaterService.h"

class EnviroumentCloudService {
public:
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
    uint8_t getCloudness(size_t index) const noexcept;
    uint8_t getRainForce(size_t index) const noexcept;

private:
    std::bitset<6> mCheckList;

    uint8_t mCloudHeight; // in procent
    uint8_t mTriggerRain;
    uint8_t mTriggerChainReaction;

    size_t mWidth;
    size_t mHeight;
    std::vector<bool> mIsSea;
    std::vector<bool> mIsMountain;
    std::vector<uint8_t> mCloudness;
    std::vector<uint8_t> mRainForce;

    int8_t mWindDirX;
    int8_t mWindDirY;
};

