#pragma once

namespace image_printer {
class Write_info;

class Write_info_ext {
public:
  Write_info_ext(const Write_info &basic_info);

public:
  float center_x;
  float center_y;
  float center_distance;
  float center_radian;
  float center_circle_degree_percent;
};
} // namespace image_printer