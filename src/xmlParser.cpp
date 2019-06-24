#include "xmlParser.h"
#include <sstream>
#include <unordered_map>
#include "ambientLight.h"
#include "blinnMaterial.h"
#include "blinnPhongIntegrator.h"
#include "bvhaccel.h"
#include "cyTriMesh.h"
#include "depthMapIntegrator.h"
#include "directionalLight.h"
#include "flatIntegrator.h"
#include "flatMaterial.h"
#include "listPrimitive.h"
#include "normalMapIntegrator.h"
#include "orthographic_camera.h"
#include "perspective_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "trianglemesh.h"

std::shared_ptr<Camera> XmlParser::getCamera() {
  if (m_topElement == nullptr) return nullptr;
  if (camera != nullptr) return camera;

  TiXmlNode *cameraNode = getChildWithName(m_topElement, "camera");
  if (cameraNode == nullptr) return nullptr;

  TiXmlAttribute *cameraType = getAttribute(cameraNode->ToElement(), "type");
  if (cameraType == nullptr) return nullptr;

  bool isOrthographic = (std::string(cameraType->Value()) == "orthographic");
  auto position = getVector(cameraNode, "position");
  auto target = getVector(cameraNode, "target");
  auto up = getVector(cameraNode, "up");
  auto width = getChildWithName(cameraNode, "width");
  auto height = getChildWithName(cameraNode, "height");
  if (position == nullptr || target == nullptr || up == nullptr ||
      width == nullptr || height == nullptr)
    return nullptr;

  auto widthAttr = getAttribute(width->ToElement(), "value");
  auto heightAttr = getAttribute(height->ToElement(), "value");
  if (widthAttr == nullptr || heightAttr == nullptr) return nullptr;

  int widthVal = -1;
  int heightVal = -1;
  widthAttr->QueryIntValue(&widthVal);
  heightAttr->QueryIntValue(&heightVal);
  if (widthVal < 0 || heightVal < 0) return nullptr;

  if (isOrthographic) {
    auto vpdimNode = getChildWithName(cameraNode, "vpdim");
    if (vpdimNode == nullptr) return nullptr;
    auto vpdim = vpdimNode->ToElement();

    auto l = getAttribute(vpdim, "l");
    auto r = getAttribute(vpdim, "r");
    auto b = getAttribute(vpdim, "b");
    auto t = getAttribute(vpdim, "t");
    if (l == nullptr || r == nullptr || b == nullptr || t == nullptr)
      return nullptr;

    double lVal, rVal, bVal, tVal;
    l->QueryDoubleValue(&lVal);
    r->QueryDoubleValue(&rVal);
    b->QueryDoubleValue(&bVal);
    t->QueryDoubleValue(&tVal);

    camera = std::shared_ptr<Camera>(new OrthographicCamera(
        widthVal, heightVal, *position, *target, *up, lVal, rVal, bVal, tVal));
  } else {
    auto fovyNode = getChildWithName(cameraNode, "fovy");
    auto fdistanceNode = getChildWithName(cameraNode, "fdistance");
    if (fovyNode == nullptr) return nullptr;

    auto fovyAttr = getAttribute(fovyNode->ToElement(), "value");
    auto fdistanceAttr = (fdistanceNode != nullptr)
                             ? getAttribute(fdistanceNode->ToElement(), "value")
                             : nullptr;
    if (fovyAttr == nullptr) return nullptr;

    double fovy, aspect = (double)widthVal / heightVal, fdistance = 1.0;
    fovyAttr->QueryDoubleValue(&fovy);
    if (fdistanceAttr != nullptr) fdistanceAttr->QueryDoubleValue(&fdistance);

    camera = std::shared_ptr<Camera>(new PerspectiveCamera(
        widthVal, heightVal, *position, *target, *up, fovy, aspect, fdistance));
  }

  return camera;
}

