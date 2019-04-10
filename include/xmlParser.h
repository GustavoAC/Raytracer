#pragma once
#include <iostream>
#include <memory>
#include "background.h"
#include "buffer.h"
#include "camera.h"
#include "vec3.h"
#include "config.h"
#include "tinyxml.h"

class XmlParser {
   private:
    std::string m_filename;
    std::shared_ptr<TiXmlDocument> m_doc;
    TiXmlElement *m_topElement = nullptr;

    XmlParser(std::string _filename) : m_filename(_filename) {
        m_doc = std::make_shared<TiXmlDocument>(TiXmlDocument(m_filename.c_str()));
    };

    std::shared_ptr<Background> parseColorBackground(TiXmlElement *backgroundNode);
    std::shared_ptr<Background> parseTextureBackground(TiXmlElement *backgroundNode);
    TiXmlNode* getChildWithName(TiXmlNode *parent, const std::string &name);
    TiXmlAttribute* getAttribute(TiXmlElement *parent, const std::string &name);

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

    std::shared_ptr<Background> getBackground();
    std::shared_ptr<Camera> getCamera();
    std::shared_ptr<vec3> getVector(TiXmlNode* parent, const std::string &vecName);
};