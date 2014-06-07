/*
 * xmldom.cpp
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef XMLDOM_CPP_
#define XMLDOM_CPP_

#include "../headers/xmldom.h"

#include <iostream>

#include "../headers/tokenizer.h"
#include "../headers/name-validation.h"

//typedef PointerListIterator<XMLElement> XMLElementListIterator;
//typedef std::wstring XMLString;

XMLNode::XMLNode(const XMLString& _name = XMLString(),
		const XMLString& _value = XMLString()): name(_name), value(_value) {}

XMLString XMLNode::getName() const
{
    return name;
}

XMLString XMLNode::getValue() const
{
    return value;
}

void XMLNode::setValue(const XMLString& _value)
{
    value = _value;
}

bool assignParsedXML(const XMLString& xml, XMLElement* elem)
{
	// find closing `>' of opening tag and store its position
	size_t endpos_otag_elem = xml.find(">");
	if (endpos_otag_elem == XMLString::npos)
		return false;
	// retrieve opening tag of element
	XMLString otag_elem = xml.substr(0, endpos_otag_elem + 1);
	// create tokenizer for opening tag of element
	Tokenizer tok_otag_elem(otag_elem);
	// retrieve element name from tokenizer
	XMLString qname_elem = tok_otag_elem.getToken();
	Tokenizer tok_qname_elem(qname_elem, ":");
	elem->ns = tok_qname_elem.getToken();
	elem->name = tok_qname_elem.getToken();

	// for each attribute (`name=value' pair) in the elem element
	while (tok_otag_elem.nextToken())
	{
		// retrieve attribute from tokenizer
		XMLString attr_elem = tok_otag_elem.getToken();
		// create tokenizer for attribute
		Tokenizer tok_attr_elem(attr_elem, "=");
		XMLString name_attr_elem = tok_attr_elem.getToken(),
			   value_attr_elem = tok_attr_elem.getToken();
		size_t value_attr_elem_length = value_attr_elem.length();
		if (value_attr_elem[0] == '"' &&
				value_attr_elem[value_attr_elem_length - 1] == '"')
			value_attr_elem = value_attr_elem.substr(1,
					value_attr_elem_length - 1);
		elem->attributes[name_attr_elem] = value_attr_elem;
	}

	size_t endpos_prevtag = endpos_otag_elem,
			beginpos_nexttag = xml.find("<", endpos_prevtag);
	while (beginpos_nexttag != XMLString::npos &&
		   beginpos_nexttag + 1 < xml.length())
	{
		// retrieve upper part of inner content (text) of element
		elem->value.std::wstring::append(xml.substr(endpos_prevtag + 1,
				beginpos_nexttag - endpos_prevtag - 1));
		if (xml[beginpos_nexttag + 1] != '/')
		{
			XMLElement* childelem = new XMLElement();
			childelem->parent = elem;
			elem->children.push_back(childelem);
			if (!assignParsedXML(xml.substr(beginpos_nexttag), childelem))
				return false;
		}
		else
			endpos_prevtag = xml.find(">", beginpos_nexttag);
		// find opening `<' of next tag (NOT necessarily element) and store its
		// position
		beginpos_nexttag = xml.find("<", endpos_prevtag);
	}

	return true;
}

XMLElement::XMLElement(const XMLString& xml, bool)
{
	/* find the first occurence of `<' (de facto beginning of XML document)
	 *  and the last  occurence of `>' (de facto end of XML document)
	 **/
	size_t beginpos_root = xml.find("<"), endpos_root = xml.rfind(">");
	/*
	 * reduce string to XML only (remove leading and trailing junk that is not
	 * part of the XML document)
	 **/
	assignParsedXML(xml.substr(beginpos_root, endpos_root - beginpos_root + 1),
			this);
}

XMLElement::XMLElement(std::istream& is): parent(NULL)
{
	XMLString xml;
	std::string line;

    while (!is.eof())
    {
    	std::getline(is, line);
        xml += line;
        xml += '\n';
    }

    XMLElement(xml, true);
}

XMLElement::XMLElement(std::wistream& is): parent(NULL)
{
    XMLString xml, line;

    while (!is.eof())
    {
    	std::getline(is, line);
        xml += line;
        xml += '\n';
    }

    XMLElement(xml, true);
}

XMLElement::XMLElement(const XMLString& _name, const XMLString& text,
		const XMLString& _ns): XMLNode(_name, text), ns(_ns), parent(NULL) {}

XMLString XMLElement::getNamespace() const
{
    return ns;
}

XMLString XMLElement::getText() const
{
    return value;
}

bool XMLElement::doesAttributeExist(const XMLString& attrname) const
{
    return attributes.find(attrname) != attributes.end();
}

size_t XMLElement::getChildrenCount() const
{
    return children.size();
}

XMLString& XMLElement::getAttribute(const XMLString& attrname)
{
    return doesAttributeExist(attrname) ? attributes[attrname] :
    		attributes.begin()->second;
}

XMLString& XMLElement::operator[](const XMLString& attrname)
{
    return getAttribute(attrname);
}

XMLElement& XMLElement::getChild(size_t pos)
{
	std::list<XMLElement*>::iterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos;
    		ichild++)
        i++;

    return ichild != children.end() ? **ichild : *children.back();
}

XMLElement& XMLElement::operator[](size_t pos)
{
    return getChild(pos);
}

void XMLElement::setText(const XMLString& text)
{
    value = text;
}

void XMLElement::addAttribute(const XMLString& name, const XMLString& value)
{
    attributes[name] = value;
}

bool XMLElement::removeAttribute(const XMLString& name)
{
    return attributes.erase(name);
}

void XMLElement::addChild(size_t pos, const XMLElement& child)
{
	std::list<XMLElement*>::iterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos;
    		ichild++)
        i++;

    children.insert(ichild, (XMLElement*) &child);
}

void XMLElement::appendChild(const XMLElement& child)
{
    children.push_back((XMLElement*) &child);
}

void XMLElement::removeChild(size_t pos)
{
	std::list<XMLElement*>::iterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos;
    		ichild++)
        i++;

    children.erase(ichild);
}

std::wostream& operator<<(std::wostream& os, const XMLElement& elem)
{
	os << "XML element `" << elem.name << "'";
	if (elem.parent)
		os << ", child of `" << elem.parent->name << "'";
	os << std::endl;
	os << "Namespace: `" << elem.ns << "'" << std::endl;
	os << "Text content:";
	os << "------------------------" << std::endl;
	os << elem.value << std::endl;
	os << "------------------------" << std::endl;
	os << "Attributes: " << std::endl;

	for (XMLAttributeMap::const_iterator i = elem.attributes.begin();
			i != elem.attributes.end(); i++)
		os << "|-  " << i->first << " = `" << i->second << "'" << std::endl;

	for (std::list<XMLElement*>::const_iterator i = elem.children.begin();
			i != elem.children.end(); i++)
		operator<<(os, **i);

	return os;
}

#endif /* XMLDOM_CPP_ */
