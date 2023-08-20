#include "EnviroumentCloudService.h"

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
    mIsSea.resize(mCloudness.size(), 0);
    mIsMountain.resize(mCloudness.size(), 255);
    mCheckList[0] = true;
}

void EnviroumentCloudService::setCloudHeight(uint8_t cloudHeight) {
    mCloudHeight = cloudHeight;
    mCheckList[1] = true;
}

void EnviroumentCloudService::setTopologyOfRegions(const EnviroumentTerrainService& terrainService, const EnviroumentWaterService& waterService) {
    for (size_t i = 0; i < mCloudness.size(); i++) {
        mIsSea[i] = waterService.getLowestLevel() > terrainService.getHeight(i);
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
    // then we're blow them
    // and then, we're spending them

    float curWindX = mWindDirX * widness;
    float curWindY = mWindDirY * widness;

    // TODO: make gaining the rest of EnviroumentCloudService::takeStep
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
