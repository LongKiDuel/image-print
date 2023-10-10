#pragma once

#include <array>
#include <cstdint>
namespace colors {

class Rgb_u8 {
public:
    Rgb_u8(uint8_t r = {}, uint8_t g = {}, uint8_t b = {})
        : storage_{r, g, b} {}
    
    // Red channel accessor and mutator
    uint8_t r() const noexcept { return storage_[0]; }
    void r(uint8_t value) noexcept { storage_[0] = value; }
    
    // Green channel accessor and mutator
    uint8_t g() const noexcept { return storage_[1]; }
    void g(uint8_t value) noexcept { storage_[1] = value; }
    
    // Blue channel accessor and mutator
    uint8_t b() const noexcept { return storage_[2]; }
    void b(uint8_t value) noexcept { storage_[2] = value; }

    // For-each function templates to allow iteration over the RGB values
    template <typename T>
    void for_each(T&& callback) {
        for (auto &i : storage_) {
            callback(i);
        }
    }

    template <typename T>
    void for_each(T&& callback) const {
        for (auto &i : storage_) {
            callback(i);
        }
    }

private:
    std::array<uint8_t, 3> storage_;
};


class Hsb {
public:
    // Constructor with default values
    Hsb(float h = 0.0f, float s = 0.0f, float b = 0.0f)
        : h_(h), s_(s), b_(b) {}
    
    // Hue accessor and mutator
    float h() const noexcept { return h_; }
    void h(float value) noexcept { h_ = value; }
    
    // Saturation accessor and mutator
    float s() const noexcept { return s_; }
    void s(float value) noexcept { s_ = value; }
    
    // Brightness accessor and mutator
    float b() const noexcept { return b_; }
    void b(float value) noexcept { b_ = value; }

    // Ensure values are within valid ranges
    void validate() noexcept {
        if (h_ < 0.0f) h_ = 0.0f;
        if (h_ > 360.0f) h_ = 360.0f;
        if (s_ < 0.0f) s_ = 0.0f;
        if (s_ > 100.0f) s_ = 100.0f;
        if (b_ < 0.0f) b_ = 0.0f;
        if (b_ > 100.0f) b_ = 100.0f;
    }

private:
    float h_, s_, b_;
};
}; // namespace colors