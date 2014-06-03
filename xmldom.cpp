#include <iostream>
#include "xmldom.h"
///#include "xmlparser.h"

typedef PointerListIterator<XMLElement> XMLElementListIterator;

XMLNode::XMLNode(const String& _name, const String& _value): name(_name), value(_value) {}

String XMLNode::getName() const
{
    return name;
}

String XMLNode::getValue() const
{
    return value;
}

void XMLNode::setValue(const String& _value)
{
    value = _value;
}

XMLElement::XMLElement(const String& xml, bool)
{
    xml.find("");

}

/*
XMLElement::XMLElement(const String& xml, bool)
{
    XMLElementParser parser(xml);
    name = parser.getName();
    value = parser.getValue();
    ns = parser.getNamespace();
    attributes = parser.getAttributeMap();
    children = parser.getChildrenAsPtrList();
}
*/
/*
XMLElement::XMLElement(std::istream& is = std::cin)
{
    std::string xml, line;

    while (std::getline(is, line))
    {
        xml += line;
        xml += '\n';
    }

    XMLElement(std::wstring(xml.begin(), xml.end()), true);
}

XMLElement::XMLElement(std::wistream& is = std::wcin)
{
    String xml, line;

    while (std::getline(is, line))
    {
        xml += line;
        xml += '\n';
    }

    XMLElement(xml, true);
}
*/
XMLElement::XMLElement(const String& _name, const String& text, const String& _ns): XMLNode(_name, text), ns(_ns) {}

String XMLElement::getNamespace() const
{
    return ns;
}

String XMLElement::getText() const
{
    return value;
}

bool XMLElement::doesAttributeExist(const String& attrname) const
{
    return attributes.find(attrname) != attributes.end();
}

size_t XMLElement::getChildrenCount() const
{
    return children.size();
}

String& XMLElement::getAttribute(const String& attrname)
{
    return doesAttributeExist(attrname) ? attributes[attrname] : attributes.begin()->second;
}

String& XMLElement::operator[](const String& attrname)
{
    return getAttribute(attrname);
}

XMLElement& XMLElement::getChild(size_t pos)
{
    XMLElementListIterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos; ichild++)
        i++;

    return ichild != children.end() ? *ichild : children.back();
}

XMLElement& XMLElement::operator[](size_t pos)
{
    return getChild(pos);
}

void XMLElement::setText(const String& text)
{
    value = text;
}

void XMLElement::addAttribute(const String& name, const String& value)
{
    attributes[name] = value;
}

bool XMLElement::removeAttribute(const String& name)
{
    return attributes.erase(name);
}

void XMLElement::addChild(size_t pos, const XMLElement& child)
{
    XMLElementListIterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos; ichild++)
        i++;

    children.insert(ichild, child);
}

void XMLElement::appendChild(const XMLElement& child)
{
    children.push_back(child);
}

void XMLElement::removeChild(size_t pos)
{
    XMLElementListIterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos; ichild++)
        i++;

    children.erase(ichild);
}
