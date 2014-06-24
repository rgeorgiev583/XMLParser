/*
 * parser.cpp
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef PARSER_CPP_
#define PARSER_CPP_

#include "../headers/parser.h"

#include "../headers/validation.h"
#include "../headers/word-tokenizer.h"

#ifndef NOTHROW
#define RAISE_PARSE_ERROR(type) throw XMLParserError(type, \
		tok_stag_elem.getTokenPosition() != string_t::npos ? \
		tok_stag_elem.getTokenPosition() : tok_stag_elem.getOffset())
#define RAISE_PARSE_ERROR_AT_OFFSET(type, offset) \
		throw XMLParserError(type, offset)
#define RAISE_XPATH_ERROR(type) throw XPathQueryError(type, \
		tok_pred.getTokenPosition() != string_t::npos ? \
		tok_pred.getTokenPosition() : tok_pred.getOffset())
#define RAISE_XPATH_ERROR_AT_OFFSET(type, offset) \
		throw XPathQueryError(type, offset)
#else
#define RAISE_PARSE_ERROR(type) return string_t::npos
#define RAISE_PARSE_ERROR_AT_OFFSET(type, offset) return string_t::npos
#define RAISE_XPATH_ERROR(type) return string_t::npos
#define RAISE_XPATH_ERROR_AT_OFFSET(type, offset) return string_t::npos
#endif

//typedef PointerListIterator<XMLElement> XMLElementListIterator;

//Error::Error(size_t _pos = 0): pos(_pos) {}

XMLParserError::XMLParserError(XMLParserErrorType _type = PARSE_NO_ERR,
		size_t _pos = 0): Error(_pos), type(_type) {}

XPathQueryError::XPathQueryError(XPathQueryErrorType _type = XPATH_NO_ERR,
		size_t _pos = 0): Error(_pos), type(_type) {}

XMLNode::XMLNode(const string_t& _name = string_t(),
		const string_t& _value = string_t(),
		const string_t& _ns = string_t(), XMLElement* _parent = NULL):
		name(_name), value(_value), ns(_ns), parent(_parent) {}

XMLNode splitQualifiedName(const string_t& qname, size_t offset = 0)
{
	Tokenizer tok_qname(qname, ":");
	if (!tok_qname.nextToken())
		RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR,
				offset + tok_qname.getOffset());

	string_t prefix = tok_qname.getToken();
	if (!isValidName(prefix))
		RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR,
				offset + tok_qname.getTokenPosition());

	if (!tok_qname.wereDelimitersFound())
		return XMLNode(prefix);
	if (!tok_qname.nextToken())
		RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR,
				offset + tok_qname.getOffset());

	string_t name = tok_qname.getToken();
	if (!isValidName(name))
		RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR,
				offset + tok_qname.getTokenPosition());

	return XMLNode(name, "", prefix);
}

size_t parseAndAssignXML(const string_t& xml, XMLElement* elem)
{
	/*// find closing `>' of opening tag and store its position
	size_t endpos_stag_elem = xml.find(">");
	if (endpos_stag_elem == string_t::npos)
		return string_t::npos;
	// retrieve opening tag of element
	size_t closing_slashpos = xml.rfind("/", endpos_stag_elem);
	size_t
	if (closing_slashpos)

	size_t endpos_stag_attr = closing_slashpos != string_t::npos

	string_t stag_elem = ;

	if (stag_elem)
	{

	}*/
	// create tokenizer for opening tag of element
	// current location in document
	Tokenizer tok_stag_elem(xml);

	// retrieve element name from tokenizer
	if (!tok_stag_elem.nextToken(Tokenizer::WHITESPACE + ">"))
		RAISE_PARSE_ERROR(PARSE_ELEMOTAG_NOTFOUND_ERR);
	string_t qname_elem = tok_stag_elem.getToken();
	if (qname_elem[0] != '<')
		RAISE_PARSE_ERROR(PARSE_ELEMOTAG_NOTFOUND_ERR);
	if (qname_elem == "<")
		RAISE_PARSE_ERROR(PARSE_AFTEROTAG_WHITESPACE_ERR);
    qname_elem = qname_elem.substr(1);
	//elem->ns = elem->ns[elem->ns.length() - 1] == '>' ?
			//elem->ns.substr(1, elem->ns.length() - 2) : elem->ns.substr(1);
    XMLNode ns_name_pair_elem = splitQualifiedName(qname_elem,
    		tok_stag_elem.getTokenPosition());
    elem->ns = ns_name_pair_elem.ns;
    elem->name = ns_name_pair_elem.name;

	// for each attribute (`name=value' pair) in the elem element
	while (tok_stag_elem.nextToken(Tokenizer::WHITESPACE + "=") &&
			tok_stag_elem.getToken()[0] != '>' &&
			tok_stag_elem.getToken().substr(0, 2) != "/>")
	{
		string_t qname_attr_elem = tok_stag_elem.getToken();
		XMLNode attr_elem = splitQualifiedName(qname_attr_elem,
				tok_stag_elem.getTokenPosition());
		if (!tok_stag_elem.nextToken(Tokenizer::WHITESPACE + "\"") ||
				tok_stag_elem.getToken() != "=")
			RAISE_PARSE_ERROR(PARSE_ATTR_UNASSIGNED_ERR);
		if (!tok_stag_elem.nextToken())
			RAISE_PARSE_ERROR(PARSE_ATTR_NOVALUE_ERR);
		if (tok_stag_elem.getToken()[0] != '"' &&
				 tok_stag_elem.getToken()[0] != '\'')
			RAISE_PARSE_ERROR(PARSE_ATTRVALUE_UNQUOTED_ERR);

		tok_stag_elem.rewind(tok_stag_elem.getTokenPosition() + 1);
		if (xml[tok_stag_elem.getTokenPosition()] == '"')
		{
			if (!tok_stag_elem.nextToken("\""))
				RAISE_PARSE_ERROR(PARSE_ATTRVALUE_UNCLOSEDQUOTE_ERR);
		}
		else if (xml[tok_stag_elem.getTokenPosition()] == '\'')
		{
			if (!tok_stag_elem.nextToken("'"))
				RAISE_PARSE_ERROR(PARSE_ATTRVALUE_UNCLOSEDQUOTE_ERR);
		}

		attr_elem.value = tok_stag_elem.getToken();
		if (!isEscaped(attr_elem.value))
			RAISE_PARSE_ERROR(PARSE_ATTRVALUE_INVALID_ERR);

        if (!tok_stag_elem.nextToken(Tokenizer::WHITESPACE + ">"))
            RAISE_PARSE_ERROR(PARSE_ATTRVALUE_UNCLOSEDQUOTE_ERR);
        else
        {
        	string_t closing_quote_attr_elem = tok_stag_elem.getToken();

        	if (closing_quote_attr_elem != "\"")
        		RAISE_PARSE_ERROR(
        				closing_quote_attr_elem[0] != '"' ?
        				PARSE_ATTRVALUE_UNCLOSEDQUOTE_ERR :
        				PARSE_BETWEENATTRS_NOWHITESPACE_ERR
        		);
        }

        elem->attributes[attr_elem.name] = attr_elem;
		/*else
		{


			if (tok_stag_elem.nextToken() &&
					tok_stag_elem.getToken() == "=")



			}
		else
			return string_t::npos;*/
	}

	// find closing `>' of opening tag and store its position
	// if the element is empty, find closing `/' and store its position

	size_t endpos_stag_elem = tok_stag_elem.getTokenPosition();
	string_t ctag_elem = tok_stag_elem.getToken();
	if (endpos_stag_elem == string_t::npos || ctag_elem[0] != '>' &&
			ctag_elem.substr(0, 2) != "/>")
		RAISE_PARSE_ERROR(PARSE_ELEMCTAG_NOTFOUND_ERR);
	if (ctag_elem == "/>")
		return endpos_stag_elem + 1;

	size_t beginpos_nexttag = xml.find("<", endpos_stag_elem);
	/*if (beginpos_nexttag != string_t::npos &&
			beginpos_nexttag + 1 < xml.length())
	{*/

	while (beginpos_nexttag != string_t::npos &&
		   beginpos_nexttag + 1 < xml.length() &&
		   xml.substr(beginpos_nexttag + 1,
		   elem->name.length() + 1) != "/" + elem->name)
	{
		// retrieve upper part of inner content (text) of element
		elem->value.append(xml.substr(endpos_stag_elem + 1,
				beginpos_nexttag - endpos_stag_elem - 1));
		if (xml[beginpos_nexttag + 1] != '/')
		{
			XMLElement* childelem = new XMLElement();
			childelem->parent = elem;
			elem->children.push_back(childelem);
			endpos_stag_elem = beginpos_nexttag +
					parseAndAssignXML(xml.substr(beginpos_nexttag), childelem);
			if (endpos_stag_elem == string_t::npos)
				return string_t::npos;
		}
		else
			endpos_stag_elem = xml.find(">", beginpos_nexttag);

		beginpos_nexttag = xml.find("<", endpos_stag_elem);
	}

	if (beginpos_nexttag != string_t::npos &&
			beginpos_nexttag + 1 < xml.length())
	{
		elem->value.append(xml.substr(endpos_stag_elem + 1,
						beginpos_nexttag - endpos_stag_elem - 1));
		endpos_stag_elem = xml.find(">", beginpos_nexttag);
		return endpos_stag_elem;
	}
	else
		RAISE_PARSE_ERROR(PARSE_ELEMOTAG_UNMATCHED_ERR);
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
	if (elem)
	{
		name = elem->name;
		value = elem->value;
		ns = elem->ns;
		attributes = elem->attributes;
		children = elem->children;
		parent = elem->parent;
	}
}

