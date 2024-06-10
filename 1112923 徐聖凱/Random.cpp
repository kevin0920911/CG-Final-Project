#include "Random.h"

#include <random>

float randomRange(float min, float max) {
    static std::mt19937 rng(std::random_device{}());  // Mersenne Twister �üƤ���
    std::uniform_real_distribution<float> dist(min, max);  // �B�I���H�����G
    return dist(rng);  // �����H����
}