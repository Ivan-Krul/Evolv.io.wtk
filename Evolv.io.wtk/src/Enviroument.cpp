#include "Enviroument.h"
#include "Utils.h"

Enviroument::Enviroument(size_t sizeX, size_t sizeY) {
    mWidth = sizeX;
    mHeight = sizeY;
    mImage = GenImageColor(mWidth, mHeight, WHITE);

    auto prop = component_property_kit_lib::Properties();
    auto pgc = PlainGenComponent(prop, 0, typeid(PlainGenComponent).hash_code());
    pgc.init();

    auto map = pgc.generatePerlinChunk<component_property_kit_lib::SmootherSmoothLinear>(0.5, 0.5, 6, sizeX, sizeY);

    mTileMap.resize(map.size());

    for (size_t i = 0; i < map.size(); i++) {
        mTileMap[i].height = map[i] * 256.0f;
        mTileMap[i].plantFillness = mTileMap[i].height;
    }

    calculateSteepnessLevel();
}

void Enviroument::step(float wideness) {
    mOscillator += wideness;
    mETemperatureService.takeStep(mOscillator);
    mEWaterService.takeStep(mOscillator);
}

void Enviroument::generateImage() {
    Color* imageData = (Color*)mImage.data;

    //if (!mCheckList.any()) {
    //    handleUnfinishedCheckList();
    //    return;
    //}

    int shiftValue = 1;
    if (mEWaterService.getHighestLevel() > 128)
        shiftValue--;

    float steep = 0;

    for (size_t i = 0; i < mTileMap.size(); i++) {
        if (mTileMap[i].height > mETemperatureService.getIceCapLevel())
            imageData[i] = { (uint8_t)(mTileMap[i].height >> 1),(uint8_t)(mTileMap[i].height >> 1),(mTileMap[i].height), 255 };
        else if (mTileMap[i].height < mEWaterService.calculateSeaLevel())
            imageData[i] = { 0,0,(uint8_t)(mTileMap[i].height << shiftValue), 255 };
        else {
            steep = fit<float>(mTileMap[i].steepnessLevel, 0.f, 255.f);
            imageData[i] = { (uint8_t)(steep * 256.f),mTileMap[i].height,0, 255 };
        }

        //else
        //    imageData[i] = { mTileMap[i].height, mTileMap[i].height, mTileMap[i].height, 255 };
    }
}

float Enviroument::getOscillator() {
    return mOscillator;
}

Image& Enviroument::getImage() {
    return mImage;
}

bool Enviroument::chechIsUnfinishedCheckList() const noexcept {
    return !mEWaterService.isReady() && ~mCheckList.count();
}

void Enviroument::handleUnfinishedCheckList() const noexcept {
    printf("Operations was denied because not all in checklist was done: ");
    for (size_t i = 0; i < mCheckList.size(); i++) {
        printf("%d", (bool)mCheckList[i]);
    }

    printf("\n");
}

void Enviroument::calculateSteepnessLevel() {
    uint16_t avrg = 0;
    auto isInMap = [=](size_t index, int x, int y) {
        size_t ix = index % mWidth;
        size_t iy = index / mWidth;
        return (0 <= (ix + x)) && ((ix + x) < mWidth) && (0 <= (iy + y)) && ((iy + y) < mHeight);
    };

    uint8_t valids = 1;

    for (size_t i = 0; i < mTileMap.size(); i++) {
        avrg = mTileMap[i].height;
        valids = 1;
        for (int x = 0; x < 2; x++) {
            for (int y = 0; y < 2; y++) {
                if (isInMap(i, x, y)) {
                    valids++;
                    avrg += abs(mTileMap[i].height - mTileMap[i + x + y * mWidth].height) * 48;
                }
            }
        }

        mTileMap[i].steepnessLevel = slice<uint8_t>(avrg / valids, 0, 255);
    }
}