std::shared_ptr<Scene> XmlParser::getScene() {
  if (m_topElement == nullptr) return nullptr;

  TiXmlNode *sceneNode = getChildWithName(m_topElement, "scene");
  if (sceneNode == nullptr) return nullptr;

  auto background = getBackground(sceneNode);

  for (auto child = sceneNode->FirstChild(); child != nullptr;
       child = child->NextSibling()) {
    if (child->Type() == TiXmlNode::TINYXML_ELEMENT &&
        std::string(child->Value()) == "material")
      addMaterial(child);
    if (child->Type() == TiXmlNode::TINYXML_ELEMENT &&
        std::string(child->Value()) == "light")
      addLight(child);
  }

  std::vector<std::shared_ptr<Primitive>> primitives;

  for (TiXmlNode *pChild = sceneNode->FirstChild(); pChild != 0;
       pChild = pChild->NextSibling()) {
    if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT &&
        std::string(pChild->Value()) == "object") {
      auto primitive = getPrimitive(pChild->ToElement());
      if (primitive != nullptr) primitives.push_back(primitive);
    }
  }

  ListPrimitive *ap = new ListPrimitive(primitives);
  return std::make_shared<Scene>(
      Scene(std::shared_ptr<Primitive>(ap), lights, background));
  return nullptr;
}

std::shared_ptr<Integrator> XmlParser::getIntegrator() {
  if (m_topElement == nullptr) return nullptr;

  TiXmlNode *runningNode = getChildWithName(m_topElement, "running");
  if (runningNode == nullptr) return nullptr;

  TiXmlNode *integratorNode = getChildWithName(runningNode, "integrator");
  if (integratorNode == nullptr) return nullptr;

  auto integratorElement = integratorNode->ToElement();
  auto integratorTypeAttr = getAttribute(integratorElement, "type");
  if (integratorTypeAttr == nullptr) return nullptr;
  auto integratorType = std::string(integratorTypeAttr->Value());

  if (integratorType == "flat") {
    return std::shared_ptr<Integrator>(
        new FlatIntegrator(getCamera(), std::make_shared<Sampler>(Sampler())));
  } else if (integratorType == "depthMap") {
    auto near = parseColor(getChildWithName(integratorNode, "near"));
    auto far = parseColor(getChildWithName(integratorNode, "far"));

    return std::shared_ptr<Integrator>(new DepthMapIntegrator(
        getCamera(), std::make_shared<Sampler>(Sampler()), near, far));
  } else if (integratorType == "normalMap") {
    return std::shared_ptr<Integrator>(new NormalMapIntegrator(
        getCamera(), std::make_shared<Sampler>(Sampler())));
  } else if (integratorType == "blinn phong") {
    return std::shared_ptr<Integrator>(new BlinnPhongIntegrator(
        getCamera(), std::make_shared<Sampler>(Sampler())));
  }

  return nullptr;
}

std::shared_ptr<Background> XmlParser::getBackground(TiXmlNode *parent) {
  if (parent == nullptr) return nullptr;
  TiXmlNode *backgroundNode = getChildWithName(parent, "background");
  if (backgroundNode == nullptr) return nullptr;

  TiXmlElement *backgroundElement = backgroundNode->ToElement();
  TiXmlAttribute *attr = backgroundElement->FirstAttribute();
  while (attr) {
    if (std::string(attr->Name()) == "type") {
      if (std::string(attr->Value()) == "color")
        return parseColorBackground(backgroundElement);
      if (std::string(attr->Value()) == "texture")
        return parseTextureBackground(backgroundElement);
    }

    attr = attr->Next();
  }

  return nullptr;
}

void XmlParser::addMaterial(TiXmlNode *parent) {
  auto typeAttr = getAttribute(parent->ToElement(), "type");
  if (typeAttr == nullptr) return;
  auto type = std::string(typeAttr->Value());

  if (type == "flat") addFlatMaterial(parent);
  if (type == "blinn") addBlinnMaterial(parent);
}

