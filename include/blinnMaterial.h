#pragma once
#include "material.h"
#include "vec3.h"

class BlinnMaterial : public Material {
 private:
  vec3 kd, ka, ks;
  float glossiness;

 public:
  BlinnMaterial(const std::string &_name, const vec3 &_kd, const vec3 &_ka,
                const vec3 &_ks, const float &_glossiness)
      : Material(_name), kd(_kd), ka(_ka), ks(_ks), glossiness(_glossiness) {}

  vec3 getKd() const { return kd; }
  vec3 getKa() const { return ka; }
  vec3 getKs() const { return ks; }
  float getGlossiness() const { return glossiness; }
};
