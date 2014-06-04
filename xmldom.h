/*
 * xmldom.h
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef XMLDOM_H_
#define XMLDOM_H_

#include <list>
#include <map>
#include <iostream>

#include "ptrlist.cpp"

typedef std::wstring XMLString;
typedef std::map<XMLString, XMLString> XMLAttributeMap;

class XMLNode
{
protected:
    XMLString name, value;

public:
    XMLNode(const XMLString& = XMLString(), const XMLString& = XMLString());

    XMLString getName() const;
    XMLString getValue() const;

    void setValue(const XMLString&);
};

class XMLElement: public XMLNode
{
protected:
    XMLString ns;
    XMLAttributeMap attributes;
    PointerList<XMLElement> children;
    XMLElement* parent;

    friend bool assignParsedXML(XMLElement*, const XMLString&);

public:
    XMLElement(const XMLString&, bool);
    XMLElement(std::istream&); // ASCII
    XMLElement(std::wistream&); // Unicode
    XMLElement(const XMLString& = XMLString(), const XMLString& = XMLString(),
    		const XMLString& = XMLString());
    //XMLElement(const XMLString&, const XMLString&, const XMLString&,
    //		const map<XMLString, XMLString>&, const list<XMLElement*>&);

    XMLString getNamespace() const;
    XMLString getText() const;
    bool doesAttributeExist(const XMLString&) const;
    size_t getChildrenCount() const;

    XMLString& getAttribute(const XMLString&);
    XMLString& operator[](const XMLString&);
    XMLElement& getChild(size_t);
    XMLElement& getParent();
    XMLElement& operator[](size_t);

    void setText(const XMLString&);
    void addAttribute(const XMLString&, const XMLString&);
    bool removeAttribute(const XMLString&);
    void addChild(size_t, const XMLElement&);
    void appendChild(const XMLElement&);
    void removeChild(size_t);

    ///PointerList<XMLNode> queryXPath(const XMLString&);

    ///friend ostream& operator<<(ostream&, const XMLElement&);
};

#endif /* XMLDOM_H_ */
