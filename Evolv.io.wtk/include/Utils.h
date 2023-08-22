#pragma once

template<typename T>
inline T findZeroFromFit(T min, T max) { return min; }

template<typename T>
inline T scale(float x, T min, T max) {
    return (T)(x * (max - min)) + min;
}

template<typename T>
inline T findZeroFromScale(T min, T max) {
    return -min / (max - min);
}

template<typename T>
inline T fit(T x, T min, T max) {
    return (x - min) / (max - min);
}

template<typename T>
inline T slice(T x, T min, T max) noexcept {
    return std::min(std::max(x, min), max);
}

inline bool isInsideOfMap(int x, int y, size_t maxX, size_t maxY) {
    return (0 <= x) && (x < maxX) && (0 <= y) && (y < maxY);
}

inline bool isInsideOfMap(size_t startPoint,int x, int y, size_t maxX, size_t maxY) {
    size_t startX = startPoint % maxX;
    size_t startY = startPoint / maxX;
    return (0 <= x + startX) && (x + startX < maxX) && (0 <= y + startY) && (y+startY < maxY);
}

inline bool willOverflow(uint8_t val, uint8_t add) {
    return val > 255 - add;
}

inline void addExpectedOverflowAndUnderflow(uint8_t& val, int8_t add) {
    if (add > 0) {
        if (val > 255 - add)
            val = 255;
        else
            val += add;
    }
    else {
        if (val < add)
            val = 0;
        else
            val += add;
    }
}
