#include <iostream>
#include "pixel.h"
#include "ray.h"
#include "xmlParser.h"

#include <vector>
#include "sphere.h"

int main(int argc, char const* argv[]) {
  if (argc < 2) {
    std::cout << "Please insert the xml description path as an argument"
              << std::endl;
    return 1;
  }

  std::cout << "Creating Parser...\n";
  auto parser = XmlParser::instantiate(argv[1]);

  std::cout << "Reading Camera...\n";
  auto camera = parser->getCamera();
  std::cout << "Reading Scene...\n";
  auto scene = parser->getScene();
  std::cout << "Reading Integrator...\n";
  auto integrator = parser->getIntegrator();

  integrator->render(*scene);

  if (argc > 2)
    camera->getFilm()->writeToFile(argv[2]);
  else
    camera->getFilm()->writeToFile(argv[1]);

  return 0;
}
