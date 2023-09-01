#include "EnviroumentCloudService.h"
#include "Utils.h"
#include "TimeStop.h"

EnviroumentCloudService::EnviroumentCloudService() {
    mCheckList = 0;

    mCloudHeight = 0; // in procent
    mTriggerRain = 0;
    mTriggerChainReaction = 0;

    mPropList.pushComponent<component_property_kit_lib::PlainGeneratorComponent>();
}

void EnviroumentCloudService::setPlain(size_t width, size_t height) {
    mWidth = width;
    mHeight = height;
    mPlainSize = width * height;
    mCloudness = new CloudnessValue[mPlainSize];
    mCloudnessBuffer = new CloudnessValue[mPlainSize];
    mRainForce = new CloudnessValue[mPlainSize];
    mIsMountain = new bool[mPlainSize];
    mIsSea = new bool[mPlainSize];
    mWindDir = new Vector2[mPlainSize];

    memset(mCloudness, 0, mPlainSize * sizeof(CloudnessValue));
    memset(mCloudnessBuffer, 0, mPlainSize * sizeof(CloudnessValue));
    memset(mRainForce, 0, mPlainSize * sizeof(CloudnessValue));
    memset(mIsMountain, false, mPlainSize);
    memset(mIsSea, false, mPlainSize);
    // uninitialized values of mWindDir
    mCheckList[0] = true;
}

void EnviroumentCloudService::setCloudHeight(uint8_t cloudHeight) {
    mCloudHeight = cloudHeight;
    mCheckList[1] = true;
}

void EnviroumentCloudService::setTopologyOfRegions(const EnviroumentTerrainService& terrainService, const EnviroumentWaterService& waterService) {
    for (size_t i = 0; i < mPlainSize; i++) {
        mIsSea[i] = waterService.getLowestLevel() > terrainService.getHeight(i);
        mIsMountain[i] = terrainService.getHeight(i) > mCloudHeight;
        mCloudness[i] = 0;
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

void EnviroumentCloudService::setWind(float windX, float windY) {
    static auto& pgc = mPropList.getComponent<component_property_kit_lib::PlainGeneratorComponent>(0);
    auto windsX = pgc.generatePerlinChunk<component_property_kit_lib::smoother::SmootherSmoothLinear>(.5f, .5f, 4, mWidth, mHeight); // 0 - 1
    auto windsY = pgc.generatePerlinChunk<component_property_kit_lib::smoother::SmootherSmoothLinear>(.5f, .5f, 4, mWidth, mHeight); // 0 - 1

    for (size_t i = 0; i < mPlainSize; i++) {
        mWindDir[i].x = (windsX[i] * 2.f - 1.f) + windX;
        mWindDir[i].y = (windsY[i] * 2.f - 1.f) + windY;
    }
    mCheckList[5] = true;
}

bool EnviroumentCloudService::isReady() const noexcept {
    return !(~mCheckList.count());
}

void EnviroumentCloudService::takeStep(float widness) {
    TimeStop tsCopy;
    memcpy(mCloudnessBuffer, mCloudness, mPlainSize * sizeof(CloudnessValue));
    printf("Copy: ");
    tsCopy.stop();

    float windForce;
    CloudnessValue* cloudSrc;
    CloudnessValue* cloudDst;

    TimeStop tsIter;
    for (size_t i = 0; i < mPlainSize; i++) {
        cloudSrc = &mCloudness[i];
        cloudDst = &mCloudness[i + (size_t)mWindDir[i].x + (size_t)mWindDir[i].y * mWidth];

        if(mIsSea[i])
            *cloudSrc = std::min(*cloudSrc + cCloudIncome, 255);

        windForce = calculateWindForce(i, widness);

        if (isInsideOfMap(i, mWindDir[i].x, mWindDir[i].y, mWidth, mHeight))
            *cloudDst = std::min<CloudnessValue>(*cloudDst + windForce, 255);
        *cloudSrc = std::max<CloudnessValue>(*cloudSrc - windForce, 0);

        if (*cloudSrc > mTriggerRain || mIsMountain[i])
            *cloudSrc = std::max(*cloudSrc - 5, 0);
    }
    printf("Iter: ");
    tsIter.stop();
}

size_t EnviroumentCloudService::getSize() const noexcept {
    return mPlainSize;
}

EnviroumentCloudService::CloudnessValue* EnviroumentCloudService::getCloudness() const noexcept {
    return mCloudness;
}

EnviroumentCloudService::CloudnessValue* EnviroumentCloudService::getRainForce() const noexcept {
    return mRainForce;
}

float EnviroumentCloudService::calculateWindForce(size_t index, float mult) const noexcept {
    static float movementSpeed;
    movementSpeed = (1.f - (float)mCloudnessBuffer[index]/256.f) * mult;

    return abs(movementSpeed * mWindDir[index].x * mWindDir[index].y);
}
