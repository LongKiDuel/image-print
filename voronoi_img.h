#pragma once
#include "colors.h"
#include "colors_convert.h"
#include "image_printer.h"
#include "img.h"
#include "rand_hsb.h"
#include "rand_rgb.h"
#include "voronoi.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <span>
#include <unordered_map>

namespace image_printer {
inline Img generate_voronoi_img(int px, uint64_t grid) {
  Img img{px, px, 1};
  Voronoi voronoi{grid};
  write_image(img, [&voronoi](Write_info info) {
    auto result = voronoi.find_nearest(info.coord_u, info.coord_v);
    auto d = result.distance;
    // std::cout << d <<"\n";
    // d = std::max<float>(0,1-d);

    d = std::min<float>(1, d);
    // show center dot.

    // if (d < 0.05) {
    //   d = 1 - d;
    // }
    // d = sqrt(d);
    d = pow(d, 2);

    uint8_t color[1];
    color[0] = d * 255.99;
    info(std::span{(char *)color, 1});
  });
  return img;
}
inline Img generate_voronoi_img_with_color(int px, uint64_t grid) {
  Img img{px, px, 3};
  Voronoi voronoi{grid};
  std::unordered_map<uint64_t, colors::Rgb_u8> id_to_color;
  write_image(img, [&voronoi,&id_to_color](Write_info info) {
    auto result = voronoi.find_nearest(info.coord_u, info.coord_v);
    auto d = result.distance;
    auto id = result.id;
    if(id_to_color.find(id)==id_to_color.end()){
        id_to_color[id] = colors::rand_rgb();
    }
    
    
    auto rgb = id_to_color[id];
    auto span = rgb.data_span();
    info(std::span<char>{(char*)span.data(),span.size()});
  });
  return img;
}
} // namespace image_printer