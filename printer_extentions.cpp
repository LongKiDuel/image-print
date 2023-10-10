#include "printer_extentions.h"
#include "image_printer.h"
#include <cmath>
namespace image_printer {

Write_info_ext::Write_info_ext(const Write_info &basic_info) {
  const float center_x =
      image_printer::zero_one_to_neg_one_to_one(basic_info.coord_u);
  const float center_y =
      image_printer::zero_one_to_neg_one_to_one(basic_info.coord_v);
  float distance = std::sqrt(std::pow(center_x, 2) + std::pow(center_y, 2));

  auto rad = atan2(center_y, center_x);

  float percent = (rad) / std::numbers::pi / 2;


  this->center_x = center_x;
  this->center_y = center_y;

  this->center_distance = distance;
  this->center_radian = rad;
  this->center_circle_degree_percent = percent;
};
} // namespace image_printer