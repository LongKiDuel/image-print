#include "colors.h"
#include "colors_convert.h"
#include "grid_check.h"
#include "grid_fill.h"
#include "image_printer.h"
#include "img.h"
#include "pencil.h"
#include "printer_extentions.h"
#include "rand_hsb.h"
#include "rand_rgb.h"
#include "voronoi_img.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <numbers>
#include <span>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image_write.h>
auto write_image = [](auto name, auto img) {
  stbi_write_png(name, img.width, img.height, img.channel, img.data(),
                 img.width * img.channel);
};
Img generateGridImg() {
  const int block_pix = 125;
  const int img_size_x = 4096;
  const int img_size_y = 2048;
  const float border_ratio = 0.125;
  Img image{img_size_x, img_size_y, 1};
  auto line_gama_callback = [&](const image_printer::Write_info &info) {
    float pos = image_printer::grid_border(
        info.coord_u , img_size_x / block_pix, border_ratio);
    float posy = image_printer::grid_border(
        info.coord_v , img_size_y / block_pix, border_ratio);
    pos = std::max(pos, posy);
    const float gamma = 2.2f;
    // do encode to gamma
    // to decode do: pow(v,gamma)
    pos = pow(pos, 1 / gamma);

    uint8_t buffer[1];
    buffer[0] = pos * 255.99;

    info(std::span<char>{(char *)buffer, 1});
  };
  image_printer::write_image(image, line_gama_callback);
  return image;
}
void extern_main() {
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
    const float border_ratio = 0.1;
    float pos = image_printer::grid_border(info.coord_u, img_size / block_pix,
                                           border_ratio);
    float posy = image_printer::grid_border(info.coord_v, img_size / block_pix,
                                            border_ratio);
    pos = std::max(pos, posy);
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
  {
    const auto size = 4096 / 125;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        auto color = i > size / 2 ? colors::rand_rgb()
                                  : colors::hsbToRgb(colors::rand_hsb());
        image_printer::grid_index_paint(line_img, size, size, j, i,
                                        color.data_span());
      }
    }
  }
  auto white = std::array<uint8_t, 3>{255, 255, 255};
  image_printer::grid_index_paint(line_img, 4096 / 125, 4096 / 125, 4, 4,
                                  std::span{white});
  for (int i = 0; i < 4096; i++) {
    image_printer::paint_with_pencil(line_img, i, i, 5, 0.7, std::span{white});
  }
  for (float i = 0; i < std::numbers::pi * 2; i += 0.01) {
    const float center = 4096 / 2;
    const float len = 500;
    const float thickness = 20;
    const float x = len * cos(i) + center;
    const float y = len * sin(i) + center;
    colors::Hsb hsb{i / (float)std::numbers::pi / 2 * 360, 100, 100};
    image_printer::paint_with_pencil(line_img, x, y, thickness, 0.9,
                                     colors::hsbToRgb(hsb).data_span());
  }
  stbi_write_png("line_gama.png", line_img.width, line_img.height,
                 line_img.channel, line_img.data(),
                 line_img.width * line_img.channel);
}
int main() {

  auto tm = time(nullptr);
  std::string t_str = ctime(&tm);
  t_str.pop_back();
  auto t = t_str + "voronoi.png";
  write_image(t.c_str(), image_printer::generate_voronoi_img(1024, 16));
  write_image((t_str + "grid.png").c_str(), generateGridImg());
  return 0;
}
