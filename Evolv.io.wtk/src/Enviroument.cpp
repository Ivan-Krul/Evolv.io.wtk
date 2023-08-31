#include "Enviroument.h"
#include "Utils.h"

Enviroument::Enviroument(size_t sizeX, size_t sizeY) {
    mWidth = sizeX;
    mHeight = sizeY;
    mImage = GenImageColor(mWidth, mHeight, WHITE);
}

void Enviroument::step(float wideness) {
    mOscillator += wideness;
    //mETemperatureService.takeStep(mOscillator);
    //mEWaterService.takeStep(mOscillator);

    mECloudService.takeStep(wideness);
}

void Enviroument::generateImage() {
    Color* imageData = (Color*)mImage.data;

    //if (chechIsUnfinishedCheckList()) {
    //    handleUnfinishedCheckList();
    //    return;
    //}

    int shiftValue = 1;
    if (mEWaterService.getHighestLevel() > 128)
        shiftValue--;

    float steep = 0;

    int16_t val;
    uint8_t c;
    auto& cloud = mECloudService.getCloudness();

    for (size_t y = 0; y < mHeight; y++) {
        for (size_t x = 0; x < mWidth; x++) {
            c = cloud[x + y * mWidth];
            val = cloud[x + y * mWidth];
            if (val < 0) {
                imageData[x + y * mWidth] = { 0,0,255, 255 };
                //printf("und\n");
            }
            else if (val > 256) {
                imageData[x + y * mWidth] = { 255,0,0, 255 };
                //printf("ove\n");
            }
            else
                imageData[x + y * mWidth] = { c,c,c, 255 };
            //imageData[x + y * mWidth] = { c,c,c, 255 };
        }
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
