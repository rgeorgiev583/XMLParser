/*
 * xmldom.cpp
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef XMLDOM_CPP_
#define XMLDOM_CPP_

#include "../headers/xmldom.h"

#include "../headers/tokenizer.h"

//typedef PointerListIterator<XMLElement> XMLElementListIterator;

XMLNode::XMLNode(const string_t& _name = string_t(),
		const string_t& _value = string_t()): name(_name), value(_value) {}

size_t parseAndAssignXML(const string_t& xml, XMLElement* elem)
{
	// find closing `>' of opening tag and store its position
	size_t endpos_otag_elem = xml.find(">");
	if (endpos_otag_elem == string_t::npos)
		return string_t::npos;
	// retrieve opening tag of element
	string_t otag_elem = xml.substr(1, endpos_otag_elem - 1);
	// create tokenizer for opening tag of element
	Tokenizer tok_otag_elem(otag_elem);

	// retrieve element name from tokenizer
	if (tok_otag_elem.nextToken())
	{
		string_t qname_elem = tok_otag_elem.getToken();
		Tokenizer tok_qname_elem(qname_elem, ":");
		if (tok_qname_elem.nextToken())
		{
			elem->ns = tok_qname_elem.getToken();

			if (tok_qname_elem.nextToken())
				elem->name = tok_qname_elem.getToken();
			else
				elem->name.swap(elem->ns);
		}
	}

	// for each attribute (`name=value' pair) in the elem element
	while (tok_otag_elem.nextToken())
	{
		// retrieve attribute from tokenizer
		string_t attr_elem = tok_otag_elem.getToken();
		// create tokenizer for attribute
		Tokenizer tok_attr_elem(attr_elem, "=");

		if (tok_attr_elem.nextToken())
		{
			string_t name_attr_elem = tok_attr_elem.getToken();

			if (tok_attr_elem.nextToken())
			{
				string_t value_attr_elem = tok_attr_elem.getToken();
				size_t value_attr_elem_length = value_attr_elem.length();
				if (value_attr_elem[0] == '"' &&
						value_attr_elem[value_attr_elem_length - 1] == '"')
					value_attr_elem = value_attr_elem.substr(1,
							value_attr_elem_length - 1);
				elem->attributes[name_attr_elem] = value_attr_elem;
			}
			else
				elem->attributes[name_attr_elem] = STR("");
		}
	}

	size_t beginpos_nexttag = xml.find("<", endpos_otag_elem);
	/*if (beginpos_nexttag != string_t::npos &&
			beginpos_nexttag + 1 < xml.length())
	{*/
	while (beginpos_nexttag != string_t::npos &&
			beginpos_nexttag + 1 < xml.length() &&
			xml.substr(beginpos_nexttag + 1,
			elem->name.length() + 1) != "/" + elem->name)
	{
		// retrieve upper part of inner content (text) of element
		elem->value.append(xml.substr(endpos_otag_elem + 1,
				beginpos_nexttag - endpos_otag_elem - 1));
		if (xml[beginpos_nexttag + 1] != '/')
		{
			XMLElement* childelem = new XMLElement();
			childelem->parent = elem;
			elem->children.push_back(childelem);
			endpos_otag_elem = beginpos_nexttag +
					parseAndAssignXML(xml.substr(beginpos_nexttag), childelem);
			if (endpos_otag_elem == string_t::npos)
				return string_t::npos;
		}
		else
			endpos_otag_elem = xml.find(">", beginpos_nexttag);

		beginpos_nexttag = xml.find("<", endpos_otag_elem);
	}

	if (beginpos_nexttag != string_t::npos &&
			beginpos_nexttag + 1 < xml.length())
	{
		elem->value.append(xml.substr(endpos_otag_elem + 1,
						beginpos_nexttag - endpos_otag_elem - 1));
		endpos_otag_elem = xml.find(">", beginpos_nexttag);
		return endpos_otag_elem;
	}
	else
		return string_t::npos;
}

XMLElement* getPtrToParsedXML(const string_t& xml)
{
	/*
	 * find the first occurence of `<' (de facto beginning of XML document)
	 *  and the last  occurence of `>' (de facto end of XML document)
	 **/
	size_t beginpos_root = xml.find("<"), endpos_root = xml.rfind(">");
	/*
	 * reduce string to XML only (remove leading and trailing junk that is not
	 * part of the XML document)
	 **/
	if (beginpos_root != string_t::npos && endpos_root != string_t::npos)
	{
		XMLElement* root = new XMLElement();
		root->parent = NULL;
		size_t res = parseAndAssignXML(
				xml.substr(beginpos_root, endpos_root - beginpos_root + 1),
				root);
		return res != string_t::npos ? root : NULL;
	}
	else
		return NULL;
}

XMLElement parseXML(const string_t& xml)
{
	return *getPtrToParsedXML(xml);
}

void XMLElement::assignParsedXML(const string_t& xml)
{
	XMLElement* elem = getPtrToParsedXML(xml);
	name = elem->name;
	value = elem->value;
	ns = elem->ns;
	attributes = elem->attributes;
	children = elem->children;
	parent = elem->parent;
}

XMLElement::XMLElement(const string_t& xml, bool)
{
	assignParsedXML(xml);
}

XMLElement::XMLElement(istream_t& is)
{
	string_t xml, line;

    while (!is.eof())
    {
    	getline(is, line);
        xml += line;
        xml += '\n';
    }

    assignParsedXML(xml);
}

XMLElement::XMLElement(const string_t& _name, const string_t& text,
		const string_t& _ns): XMLNode(_name, text), ns(_ns), parent(NULL) {}

string_t XMLElement::getNamespace() const
{
    return ns;
}

string_t XMLElement::getText() const
{
    return value;
}

bool XMLElement::doesAttributeExist(const string_t& attrname) const
{
    return attributes.find(attrname) != attributes.end();
}

size_t XMLElement::getChildrenCount() const
{
    return children.size();
}

string_t& XMLElement::getAttribute(const string_t& attrname)
{
    return doesAttributeExist(attrname) ? attributes[attrname] :
    		attributes.begin()->second;
}

string_t& XMLElement::operator[](const string_t& attrname)
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

void XMLElement::setText(const string_t& text)
{
    value = text;
}

void XMLElement::addAttribute(const string_t& name, const string_t& value)
{
    attributes[name] = value;
}

bool XMLElement::removeAttribute(const string_t& name)
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

ostream_t& operator<<(ostream_t& os, const XMLElement& elem)
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
