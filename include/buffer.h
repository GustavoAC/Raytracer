#pragma once

#include <memory>
#include <fstream>
#include "pixel.h"

//! Class representing the buffer object in which the objects are drawn

class Buffer {
  private:
  	int m_width;
  	int m_height;
  	std::shared_ptr<Pixel> buffer;
    
  public:
  	Buffer(int _width, int _height);

    //! Gets the pixel on the given point
    Pixel& getPixel(const int &x, const int &y);
    //! Sets the given point with the given pixel
    void setPixel(const int &x, const int &y, const Pixel &p);
    //! Returns buffer width
    inline int getWidth() const { return m_width; }
    //! Returns buffer height
    inline int getHeight() const { return m_height; }
    //! Writes buffer to file, applying convolution antialiasing is optional
    void writeToFile(const std::string &filename);
};