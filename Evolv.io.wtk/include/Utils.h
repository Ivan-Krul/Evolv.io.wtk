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
