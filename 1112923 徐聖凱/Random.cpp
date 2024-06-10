#include "Random.h"

#include <random>

float randomRange(float min, float max) {
    static std::mt19937 rng(std::random_device{}());  // Mersenne Twister 亂數引擎
    std::uniform_real_distribution<float> dist(min, max);  // 浮點數隨機分佈
    return dist(rng);  // 產生隨機數
}