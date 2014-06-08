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

#include "unicode.h"

typedef std::map<string_t, string_t> XMLAttributeMap;

struct XMLNode
{
    string_t name, value;

    XMLNode(const string_t&, const string_t&);
};

class XMLElement: public XMLNode
{
//protected:
    string_t ns;
    XMLAttributeMap attributes;
    std::list<XMLElement*> children;
    XMLElement* parent;

    friend size_t parseAndAssignXML(const string_t&, XMLElement*);
    friend XMLElement* getPtrToParsedXML(const string_t&);
    friend XMLElement parseXML(const string_t&);
    void assignParsedXML(const string_t&);
public:
    XMLElement(const string_t&, bool);
    XMLElement(istream_t&);
    XMLElement(const string_t& = string_t(), const string_t& = string_t(),
    		const string_t& = string_t());
    //XMLElement(const string_t&, const string_t&, const string_t&,
    //		const XMLAttributeMap&, const std::list<XMLElement*>&, XMLElement*);

    string_t getNamespace() const;
    string_t getText() const;
    bool doesAttributeExist(const string_t&) const;
    size_t getChildrenCount() const;

    string_t& getAttribute(const string_t&);
    string_t& operator[](const string_t&);
    XMLElement& getChild(size_t);
    XMLElement& getParent();
    XMLElement& operator[](size_t);

    void setText(const string_t&);
    void addAttribute(const string_t&, const string_t&);
    bool removeAttribute(const string_t&);
    void addChild(size_t, const XMLElement&);
    void appendChild(const XMLElement&);
    void removeChild(size_t);

    ///std::list<XMLNode*> queryXPath(const string_t&);

    friend ostream_t& operator<<(ostream_t&, const XMLElement&);
};

#endif /* XMLDOM_H_ */
