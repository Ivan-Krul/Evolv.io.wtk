#pragma once
#include <vector>
#include <stdint.h>

class EnviroumentTerrainService {
public:
    EnviroumentTerrainService();

    void generate(size_t width, size_t height, size_t iters);
    void calculateSteepnessLevel(float steepCoef);

    inline bool isReady() const noexcept { return mIsReady; }

    size_t getSize() const noexcept;

    uint8_t getHeight(size_t index) const noexcept;
    float getSteepness(size_t index) const noexcept;

private:
    bool mIsReady;

    size_t mWidth;
    size_t mHeight;

    std::vector<uint8_t> mHeightMap;
    std::vector<float> mSteepnessMap;
};

