#include <iostream>
#include "pixel.h"
#include "xmlParser.h"

int main(int argc, char const* argv[]) {
    std::cout << "Creating Parser...\n";
    auto parser = XmlParser::instantiate("example.xml");

    std::cout << "Reading Buffer...\n";
    auto buffer = parser->getBuffer();
    std::cout << "Reading Background...\n";
    auto background = parser->getBackground();

    std::cout << "Painting buffer...\n";
    
    for (int i = 0 ; i < buffer->getWidth(); i++) {
        for (int j = 0; j < buffer->getHeight(); j++) {
            buffer->setPixel(i, j,
                             background->getColor(float(i) / buffer->getWidth(),
                                                  float(j) / buffer->getHeight()));
        }
    }

    buffer->writeToFile("background_test");

    return 0;
}
