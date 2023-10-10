#pragma once

#include "colors.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <span>
namespace colors {
inline Hsb rand_hsb() {
  auto rand_one_to_zero = [] {
    return static_cast<float>(std::rand()) / RAND_MAX;
  };
  auto sqrt_rand = [=] { return std::sqrt(rand_one_to_zero()); };
  return Hsb(rand_one_to_zero() * 360.f, sqrt_rand() * 100, sqrt_rand() * 100);
}
inline Hsb rand_hue(float s,float b) {
  auto rand_one_to_zero = [] {
    return static_cast<float>(std::rand()) / RAND_MAX;
  };
  auto sqrt_rand = [=] { return std::sqrt(rand_one_to_zero()); };
  return Hsb(rand_one_to_zero() * 360.f, s, b);
}
}; // namespace colors