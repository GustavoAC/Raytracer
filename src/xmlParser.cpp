#include "xmlParser.h"
#include "orthographic_camera.h"
#include "perspective_camera.h"

std::shared_ptr<Background> XmlParser::getBackground() {
    if (m_topElement == nullptr) return nullptr;
    TiXmlNode *backgroundNode = getChildWithName(m_topElement, "background");

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

std::shared_ptr<Background> XmlParser::parseColorBackground(TiXmlElement *backgroundNode) {
    Background b;
    for (TiXmlNode *pChild = backgroundNode->FirstChild(); pChild != 0;
         pChild = pChild->NextSibling()) {
        if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT &&
            std::string(pChild->Value()) == "color") {
            Pixel p;
            int ival;
            for (TiXmlAttribute *attr = pChild->ToElement()->FirstAttribute(); attr != nullptr;
                 attr = attr->Next()) {
                std::string attrName(attr->Name());
                if (attrName == "r" && attr->QueryIntValue(&ival) == TIXML_SUCCESS) p.setR(ival);
                if (attrName == "g" && attr->QueryIntValue(&ival) == TIXML_SUCCESS) p.setG(ival);
                if (attrName == "b" && attr->QueryIntValue(&ival) == TIXML_SUCCESS) p.setB(ival);
            }
            b.addColor(p);
        }
    }

    return std::make_shared<Background>(b);
}

// TODO
std::shared_ptr<Background> XmlParser::parseTextureBackground(TiXmlElement *backgroundNode) {
    return nullptr;
}

std::shared_ptr<Camera> XmlParser::getCamera() {
    if (m_topElement == nullptr) return nullptr;

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
    if (position == nullptr || target == nullptr || up == nullptr || width == nullptr ||
        height == nullptr)
        return nullptr;

    auto widthAttr = getAttribute(width->ToElement(), "value");
    auto heightAttr = getAttribute(height->ToElement(), "value");
    if (widthAttr == nullptr || heightAttr == nullptr) return nullptr;

    int widthVal = -1;
    int heightVal = -1;
    widthAttr->QueryIntValue(&widthVal);
    heightAttr->QueryIntValue(&heightVal);
    if (widthVal < 0 || heightVal < 0) return nullptr;

    Camera *camera = nullptr;
    if (isOrthographic) {
        auto vpdimNode = getChildWithName(cameraNode, "vpdim");
        if (vpdimNode == nullptr) return nullptr;
        auto vpdim = vpdimNode->ToElement();

        auto l = getAttribute(vpdim, "l");
        auto r = getAttribute(vpdim, "r");
        auto b = getAttribute(vpdim, "b");
        auto t = getAttribute(vpdim, "t");
        if (l == nullptr || r == nullptr || b == nullptr || t == nullptr) return nullptr;

        double lVal, rVal, bVal, tVal;
        l->QueryDoubleValue(&lVal);
        r->QueryDoubleValue(&rVal);
        b->QueryDoubleValue(&bVal);
        t->QueryDoubleValue(&tVal);

        camera = new OrthographicCamera(widthVal, heightVal, *position,
                                                             *target, *up, lVal, rVal, bVal, tVal);
    } else {
        auto fovyNode = getChildWithName(cameraNode, "fovy");
        auto aspectNode = getChildWithName(cameraNode, "aspect");
        auto fdistanceNode = getChildWithName(cameraNode, "fdistance");
        if (fovyNode == nullptr || aspectNode == nullptr) return nullptr;

        auto fovyAttr = getAttribute(fovyNode->ToElement(), "value");
        auto aspectAttr = getAttribute(aspectNode->ToElement(), "value");
        auto fdistanceAttr = (fdistanceNode != nullptr)
                                 ? getAttribute(fdistanceNode->ToElement(), "value")
                                 : nullptr;
        if (fovyAttr == nullptr || aspectAttr == nullptr) return nullptr;

        double fovy, aspect, fdistance = 1.0;
        fovyAttr->QueryDoubleValue(&fovy);
        aspectAttr->QueryDoubleValue(&aspect);
        if (fdistanceAttr != nullptr) fdistanceAttr->QueryDoubleValue(&fdistance);

        camera = new PerspectiveCamera(widthVal, heightVal, *position, *target,
                                                            *up, fovy, aspect, fdistance);
    }

    return std::shared_ptr<Camera>(camera);
}

std::shared_ptr<vec3> XmlParser::getVector(TiXmlNode *parent, const std::string &vecName) {
    TiXmlNode *vecNode = getChildWithName(parent, vecName);
    if (vecNode == nullptr) return nullptr;

    double xVal, yVal, zVal;
    for (TiXmlAttribute *attr = vecNode->ToElement()->FirstAttribute(); attr != nullptr;
         attr = attr->Next()) {
        std::string attrName(attr->Name());
        if (attrName == "x") attr->QueryDoubleValue(&xVal);
        if (attrName == "y") attr->QueryDoubleValue(&yVal);
        if (attrName == "z") attr->QueryDoubleValue(&zVal);
    }

    return std::make_shared<vec3>(vec3(xVal, yVal, zVal));
}

TiXmlNode *XmlParser::getChildWithName(TiXmlNode *parent, const std::string &name) {
    TiXmlNode *target = nullptr;
    for (TiXmlNode *pChild = parent->FirstChild(); pChild != 0 and target == nullptr;
         pChild = pChild->NextSibling())
        if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && std::string(pChild->Value()) == name)
            target = pChild;

    return target;
}

TiXmlAttribute *XmlParser::getAttribute(TiXmlElement *parent, const std::string &name) {
    TiXmlAttribute *attr = parent->FirstAttribute();
    while (attr) {
        if (std::string(attr->Name()) == name) return attr;
        attr = attr->Next();
    }

    return nullptr;
}
