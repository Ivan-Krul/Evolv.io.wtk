#include "Enviroument.h"
#include "Utils.h"

Enviroument::Enviroument(size_t sizeX, size_t sizeY) {
    mWidth = sizeX;
    mHeight = sizeY;
    mImage = GenImageColor(mWidth, mHeight, WHITE);
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

    for (size_t i = 0; i < mETerrainService.getSize(); i++) {
        if (mETerrainService.getHeight(i) > mETemperatureService.getIceCapLevel())
            imageData[i] = { (uint8_t)(mETerrainService.getHeight(i) >> 1),(uint8_t)(mETerrainService.getHeight(i) >> 1),mETerrainService.getHeight(i), 255 };
        else if (mETerrainService.getHeight(i) < mEWaterService.calculateSeaLevel())
            imageData[i] = { 0,0,(uint8_t)(mETerrainService.getHeight(i) << shiftValue), 255 };
        else {
            imageData[i] = { (uint8_t)(mETerrainService.getSteepness(i) * 256.f),mETerrainService.getHeight(i),0, 255 };
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
    return !mEWaterService.isReady() && ~mCheckList.count() && !mETemperatureService.isReady() && mETerrainService.isReady();
}

void Enviroument::handleUnfinishedCheckList() const noexcept {
    printf("Operations was denied because not all in checklist was done: ");
    for (size_t i = 0; i < mCheckList.size(); i++) {
        printf("%d", (bool)mCheckList[i]);
    }

    printf("\n");
}
