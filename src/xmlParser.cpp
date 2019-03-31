#include "xmlParser.h"

std::shared_ptr<Background> XmlParser::getBackground() {
    if (m_topElement == nullptr) return nullptr;
    TiXmlElement *backgroundNode = nullptr;
    for (TiXmlNode *pChild = m_topElement->FirstChild(); pChild != 0 and backgroundNode == nullptr;
         pChild = pChild->NextSibling()) {
        if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT) {
            TiXmlElement *el = pChild->ToElement();
            if (std::string(el->Value()) == "background") backgroundNode = el;
        }
    }

    if (backgroundNode == nullptr) return nullptr;

    TiXmlAttribute *attr = backgroundNode->FirstAttribute();
    while (attr) {
        if (std::string(attr->Name()) == "type") {
            if (std::string(attr->Value()) == "color") return parseColorBackground(backgroundNode);
            if (std::string(attr->Value()) == "texture")
                return parseTextureBackground(backgroundNode);
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

std::shared_ptr<Buffer> XmlParser::getBuffer() {
    if (m_topElement == nullptr) return nullptr;
    int width = -1;
    int height = -1;
    int ival;

    TiXmlNode *bufferNode = nullptr;
    for (TiXmlNode *pChild = m_topElement->FirstChild(); pChild != 0 and bufferNode == nullptr;
         pChild = pChild->NextSibling()) {
        if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT) {
            if (std::string(pChild->Value()) == "buffer") bufferNode = pChild;
        }
    }

    if (bufferNode == nullptr) return nullptr;

    TiXmlAttribute *attr = bufferNode->ToElement()->FirstAttribute();
    while (attr) {
        if (std::string(attr->Name()) == "width" && attr->QueryIntValue(&ival) == TIXML_SUCCESS)
            width = ival;
        else if (std::string(attr->Name()) == "height" &&
                 attr->QueryIntValue(&ival) == TIXML_SUCCESS)
            height = ival;

        attr = attr->Next();
    }

    if (width > 0 && height > 0)
        return std::make_shared<Buffer>(Buffer(width, height));
    else
        return nullptr;
}