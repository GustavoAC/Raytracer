#include "buffer.h"
#include <cmath>
#include <iostream>

Buffer::Buffer(int _width, int _height) : m_width(_width), m_height(_height) {
    buffer = std::shared_ptr<Pixel>(new Pixel[m_width * m_height]);
}

void Buffer::setPixel(const int &x, const int &y, const Pixel &p) {
    // std::cout << "Painting position (" << (int)point.getX() << ", " << (int)point.getY()
    //   << ") with color (" << (int)p.getR() << ", " << (int)p.getG() << ", " << (int)p.getB()
    //   << ")\n";

    if (x < m_width && x >= 0 && y < m_height && y >= 0) getPixel(x, y) = p;
    // buffer.get()[y * m_width + x] = p;
}

Pixel &Buffer::getPixel(const int &x, const int &y) { return buffer.get()[y * m_width + x]; }

void Buffer::writeToFile(const std::string &filename) {
    std::ofstream file(filename + std::string(".ppm"));

    file << "P3" << std::endl << m_width << " " << m_height << std::endl << "256" << std::endl;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            Pixel &p = getPixel(x, y);
            file << (int)p.getR() << " " << (int)p.getG() << " " << (int)p.getB() << " ";
        }
        file << std::endl;
    }

    file.close();
}