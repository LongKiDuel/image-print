#pragma once

namespace colors {

class Hsb;
class Rgb_u8;

// Convert RGB to HSB
Hsb rgbToHsb(const Rgb_u8 &rgb);

// Convert HSB to RGB
Rgb_u8 hsbToRgb(const Hsb &hsb);

} // namespace colors
