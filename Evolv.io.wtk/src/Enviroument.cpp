#include "Enviroument.h"

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

void Enviroument::setSeaLevel(uint16_t seaLevel) noexcept {
    mSeaLevel = seaLevel;
    mCheckList[0] = true;
}

void Enviroument::setTides(uint16_t tideAmplitude, uint16_t tideFrequency) noexcept {
    mTideAmplitude = tideAmplitude;
    mTideFrequency = tideFrequency;
    mCheckList[1] = true;
}

void Enviroument::setSeasonFrequency(float seasonFrequency) noexcept {
    mSeasonFrequency = seasonFrequency;
    mCheckList[2] = true;
}

void Enviroument::setTempratureRange(int16_t tempMinInLowest, uint16_t heightTempDifference, int16_t tempMaxInLowest) noexcept {
    mTempMinInLowest = tempMinInLowest;
    mTempMaxInLowest = tempMaxInLowest;
    mHeightDifference = heightTempDifference;
    calculateIceCapLevel();
    mCheckList[3] = true;
}

void Enviroument::step(float wideness) {
    calculateIceCapLevel();
    mOscillator += wideness;
    mSeasonRatio = fit<float>(sin(mOscillator * PI * mSeasonFrequency), -1, 1);
    mTideRatio = sin(mOscillator * PI * mTideFrequency);
}

void Enviroument::generateImage() {
    Color* imageData = (Color*)mImage.data;

    //if (!mCheckList.any()) {
    //    handleUnfinishedCheckList();
    //    return;
    //}

    int shiftValue = 1;
    if (mSeaLevel + mTideAmplitude > 128)
        shiftValue--;

    float steep = 0;

    for (size_t i = 0; i < mTileMap.size(); i++) {
        if (mTileMap[i].height > mIceCapLevel)
            imageData[i] = { (uint8_t)(mTileMap[i].height >> 1),(uint8_t)(mTileMap[i].height >> 1),(mTileMap[i].height), 255 };
        else if (mTileMap[i].height < (mSeaLevel + mTideRatio * (float)mTideAmplitude))
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

void Enviroument::handleUnfinishedCheckList() noexcept {
    printf("Operations was denied because not all in checklist was done: ");
    for (size_t i = 0; i < mCheckList.size(); i++) {
        printf("%d", (bool)mCheckList[i]);
    }

    printf("\n");
}

void Enviroument::calculateIceCapLevel() {
    float currentLowest = scale<float>(mSeasonRatio, mTempMinInLowest, mTempMaxInLowest);
    float height = findZeroFromScale<float>(currentLowest, currentLowest - mHeightDifference) * 256.f;
    //printf("%f\n", height);
    mIceCapLevel = slice<int16_t>(height, 0, 256);
    
    //printf("Lowest point: %d°C\nHighest point: %d°C\n", getCurrentTemprature(0), getCurrentTemprature(255));
}

int16_t Enviroument::getCurrentTemprature(uint8_t height) {
    float heightDiff = (float)height / 256.f;
    uint16_t currentLowest = scale<float>(mSeasonRatio, mTempMinInLowest, mTempMaxInLowest);
    return scale<float>(heightDiff, currentLowest, currentLowest - mHeightDifference);
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
