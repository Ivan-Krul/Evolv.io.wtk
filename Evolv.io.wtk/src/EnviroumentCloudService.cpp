#include "EnviroumentCloudService.h"
#include "Utils.h"

EnviroumentCloudService::EnviroumentCloudService() {
    mCheckList = 0;

    mCloudHeight = 0; // in procent
    mTriggerRain = 0;
    mTriggerChainReaction = 0;

    mWindDirX = 0;
    mWindDirY = 0;

    propList.pushComponent<component_property_kit_lib::RandomGeneratorComponent>();
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
        mIsMountain[i] = terrainService.getHeight(i) > mCloudHeight;
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
    mRandWindX = 0;
    mRandWindY = 0;
    mCheckList[5] = true;
}

bool EnviroumentCloudService::isReady() const noexcept {
    return !(~mCheckList.count());
}

void EnviroumentCloudService::takeStep(float widness) {
    static auto& rng = propList.getComponent<component_property_kit_lib::RandomGeneratorComponent>(0);
    static auto image = mCloudness;
    image = mCloudness;
    mSpendCoord.clear();

    // first we're gaining some steam
    for (auto& index : mSeaCoord) {
        mCloudness[index] = mCloudness[index] > cCloudIncome ? 255 : mCloudness[index] + cCloudIncome;
    }

    for (size_t i = 0; i < mWidth; i++) {
        mCloudness[i] = rng.next<uint8_t>();
        mCloudness[i + mWidth * (mHeight - 1)] = rng.next<uint8_t>();
    }

    for (size_t i = 0; i < mHeight; i++) {
        mCloudness[i * mWidth] = rng.next<uint8_t>();
        mCloudness[mWidth - 1 + i * mWidth] = rng.next<uint8_t>();
    }

    // then we're blow them
    mRandWindX += std::max(std::min(rng.next(-0.01f, 0.01f), 1.f),-1.f);
    mRandWindY += std::max(std::min(rng.next(-0.01f, 0.01f), 1.f), -1.f);

    float curWindX = mRandWindX + mWindDirX;
    float curWindY = mRandWindY + mWindDirY;

    float curWind = curWindX * widness * curWindY;

    int windDirX = curWindX > 0 ? ceil(curWindX) : floor(curWindX);
    int windDirY = curWindY > 0 ? ceil(curWindY) : floor(curWindY);

    CloudnessValue pocket;

    float forceWind = 0.f;

    for (size_t i = 0; i < mCloudness.size(); i++) {
        forceWind = calculateWindForce(image, i, .5f) * curWind;
    
        if (isInsideOfMap(i,windDirX, windDirY, mWidth, mHeight)) {
            mCloudness[i + windDirX + windDirY * mWidth] = std::min<CloudnessValue>(image[i] + forceWind,255);
        }
        if(mCloudness[i] != 0)
            mCloudness[i] = std::max<CloudnessValue>(image[i] - forceWind, 0);
    
        if (((image[i] > mTriggerRain) || mIsMountain[i]) && mCloudness[i] != 0)
            mSpendCoord.push_back(i);
    }
    
    // and then, we're spending them
    for (auto& index : mSpendCoord) {
        if(mCloudness[index] != 0)
            mCloudness[index] -= 5;
    }

    //for (size_t i = 0; i < mCloudness.size(); i++) {
    //    mCloudness[i] = std::max<int16_t>(std::min<int16_t>(mCloudness[i], 255), 0);
    //}

}

size_t EnviroumentCloudService::getSize() const noexcept {
    return mCloudness.size();
}

const std::vector<EnviroumentCloudService::CloudnessValue>& EnviroumentCloudService::getCloudness() const noexcept {
    return mCloudness;
}

const std::vector<uint8_t>& EnviroumentCloudService::getRainForce() const noexcept {
    return mRainForce;
}

float EnviroumentCloudService::calculateWindForce(const std::vector<CloudnessValue>& image, size_t index, float mult) const noexcept {
    return 256.f - image[index]*mult;
}