XMLElement::XMLElement() {}

XMLElement::XMLElement(const string_t& _name = string_t(),
		const string_t& text = string_t(), const string_t& _ns = string_t(),
		XMLElement* _parent = NULL): XMLNode(_name, text, _ns, _parent) {}

XMLElement::XMLElement(const string_t& _name, const string_t& text,
		const string_t& _ns, const XMLAttributeMap& _attributes,
		const XMLElementList& _children, XMLElement* _parent = NULL):
			XMLNode(_name, text, _ns, _parent), attributes(_attributes),
			children(_children) {}

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

string_t XMLElement::getName() const
{
	return name;
}

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
    return doesAttributeExist(attrname) ? attributes[attrname].value :
    		attributes.begin()->second.value;
}

string_t& XMLElement::operator[](const string_t& attrname)
{
    return getAttribute(attrname);
}

XMLElement& XMLElement::getChild(size_t pos)
{
	XMLElementList::iterator ichild;
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

void XMLElement::addAttribute(const string_t& name,
		const string_t& value = string_t(), const string_t& ns = string_t())
{
	attributes[name].name = name;
    attributes[name].value = value;
    attributes[name].ns = ns;
}

bool XMLElement::removeAttribute(const string_t& name)
{
    return attributes.erase(name);
}

void XMLElement::addChild(size_t pos, const XMLElement& child)
{
	XMLElementList::iterator ichild;
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
	XMLElementList::iterator ichild;
    size_t i = 0;

    for (ichild = children.begin(); ichild != children.end() && i < pos;
    		ichild++)
        i++;

    children.erase(ichild);
}

/*

XPathBinaryOperatorResult evalXPathPredicateExprBinaryOperator(
		const string_t& expr, XMLElement* tree, const string_t& elem_name,
		XPathOperator op, size_t offset
		)
{
	WordTokenizer tok_expr;

	switch (op)
	{
	case XPATH_OR:
		tok_expr = WordTokenizer(expr, " or ");
		break;

	case XPATH_AND:
		tok_expr = WordTokenizer(expr, " and ");
		break;
	}

	if (!tok_expr.nextToken())
		RAISE_XPATH_ERROR_AT_OFFSET(XPATH_PREDICATE_INVALID_EXPR_ERR,
				offset + tok_expr.getOffset());
	if (tok_expr.getDelimiterFoundCount() == 1)
	{
		bool left_operand =
				evalXPathPredicateExpr(tok_expr.getToken(), tree, elem_name,
				offset + tok_expr.getTokenPosition());

		if (!tok_expr.nextToken())
			RAISE_XPATH_ERROR_AT_OFFSET(XPATH_PREDICATE_INVALID_EXPR_ERR,
					offset + tok_expr.getOffset());

		bool right_operand =
				evalXPathPredicateExpr(tok_expr.getToken(), tree, elem_name,
				offset + tok_expr.getTokenPosition());

		switch (op)
		{
		case XPATH_OR:
			return left_operand || right_operand ? XPATH_TRUE : XPATH_FALSE;

		case XPATH_AND:
			return left_operand && right_operand ? XPATH_TRUE : XPATH_FALSE;

		default:
			RAISE_XPATH_ERROR_AT_OFFSET(XPATH_PREDICATE_INVALID_EXPR_ERR,
					offset + tok_expr.getOffset());
		}
	}
	else
		return XPATH_NOTFOUND;
}

bool evalXPathBooleanExpr(const string_t& expr, XMLElement* tree,
		const string_t& elem_name, size_t offset)
{
	for (size_t op = XPATH_MOD; op <= XPATH_OR; op++)
	{
		XPathBinaryOperatorResult result =
				evalXPathPredicateExprBinaryOperator(expr, tree, elem_name,
				(XPathOperator) op, offset);
		if (result != XPATH_NOTFOUND)
			return result;
	}
}
*/
/*
XMLNodeList XMLElement::queryXPath(const string_t& xpath)
{
	std::list<XMLNode*> nodelist;
	Tokenizer tok_xpath(xpath, "/");

	if (xpath.substr(0,2) == "//")
	{

	}
	else if (xpath[0] == '/')
	{
		return XMLNodeList();
	}
	else
	{
		XMLElement* current_node = this;



		Tokenizer tok_qname(qname, ":");
		if (!tok_qname.nextToken())
			RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR, offset + tok_qname.getOffset());

		string_t prefix = tok_qname.getToken();
		if (!isValidName(prefix))
			RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR, offset + tok_qname.getTokenPosition());

		if (!tok_qname.wereDelimitersFound())
			return XMLNode(prefix);
		if (!tok_qname.nextToken())
			RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR, offset + tok_qname.getOffset());

		string_t name = tok_qname.getToken();
		if (!isValidName(name))
			RAISE_PARSE_ERROR_AT_OFFSET(PARSE_NAME_INVALID_ERR, offset + tok_qname.getTokenPosition());

		return XMLNode(name, "", prefix);




		while (tok_xpath.nextToken())
		{
			string_t expr = tok_xpath.getToken();

			size_t pred_begin = expr.find("[");
			size_t index_constraint = string_t::npos;
			XMLNode attr_constraint;
			string_t attr_elem_name;

			if (pred_begin != string_t::npos)
				if (pred_begin + 1 < expr.length() && pred[pred_begin + 1] != '@')
			{
				size_t pred_end = expr.find("]", pred_begin);
				string_t pred = expr.substr(pred_begin + 1,
						pred_end - pred_begin - 1);
				size_t pos = strtol(pred.c_str(), NULL, 10);
				if (pos)
					nodelist.push_back(children[pos]);
				else
				{
					Tokenizer tok_pred(pred, "=");
					if (!tok_pred.nextToken())
						RAISE_XPATH_ERROR(XPATH_CRITERION_NAME_INVALID_ERR,
								tok_xpath.offset() + tok_pred.getOffset());

					string_t criterion_name = tok_pred.getToken();
					if (!isValidName(criterion_name))
						RAISE_XPATH_ERROR(XPATH_CRITERION_NAME_INVALID_ERR,
								tok_xpath.offset() +
								tok_pred.getTokenPosition());

					if (!tok_pred.wereDelimitersFound())
						return

				}
				while (i != children.end() && (*i)->attributes[]);
				/*for (XMLElementList::iterator i = children.begin();
						; i++)
					if ()
					(*i)->
				}*//*
			}
/*
			for (XMLElementList::iterator i = children.begin();
					i != children.end(); i++)
			{
				if ()
				(*i)->*
			}*//*
			if (current_node->name == nodename)
				nodelist.push_back(current_node);
		}
	}

	return std::list<XMLNode*>(xpath);
}
*/
ostream_t& operator<<(ostream_t& os, const XMLElement& elem)
{
	os << "=====================================" << std::endl;
	os << "XML element `" << elem.name << "'";

	if (elem.parent)
		os << ", child of `" << elem.parent->name << "'";
	os << std::endl << "=====================================" << std::endl
			<< std::endl;
	if (elem.ns != "")
		os << "from namespace `" << elem.ns << "'" << std::endl;
	os << "Text content:" << std::endl;
	os << "-------------------------------------" << std::endl;
	os << elem.value << std::endl;
	os << "-------------------------------------" << std::endl << std::endl;
	os << "Attributes: ";
	if (elem.attributes.begin() == elem.attributes.end())
		os << "(none)";

	os << std::endl;

	for (XMLAttributeMap::const_iterator i = elem.attributes.begin();
			i != elem.attributes.end(); i++)
	{
		os << "|-  " << i->first << " = `" << i->second.value << "'";
		if (i->second.ns != "")
			os << " from namespace `" << i->second.ns << "'";
		os << std::endl;
	}

	os << std::endl;

	os << "Children (direct descendants): ";
	if (elem.children.begin() == elem.children.end())
		os << "(none)";

	os << std::endl;

	for (XMLElementList::const_iterator i = elem.children.begin();
				i != elem.children.end(); i++)
		os << "|-  " << (*i)->name << std::endl;

	os << std::endl;
	os << std::endl;

	for (XMLElementList::const_iterator i = elem.children.begin();
			i != elem.children.end(); i++)
		operator<<(os, **i);

	return os;
}

#endif /* PARSER_CPP_ */
