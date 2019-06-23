#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "camera.h"
#include "config.h"
#include "integrator.h"
#include "light.h"
#include "material.h"
#include "scene.h"
#include "tinyxml.h"
#include "trianglemesh.h"
#include "vec3.h"

class XmlParser {
 private:
  std::string m_filename;
  std::shared_ptr<TiXmlDocument> m_doc;
  TiXmlElement *m_topElement = nullptr;
  std::vector<std::shared_ptr<Material>> materials;
  std::vector<std::shared_ptr<Light>> lights;
  std::shared_ptr<Camera> camera;

  XmlParser(std::string _filename) : m_filename(_filename) {
    m_doc = std::make_shared<TiXmlDocument>(TiXmlDocument(m_filename.c_str()));
  };

  std::shared_ptr<Background> parseColorBackground(
      TiXmlElement *backgroundNode);
  std::shared_ptr<Background> parseTextureBackground(
      TiXmlElement *backgroundNode);
  std::shared_ptr<Background> getBackground(TiXmlNode *parent);
  TiXmlNode *getChildWithName(TiXmlNode *parent, const std::string &name);
  TiXmlAttribute *getAttribute(TiXmlElement *parent, const std::string &name);
  std::shared_ptr<Primitive> getPrimitive(TiXmlElement *parent);
  std::shared_ptr<Primitive> getSphere(TiXmlElement *parent);
  std::shared_ptr<Primitive> getTriangleMesh(TiXmlElement *parent);
  std::shared_ptr<Primitive> getTriangleMeshFromFile(
      const std::string &filename, const std::shared_ptr<Material> &material);
  std::shared_ptr<Primitive> createBVHFromTriangleMesh(
      const TriangleMesh &mesh);
  std::shared_ptr<vec3> getVector(TiXmlNode *parent,
                                  const std::string &vecName);
  std::vector<int> getIntVecFromString(const std::string &string);
  std::vector<vec3> getVecVecFromString(const std::string &string);
  void addMaterial(TiXmlNode *parent);
  void addFlatMaterial(TiXmlNode *parent);
  void addBlinnMaterial(TiXmlNode *parent);
  void addLight(TiXmlNode *parent);
  void addAmbientLight(TiXmlNode *parent);
  void addPointLight(TiXmlNode *parent);
  void addSpotLight(TiXmlNode *parent);
  void addDirectionalLight(TiXmlNode *parent);
  Pixel parseColor(TiXmlNode *parent);

 public:
  static std::shared_ptr<XmlParser> instantiate(std::string _filename) {
    XmlParser p(_filename);
    if (!p.m_doc->LoadFile()) {
      std::cout << "Unable to load file...";
      return nullptr;
    }

    TiXmlNode *node = &(*p.m_doc);
    TiXmlNode *pChild;
    for (pChild = node->FirstChild(); pChild != 0 and p.m_topElement == nullptr;
         pChild = pChild->NextSibling()) {
      if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT) {
        TiXmlElement *a = pChild->ToElement();
        if (a->Value() == G_PROJECT_NAME) p.m_topElement = a;
      }
    }

    if (p.m_topElement == nullptr)
      return nullptr;
    else
      return std::make_shared<XmlParser>(p);
  }

  std::shared_ptr<Camera> getCamera();
  std::shared_ptr<Scene> getScene();
  std::shared_ptr<Integrator> getIntegrator();
};