#pragma once

#include "pixel.h"

//! Class which gives pixel colors from a given (i,j) position
class Background {
   private:
    Pixel m_colors[4];
    int m_numColors = 0;
    
    std::string m_texturePath;
    int m_tex_horizontal_size = 1;
    int m_tex_vertical_size = 1;

    Pixel getColorFromTexture(float i, float j);

   public:
    //! Adds a color to the Background. Won't have any effect when the Background already has 4
    //! colors.
    void addColor(const Pixel& _newColor);

    //! Sets texture
    void setTexture(const std::string _texturePath, int repeat_horizontal_size = 1, int repeat_vertical_size = 1);

    //! Returns a Pixel color from the background given a horizontal and vertical offset
    Pixel getColor(float i, float j);

    const Pixel& getTopLeft() const;
    const Pixel& getTopRight() const;
    const Pixel& getBottomLeft() const;
    const Pixel& getBottomRight() const;
};