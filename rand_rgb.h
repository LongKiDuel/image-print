#pragma once

#include "colors.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <span>
namespace colors {
inline Rgb_u8 rand_rgb() {
  auto rand_one_to_zero = [] {
    return static_cast<float>(std::rand()) / RAND_MAX;
  };
  auto sqrt_rand = [=] { return std::sqrt(rand_one_to_zero()); };
  auto f = [rand_one_to_zero]{
    return rand_one_to_zero() * 256;
  };
  return Rgb_u8(f() , f() , f() );
}
}; // namespace colors