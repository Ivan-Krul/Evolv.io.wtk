#include "EnviroumentTerrainService.h"
#include "PlainGeneratorComponent.h"
#include "Utils.h"

EnviroumentTerrainService::EnviroumentTerrainService() {
    mIsReady = false;
    mWidth = 0;
    mHeight = 0;
}

void EnviroumentTerrainService::generate(size_t width, size_t height, size_t iters) {
    mWidth = width;
    mHeight = height;

    auto prop = component_property_kit_lib::Properties();
    auto pgc = PlainGenComponent(prop, 0, typeid(PlainGenComponent).hash_code());
    pgc.init();

    auto map = pgc.generatePerlinChunk<component_property_kit_lib::SmootherSmoothLinear>(0.5, 0.5, iters, mWidth, mHeight);
    mHeightMap.resize(map.size());

    for (size_t i = 0; i < map.size(); i++) {
        mHeightMap[i] = map[i] * 256.0f;
    }

    mIsReady = true;
}

void EnviroumentTerrainService::calculateSteepnessLevel(float steepCoef) {
    mSteepnessMap.resize(mHeightMap.size());

    auto isInMap = [=](size_t index, int x, int y) {
        size_t ix = index % mWidth;
        size_t iy = index / mWidth;
        return (0 <= (ix + x)) && ((ix + x) < mWidth) && (0 <= (iy + y)) && ((iy + y) < mHeight);
    };

    uint8_t valids = 1;

    for (size_t i = 0; i < mHeightMap.size(); i++) {
        mSteepnessMap[i] = mHeightMap[i];
        valids = 1;
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                if (isInMap(i, x, y)) {
                    valids++;
                    mSteepnessMap[i] += abs(mHeightMap[i] - mHeightMap[i + x + y * mWidth]) * steepCoef;
                }
            }
        }

        mSteepnessMap[i] = slice<float>(mSteepnessMap[i] / valids, 0.f, 255.f) / 256.f;
    }
}

size_t EnviroumentTerrainService::getSize() const noexcept {
    return mHeightMap.size();
}

uint8_t EnviroumentTerrainService::getHeight(size_t index) const noexcept {
    if(index >= mHeightMap.size())
        return 0;
    return mHeightMap[index];
}

float EnviroumentTerrainService::getSteepness(size_t index) const noexcept {
    if (index >= mSteepnessMap.size())
        return 0;
    return mSteepnessMap[index];
}
