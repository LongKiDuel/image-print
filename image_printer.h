#pragma once

#include <cstdint>
#include <cstring>
#include <span>
namespace image_printer {
template <typename T>
concept Image = requires(T img) {
  img.width;
  img.height;
  img.channel;
  img.data();
};

class Write_info {
public:
  uint32_t width{};
  uint32_t height{};
  uint32_t channel{};

  uint32_t write_x{};
  uint32_t write_y{};
  float coord_u{};
  float coord_v{};
  void operator()(std::span<char> data) const {
    memcpy(dest, data.data(), data.size());
  }
  void set_write_point(char *dest) { this->dest = dest; }

private:
  char *dest{};
};
template <Image T, typename U> void write_image(T &image, U callback) {
  auto base = image.data();
  Write_info info{};
  info.height = image.height;
  info.width = image.width;
  info.channel = image.channel;
  const float coord_offset_u = (1 / static_cast<float>(info.width)) / 2;
  const float coord_offset_v = (1 / static_cast<float>(info.height)) / 2;

  for (uint32_t i = 0; i < info.height; i++) {
    info.write_y = i;
    info.coord_v = 1 - i / static_cast<float>(info.height) - coord_offset_v;
    for (uint32_t j = 0; j < info.width; j++) {
      info.write_x = j;
      info.coord_u = j / static_cast<float>(info.width) + coord_offset_u;
      info.set_write_point(base);
      base+=info.channel;

      callback(info);
    }
  }
}
inline float zero_one_to_neg_one_to_one(float zero_to_one) {
  return zero_to_one * 2 - 1;
}
} // namespace iamge_printer