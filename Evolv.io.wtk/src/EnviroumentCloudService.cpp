#include "EnviroumentCloudService.h"
#include "Utils.h"

EnviroumentCloudService::EnviroumentCloudService() {
    mCheckList = 0;

    mCloudHeight = 0; // in procent
    mTriggerRain = 0;
    mTriggerChainReaction = 0;

    mWindDirX = 0;
    mWindDirY = 0;
}

void EnviroumentCloudService::setPlain(size_t width, size_t height) {
    mWidth = width;
    mHeight = height;
    mCloudness.resize(width * height, 0);
    mRainForce.resize(mCloudness.size(), 0);
    mIsMountain.resize(mCloudness.size(), 255);
    mCheckList[0] = true;
}

void EnviroumentCloudService::setCloudHeight(uint8_t cloudHeight) {
    mCloudHeight = cloudHeight;
    mCheckList[1] = true;
}

void EnviroumentCloudService::setTopologyOfRegions(const EnviroumentTerrainService& terrainService, const EnviroumentWaterService& waterService) {
    for (size_t i = 0; i < mCloudness.size(); i++) {
        if (waterService.getLowestLevel() > terrainService.getHeight(i))
            mSeaCoord.push_back(i);
        mIsMountain[i] = terrainService.getHeight(i) < mCloudHeight;
    }

    mCheckList[2] = true;
}

void EnviroumentCloudService::setTriggerRain(uint8_t triggerForRain) {
    mTriggerRain = triggerForRain;
    mCheckList[3] = true;
}

void EnviroumentCloudService::setTriggerChainReaction(uint8_t triggerForChainReaction) {
    mTriggerChainReaction = triggerForChainReaction;
    mCheckList[4] = true;
}

void EnviroumentCloudService::setWind(int8_t windX, int8_t windY) {
    mWindDirX = windX;
    mWindDirY = windY;
    mCheckList[5] = true;
}

bool EnviroumentCloudService::isReady() const noexcept {
    return !(~mCheckList.count());
}

void EnviroumentCloudService::takeStep(float widness) {
    // first we're gaining some steam
    for (auto& index : mSeaCoord) {
        addExpectedOverflowAndUnderflow(mCloudness[index], cCloudIncome);
    }

    for (size_t i = 0; i < mWidth; i++) {
        mCloudness[i] = rand();
        mCloudness[i + mWidth * (mHeight - 1)] = rand();
    }

    for (size_t i = 0; i < mHeight; i++) {
        mCloudness[i * mWidth] = rand();
        mCloudness[mWidth - 1 + i * mWidth] = rand();
    }

    // then we're blow them
    static auto image = mCloudness;
    image = mCloudness;
    mSpendCoord.clear();

    float curWind = mWindDirX * widness * mWindDirY;

    int windDirX = mWindDirX > 0 ? ceil(mWindDirX) : floor(mWindDirX);
    int windDirY = mWindDirY > 0 ? ceil(mWindDirY) : floor(mWindDirY);

    uint8_t pocket;

    float forceWind = curWind;

    for (size_t i = 0; i < mCloudness.size(); i++) {
        pocket = image[i];
        forceWind = curWind * (1.f - (float)pocket / 512.f) * curWind;
        
        if (isInsideOfMap(i, windDirX, windDirY, mWidth, mHeight)) {
            addExpectedOverflowAndUnderflow(mCloudness[i + windDirX + windDirY * mWidth], pocket * forceWind);
        }
        addExpectedOverflowAndUnderflow(mCloudness[i], -(pocket * forceWind));
        if (image[i] > mTriggerRain)
            mSpendCoord.push_back(i);
    }

    // and then, we're spending them
    for (auto& index : mSpendCoord) {
        addExpectedOverflowAndUnderflow(mCloudness[index], mTriggerRain - mCloudness[index] * 0.5);
    }

}

size_t EnviroumentCloudService::getSize() const noexcept {
    return mCloudness.size();
}

uint8_t EnviroumentCloudService::getCloudness(size_t index) const noexcept {
    if(index >= mCloudness.size())
        return 0;
    return mCloudness[index];
}

uint8_t EnviroumentCloudService::getRainForce(size_t index) const noexcept {
    if (index >= mRainForce.size())
        return 0;
    return mRainForce[index];
}
