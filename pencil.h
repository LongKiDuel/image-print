#pragma once

#include "image_printer.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
namespace image_printer {
template <Image T, typename U, size_t N>
inline void paint_with_pencil(T &image, uint32_t pos_x, uint32_t pos_y,
                              float radius, float pressure,
                              std::span<U, N> color) {
  if (pressure < 0.0f || pressure > 1.0f || radius < 1) {
    return;
  }
  const auto height = image.height;
  const auto width = image.width;
  const auto channel = image.channel;
  const auto image_buffer = image.data();

  int radius_int = static_cast<int>(radius);

  for (int y = -radius_int; y <= radius_int; y++) {
    for (int x = -radius_int; x <= radius_int; x++) {
      // Check if (x, y) is inside the circle defined by radius
      if (x * x + y * y <= radius * radius) {
        int currentX = pos_x + x;
        int currentY = pos_y + y;

        // Check if the current pixel is inside the image boundaries
        if (currentX >= 0 && currentX < width && currentY >= 0 &&
            currentY < height) {
          int index = (currentY * width + currentX) * channel;

          for (int i = 0; i < channel; i++) { // For R, G, B values
            image_buffer[index + i] = color[i] * pressure;
          }

          // Modify alpha (or intensity) based on pressure.
        //   image_buffer[index + channel - 1] = static_cast<char>(pressure * 255);
        }
      }
    }
  }
}
}; // namespace image_printer