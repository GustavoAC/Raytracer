#pragma once
#include <string>

using color_t = unsigned char;

//! Represents a colored pixel on the canvas, smallest point of an image
class Pixel {
   private:
    color_t m_colors[3];

   public:
    Pixel(color_t _r = 0, color_t _g = 0, color_t _b = 0) {
        m_colors[0] = _r;
        m_colors[1] = _g;
        m_colors[2] = _b;
    }

    inline void setR(const color_t &r) { m_colors[0] = r; }

    inline void setG(const color_t &g) { m_colors[1] = g; }

    inline void setB(const color_t &b) { m_colors[2] = b; }

    inline color_t getR() const { return m_colors[0]; }

    inline color_t getG() const { return m_colors[1]; }

    inline color_t getB() const { return m_colors[2]; }

    inline bool operator==(const Pixel& rhs) {
        return (getR() == rhs.getR() && getG() == rhs.getG() && getB() == rhs.getB());
    }

    inline bool operator!=(const Pixel& rhs) { return !(*this == rhs); }
};

static const Pixel COLOR_BLACK = Pixel(0,0,0);
static const Pixel COLOR_RED = Pixel(255,0,0);
static const Pixel COLOR_GREEN = Pixel(0,255,0);
static const Pixel COLOR_BLUE = Pixel(0,0,255);
static const Pixel COLOR_YELLOW = Pixel(255,255,0); 
static const Pixel COLOR_CYAN = Pixel(0,255,255);
static const Pixel COLOR_MAGENTA = Pixel(255,0,255);
static const Pixel COLOR_ORANGE = Pixel(255,165,0);
static const Pixel COLOR_PURPLE = Pixel(190,0,255);
static const Pixel COLOR_WHITE = Pixel(255,255,255);