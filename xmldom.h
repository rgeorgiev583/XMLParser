#include <list>
#include <map>
#include <iostream>
#include "ptrlist.cpp"
#include "xmlparser.h"
#pragma once

typedef std::wstring String;
typedef std::map<String, String> XMLAttributeMap;

class XMLNode
{
protected:
    String name, value;

public:
    XMLNode(const String& = String(), const String& = String());

    String getName() const;
    String getValue() const;

    void setValue(const String&);
};

class XMLElement: public XMLNode
{
protected:
    String ns;
    XMLAttributeMap attributes;
    PointerList<XMLElement> children;
    //XMLElement* parent;

public:
    XMLElement(const String&, bool);
    XMLElement(std::istream& = std::cin); // ASCII
    XMLElement(std::wistream& = std::wcin); // Unicode
    XMLElement(const String& = String(), const String& = String(), const String& = String());
    //XMLElement(const String&, const String&, const String&, const map<String, String>&, const list<XMLElement*>&);

    String getNamespace() const;
    String getText() const;
    bool doesAttributeExist(const String&) const;
    size_t getChildrenCount() const;

    String& getAttribute(const String&);
    String& operator[](const String&);
    XMLElement& getChild(size_t);
    XMLElement& operator[](size_t);

    void setText(const String&);
    void addAttribute(const String&, const String&);
    bool removeAttribute(const String&);
    void addChild(size_t, const XMLElement&);
    void appendChild(const XMLElement&);
    void removeChild(size_t);

    ///PointerList<XMLNode> queryXPath(const String&);

    ///friend ostream& operator<<(ostream&, const XMLElement&);
};
