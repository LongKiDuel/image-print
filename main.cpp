#include "colors.h"
#include "colors_convert.h"
#include "grid_check.h"
#include "image_printer.h"
#include "printer_extentions.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numbers>
#include <span>
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>
struct Img {
  int width;
  int height;
  int channel;
  char *data() {
    if (!buffer) {
      buffer = new char[width * height * channel];
    }
    return buffer;
  }
  char *buffer{};
};
int main() {
  const int len = 4096;
  Img i{len, len, 3};
  auto old_callback = [](const image_printer::Write_info &info) {
    const float center_x =
        image_printer::zero_one_to_neg_one_to_one(info.coord_u);
    const float center_y =
        image_printer::zero_one_to_neg_one_to_one(info.coord_v);
    float density =
        std::sqrt(std::pow(center_x, 2) + std::pow(center_y, 2)) * 3;
    if (density > 3) {
      density = 1;
    }
    const int grid = 128;
    const float uv_grid = 0.1;
    // density =( info.coord_u / uv_grid)  + ( info.coord_v / uv_grid);

    auto rad = atan2(center_y, center_x);
    uint8_t buffer[3]{0};
    float percent = (rad + std::numbers::pi) / std::numbers::pi / 2 * 3;
    float ratio = fmod(percent, 1);
    int index = percent;
    int last_i = (index + 2) % 3;

    buffer[index] = fmod(density, 1) * ratio * 255;
    buffer[last_i] = fmod(density, 1) * (1 - ratio) * 255;
    colors::Hsb hsb{percent / 3 * 360, 100, 100};
    auto rgb = colors::hsbToRgb(hsb);
    rgb.for_each([pointer = buffer](auto c) mutable { *(pointer++) = c; });

    info(std::span<char>{(char *)buffer, 3});
  };
  auto new_callback = [](const image_printer::Write_info &info) {
    image_printer::Write_info_ext ext{info};
    colors::Hsb hsb{0, 0, ext.center_circle_degree_percent * 100};
    auto rgb = colors::hsbToRgb(hsb);
    char buffer[3];
    rgb.for_each([pointer = buffer](auto c) mutable { *(pointer++) = c; });

    info(std::span<char>{(char *)buffer, 3});
  };
  const int line_len = 4096;
  Img line_img{line_len, line_len, 3};
  auto line_callback = [](const image_printer::Write_info &info) {
    colors::Hsb hsb{0, 0, (info.coord_u) * 100};
    auto rgb = colors::hsbToRgb(hsb);
    char buffer[3];
    rgb.for_each([pointer = buffer](auto c) mutable { *(pointer++) = c; });

    info(std::span<char>{(char *)buffer, 3});
  };
  auto line_gama_callback = [](const image_printer::Write_info &info) {
    const float gamma = 2.2f;
    // do encode to gamma
    // to decode do: pow(v,gamma)
    const int block_pix = 125;
    const int img_size = 4096;
    const float border_ratio = 0.2;
    float pos = image_printer::grid_border(info.coord_u, img_size / block_pix,border_ratio);
    float posy = image_printer::grid_border(info.coord_v, img_size / block_pix,border_ratio);
    pos = std::max(pos,posy); 
    colors::Hsb hsb{0, 0, pow<float>(pos, 1 / gamma) * 100};

    auto rgb = colors::hsbToRgb(hsb);
    char buffer[3];
    rgb.for_each([pointer = buffer](auto c) mutable { *(pointer++) = c; });

    info(std::span<char>{(char *)buffer, 3});
  };
  image_printer::write_image(i, new_callback);
  stbi_write_png("a.png", i.width, i.height, i.channel, i.data(),
                 i.width * i.channel);
  image_printer::write_image(line_img, line_callback);
  stbi_write_png("line.png", line_img.width, line_img.height, line_img.channel,
                 line_img.data(), line_img.width * line_img.channel);
  image_printer::write_image(line_img, line_gama_callback);
  stbi_write_png("line_gama.png", line_img.width, line_img.height,
                 line_img.channel, line_img.data(),
                 line_img.width * line_img.channel);

  return 0;
}
