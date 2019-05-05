#pragma once
#include "material.h"
#include "pixel.h"

class FlatMaterial : public Material {
 private:
  Pixel color;

 public:
  FlatMaterial(const std::string &_name, const Pixel &_color) : Material(_name), color(_color) {}
  Pixel getColor() const { return color; }
};