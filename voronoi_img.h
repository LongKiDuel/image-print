#pragma once
#include "image_printer.h"
#include "img.h"
#include "voronoi.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>

namespace image_printer {
inline Img generate_voronoi_img(int px, uint64_t grid) {
  Img img{px, px, 1};
  Voronoi voronoi{grid};
  write_image(img, [&voronoi](Write_info info) {
    auto d = voronoi.distance(info.coord_u, info.coord_v);
    // std::cout << d <<"\n";
    // d = std::max<float>(0,1-d);

    d = std::min<float>(1,d);
    // d = sqrt(d);
    d = pow(d,2);

    uint8_t color[1];
    color[0] = d*255.99;
    info(std::span{(char*)color,1});
  });
  return img;
}
} // namespace image_printer