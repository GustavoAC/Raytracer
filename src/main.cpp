#include <iostream>
#include "pixel.h"
#include "ray.h"
#include "xmlParser.h"

#include "sphere.h"
#include <vector>

int main(int argc, char const* argv[]) {
    if (argc < 2) {
        std::cout << "Please insert the xml description path as an argument" << std::endl;
        return 1;
    }

    std::cout << "Creating Parser...\n";
    auto parser = XmlParser::instantiate(argv[1]);

    std::cout << "Reading Buffer...\n";
    auto camera = parser->getCamera();
    std::cout << "Reading Background...\n";
    auto background = parser->getBackground();
    std::cout << "Reading Scene...\n";
    auto primitives = (std::move(parser->getScene()));

    std::cout << "Painting buffer...\n";
    Buffer buffer(camera->getWidth(), camera->getHeight());
    auto w = buffer.getWidth();
    auto h = buffer.getHeight();

    for (int i = 0; i < buffer.getWidth(); i++) {
        for (int j = 0; j < buffer.getHeight(); j++) {
            Ray r1 = camera->generateRay(float(i) / float(w), float(j) / float(h));
            // std::cout << r1 << std::endl;

            // Color any hit points with red
            Pixel p = background->getColor(float(i) / w, float(j) / h);
            for (auto &prim: primitives) {
                SurfaceInteraction si;
                bool hit = prim->intersect(r1, &si);
                if (hit) p = COLOR_RED;
            }

            buffer.setPixel(i, j, p);
        }
    }

    if (argc > 2)
        buffer.writeToFile(argv[2]);
    else
        buffer.writeToFile(argv[1]);

    return 0;
}