void XmlParser::addFlatMaterial(TiXmlNode *parent) {
  auto nameAttr = getAttribute(parent->ToElement(), "name");
  if (nameAttr == nullptr) return;
  auto name = std::string(nameAttr->Value());

  auto colorNode = getChildWithName(parent, "diffuse")->ToElement();
  Pixel p;
  int ival;
  for (TiXmlAttribute *attr = colorNode->FirstAttribute(); attr != nullptr;
       attr = attr->Next()) {
    std::string attrName(attr->Name());
    if (attrName == "r" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setR(ival);
    if (attrName == "g" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setG(ival);
    if (attrName == "b" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setB(ival);
  }

  materials.push_back(std::shared_ptr<Material>(new FlatMaterial(name, p)));
}

void XmlParser::addBlinnMaterial(TiXmlNode *parent) {
  auto nameAttr = getAttribute(parent->ToElement(), "name");
  if (nameAttr == nullptr) return;
  auto name = std::string(nameAttr->Value());

  auto diffuseVec = getVector(parent, "diffuse");
  auto ambientVec = getVector(parent, "ambient");
  auto specularVec = getVector(parent, "specular");
  auto glossinessNode = getChildWithName(parent, "glossiness");
  if (diffuseVec == nullptr || ambientVec == nullptr ||
      specularVec == nullptr || glossinessNode == nullptr) {
    std::cout << "Error while parsing blinn material, invalid settings\n";
    return;
  }

  double glossiness;
  auto glossinessElement = getChildWithName(parent, "glossiness")->ToElement();
  auto glossinessAttr = getAttribute(glossinessElement, "value");
  glossinessAttr->QueryDoubleValue(&glossiness);

  materials.push_back(std::shared_ptr<Material>(new BlinnMaterial(
      name, *diffuseVec, *ambientVec, *specularVec, glossiness)));
}

void XmlParser::addLight(TiXmlNode *parent) {
  auto typeAttr = getAttribute(parent->ToElement(), "type");
  if (typeAttr == nullptr) return;
  auto type = std::string(typeAttr->Value());

  if (type == "ambient") addAmbientLight(parent);
  if (type == "point") addPointLight(parent);
  if (type == "spot") addSpotLight(parent);
  if (type == "directional") addDirectionalLight(parent);
}

void XmlParser::addAmbientLight(TiXmlNode *parent) {
  auto intensityVec = getVector(parent, "intensity");
  if (intensityVec == nullptr) {
    std::cout << "Error reading ambient light\n";
    return;
  }

  lights.push_back(std::shared_ptr<Light>(new AmbientLight(*intensityVec)));
}

void XmlParser::addPointLight(TiXmlNode *parent) {
  auto intensityVec = getVector(parent, "intensity");
  auto positionVec = getVector(parent, "position");
  if (intensityVec == nullptr || positionVec == nullptr) {
    std::cout << "Error reading point light\n";
    return;
  }

  lights.push_back(
      std::shared_ptr<Light>(new PointLight(*intensityVec, *positionVec)));
}

void XmlParser::addSpotLight(TiXmlNode *parent) {
  auto intensityVec = getVector(parent, "intensity");
  auto positionVec = getVector(parent, "position");
  auto pointAtVec = getVector(parent, "point_at");
  auto cutoffNode = getChildWithName(parent, "cutoff");
  auto falloffNode = getChildWithName(parent, "falloff");
  if (cutoffNode == nullptr || falloffNode == nullptr) {
    std::cout << "Error reading spotlight\n";
    return;
  }

  auto cutoffAttr = getAttribute(cutoffNode->ToElement(), "value");
  auto falloffAttr = getAttribute(falloffNode->ToElement(), "value");
  if (intensityVec == nullptr || positionVec == nullptr ||
      pointAtVec == nullptr || cutoffAttr == nullptr ||
      falloffAttr == nullptr) {
    std::cout << "Error reading spotlight\n";
    return;
  }

  int cutoff, falloff;
  cutoffAttr->QueryIntValue(&cutoff);
  falloffAttr->QueryIntValue(&falloff);

  lights.push_back(std::shared_ptr<Light>(new SpotLight(
      *intensityVec, *positionVec, *pointAtVec, cutoff, falloff)));
}

void XmlParser::addDirectionalLight(TiXmlNode *parent) {
  auto intensityVec = getVector(parent, "intensity");
  auto directionVec = getVector(parent, "direction");
  if (intensityVec == nullptr || directionVec == nullptr) {
    std::cout << "Error reading directional light\n";
    return;
  }

  lights.push_back(std::shared_ptr<Light>(
      new DirectionalLight(*intensityVec, *directionVec)));
}

std::shared_ptr<Background> XmlParser::parseColorBackground(
    TiXmlElement *backgroundNode) {
  Background b;
  for (TiXmlNode *pChild = backgroundNode->FirstChild(); pChild != 0;
       pChild = pChild->NextSibling()) {
    if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT &&
        std::string(pChild->Value()) == "color") {
      Pixel p;
      int ival;
      for (TiXmlAttribute *attr = pChild->ToElement()->FirstAttribute();
           attr != nullptr; attr = attr->Next()) {
        std::string attrName(attr->Name());
        if (attrName == "r" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
          p.setR(ival);
        if (attrName == "g" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
          p.setG(ival);
        if (attrName == "b" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
          p.setB(ival);
      }
      b.addColor(p);
    }
  }

  return std::make_shared<Background>(b);
}

// TODO
std::shared_ptr<Background> XmlParser::parseTextureBackground(
    TiXmlElement *backgroundNode) {
  return nullptr;
}

std::shared_ptr<Primitive> XmlParser::getPrimitive(TiXmlElement *parent) {
  if (parent == nullptr) return nullptr;

  auto typeAttr = getAttribute(parent->ToElement(), "type");
  if (typeAttr == nullptr) return nullptr;

  auto type = std::string(typeAttr->Value());
  if (type == "sphere") return getSphere(parent);
  if (type == "trianglemesh") return getTriangleMesh(parent);

  return nullptr;
}

std::shared_ptr<Primitive> XmlParser::getSphere(TiXmlElement *parent) {
  if (parent == nullptr) return nullptr;

  double radius = 0;
  auto radiusNode = getChildWithName(parent, "radius");
  auto center = getVector(parent, "center");
  if (radiusNode == nullptr || center == nullptr) return nullptr;

  auto radiusAttr = getAttribute(radiusNode->ToElement(), "value");
  if (radiusAttr == nullptr) return nullptr;

  radiusAttr->QueryDoubleValue(&radius);

  auto materialAttr = getAttribute(parent, "material");
  if (materialAttr == nullptr) return nullptr;
  auto materialName = std::string(materialAttr->Value());

  for (auto &mat : materials) {
    if (mat->getName() == materialName)
      return std::shared_ptr<Primitive>(new Sphere(radius, *center, mat));
  }

  return nullptr;
}

std::shared_ptr<Primitive> XmlParser::getTriangleMesh(TiXmlElement *parent) {
  if (parent == nullptr) return nullptr;

  auto materialAttr = getAttribute(parent, "material");
  if (materialAttr == nullptr) return nullptr;
  auto materialName = std::string(materialAttr->Value());

  std::shared_ptr<Material> selectedMaterial = nullptr;
  for (auto &mat : materials)
    if (mat->getName() == materialName) {
      selectedMaterial = mat;
      break;
    }
  if (selectedMaterial == nullptr) return nullptr;

  auto fileNode = getChildWithName(parent, "filename");
  if (fileNode != nullptr) {
    auto filenameAttr = getAttribute(fileNode->ToElement(), "value");
    if (filenameAttr != nullptr)
      return getTriangleMeshFromFile(filenameAttr->Value(), selectedMaterial);
    else
      return nullptr;
  }

  std::cout << "parsing triangle manually\n";

  auto nTrianglesNode = getChildWithName(parent, "ntriangles");
  auto indicesNode = getChildWithName(parent, "indices");
  auto verticesNode = getChildWithName(parent, "vertices");
  auto normalsNode = getChildWithName(parent, "normals");
  // auto uvNode = getChildWithName(parent, "uv");
  if (nTrianglesNode == nullptr || indicesNode == nullptr ||
      verticesNode == nullptr || normalsNode == nullptr)
    return nullptr;

  auto nTrianglesAttr = getAttribute(nTrianglesNode->ToElement(), "value");
  auto indicesAttr = getAttribute(indicesNode->ToElement(), "value");
  auto verticesAttr = getAttribute(verticesNode->ToElement(), "value");
  auto normalsAttr = getAttribute(normalsNode->ToElement(), "value");
  // auto uvAttr = getAttribute(uvNode->ToElement(), "value");

  if (nTrianglesAttr == nullptr || indicesAttr == nullptr ||
      verticesAttr == nullptr || normalsAttr == nullptr)
    return nullptr;

  int nTriangles;
  nTrianglesAttr->QueryIntValue(&nTriangles);
  auto indices = getIntVecFromString(indicesAttr->Value());
  auto vertices = getVecVecFromString(verticesAttr->Value());
  auto normals = getVecVecFromString(normalsAttr->Value());

  TriangleMesh mesh(nTriangles, indices.data(), vertices.size(),
                    vertices.data(), normals.data(), selectedMaterial);
  return createBVHFromTriangleMesh(mesh);

  return nullptr;
}

std::vector<int> XmlParser::getIntVecFromString(const std::string &string) {
  std::stringstream ss(string);
  std::vector<int> ret;
  int n;
  while (ss >> n) ret.push_back(n);
  return ret;
}

std::vector<vec3> XmlParser::getVecVecFromString(const std::string &string) {
  std::stringstream ss(string);
  std::vector<vec3> ret;
  float x, y, z;
  while (ss >> x >> y >> z) ret.push_back(vec3(x, y, z));
  return ret;
}

std::shared_ptr<Primitive> XmlParser::getTriangleMeshFromFile(
    const std::string &filename, const std::shared_ptr<Material> &material) {
  cy::TriMesh cyMesh;
  if (!cyMesh.LoadFromFileObj(filename.c_str())) return nullptr;

  auto nTri = cyMesh.NF();
  auto nVer = cyMesh.NV();

  std::vector<int> indices;
  for (auto i = 0u; i < nTri; ++i) {
    auto &triangle = cyMesh.F(i);
    for (auto j = 0u; j < 3; ++j) {
      indices.push_back(triangle.v[j]);
    }
  }

  std::vector<point3> vertices;
  for (auto i = 0u; i < nVer; ++i) {
    const auto &triangle = cyMesh.V(i);
    vertices.push_back(point3(triangle[0], triangle[1], triangle[2]));
  }

  auto numNormals = cyMesh.NVN();
  std::vector<vec3> normals;
  for (auto i = 0u; i < numNormals; ++i) {
    const auto &normal = cyMesh.VN(i);
    normals.push_back(vec3(normal[0], normal[1], normal[2]));
  }

  TriangleMesh mesh(nTri, indices.data(), nVer, vertices.data(), normals.data(),
                    material);
  return createBVHFromTriangleMesh(mesh);
}

std::shared_ptr<Primitive> XmlParser::createBVHFromTriangleMesh(
    const TriangleMesh &mesh) {
  auto triangles = TriangleMesh::getTriangles(mesh);
  return std::shared_ptr<Primitive>(new BVHAccel(triangles));
  // return std::shared_ptr<Primitive>(new ListPrimitive(triangles));
}

std::shared_ptr<vec3> XmlParser::getVector(TiXmlNode *parent,
                                           const std::string &vecName) {
  TiXmlNode *vecNode = getChildWithName(parent, vecName);
  if (vecNode == nullptr) return nullptr;

  double xVal, yVal, zVal;
  for (TiXmlAttribute *attr = vecNode->ToElement()->FirstAttribute();
       attr != nullptr; attr = attr->Next()) {
    std::string attrName(attr->Name());
    if (attrName == "x" || attrName == "r") attr->QueryDoubleValue(&xVal);
    if (attrName == "y" || attrName == "g") attr->QueryDoubleValue(&yVal);
    if (attrName == "z" || attrName == "b") attr->QueryDoubleValue(&zVal);
  }

  return std::make_shared<vec3>(vec3(xVal, yVal, zVal));
}

TiXmlNode *XmlParser::getChildWithName(TiXmlNode *parent,
                                       const std::string &name) {
  TiXmlNode *target = nullptr;
  for (TiXmlNode *pChild = parent->FirstChild();
       pChild != 0 and target == nullptr; pChild = pChild->NextSibling())
    if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT &&
        std::string(pChild->Value()) == name)
      target = pChild;

  return target;
}

TiXmlAttribute *XmlParser::getAttribute(TiXmlElement *parent,
                                        const std::string &name) {
  TiXmlAttribute *attr = parent->FirstAttribute();
  while (attr) {
    if (std::string(attr->Name()) == name) return attr;
    attr = attr->Next();
  }

  return nullptr;
}

Pixel XmlParser::parseColor(TiXmlNode *parent) {
  Pixel p;
  int ival;
  for (TiXmlAttribute *attr = parent->ToElement()->FirstAttribute();
       attr != nullptr; attr = attr->Next()) {
    std::string attrName(attr->Name());
    if (attrName == "r" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setR(ival);
    if (attrName == "g" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setG(ival);
    if (attrName == "b" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
      p.setB(ival);
  }
  return p;
}