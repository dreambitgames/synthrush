#include "random.h"

#include <chrono>
#include <random>

static std::mt19937 rng;

void synthrush::util::StartRandomEngine() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    rng = std::mt19937(seed);
}

float synthrush::util::Random(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(rng);
}
