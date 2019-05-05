#pragma once
#include <string>

class Material {
  std::string name;

 public:
  Material(const std::string& _name) : name(_name) {}

  const std::string& getName() const { return name; }
  virtual ~Material() {}
};
