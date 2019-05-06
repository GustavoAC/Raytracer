#include "background.h"
#include <cmath>
#include <iostream>

void Background::addColor(const Pixel& _newColor) {
  if (m_numColors < 4) m_colors[m_numColors++] = _newColor;
}

void Background::setTexture(const std::string _texturePath,
                            int repeat_horizontal_size,
                            int repeat_vertical_size) {
  m_texturePath = _texturePath;
  m_tex_horizontal_size = repeat_horizontal_size;
  m_tex_vertical_size = repeat_vertical_size;
}

Pixel Background::getColor(float i, float j) {
  if (m_texturePath != "") getColorFromTexture(i, j);

  const Pixel& topLeft = getTopLeft();
  const Pixel& topRight = getTopRight();
  const Pixel& bottomLeft = getBottomLeft();
  const Pixel& bottomRight = getBottomRight();

  Pixel currentTop(topLeft.getR() + ((topRight.getR() - topLeft.getR()) * i),
                   topLeft.getG() + ((topRight.getG() - topLeft.getG()) * i),
                   topLeft.getB() + ((topRight.getB() - topLeft.getB()) * i));

  Pixel currentBottom(
      bottomLeft.getR() + ((bottomRight.getR() - bottomLeft.getR()) * i),
      bottomLeft.getG() + ((bottomRight.getG() - bottomLeft.getG()) * i),
      bottomLeft.getB() + ((bottomRight.getB() - bottomLeft.getB()) * i));

  Pixel current(
      currentTop.getR() + ((currentBottom.getR() - currentTop.getR()) * j),
      currentTop.getG() + ((currentBottom.getG() - currentTop.getG()) * j),
      currentTop.getB() + ((currentBottom.getB() - currentTop.getB()) * j));

  return current;
}

Pixel Background::getColorFromTexture(float i, float j) {
  // Initial offsets
  float horizontalOffset = i * m_tex_horizontal_size;
  float verticallOffset = j * m_tex_vertical_size;

  // Remove integer part
  horizontalOffset = horizontalOffset - int(horizontalOffset);
  verticallOffset = verticallOffset - int(verticallOffset);

  // magicimagething
  // int imageX = horizontalOffset * imagewidth;
  // int imageY = verticallOffset * imageheight;

  // return Pixelat(imageX, imageY);

  return Pixel();
}

// 0 0
// 0 0

// 0 0
// 1 1

// 0 1
// 2 2

// 0 1
// 3 2

const Pixel& Background::getTopLeft() const { return m_colors[0]; }

const Pixel& Background::getTopRight() const {
  if (m_numColors < 3)
    return m_colors[0];
  else
    return m_colors[1];
}

const Pixel& Background::getBottomLeft() const {
  return m_colors[m_numColors - 1];
}

const Pixel& Background::getBottomRight() const {
  int color_index = m_numColors - 1;
  if (m_numColors == 4) color_index = 2;
  return m_colors[color_index];
}