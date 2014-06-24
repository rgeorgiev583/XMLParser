/*
 * parser.h
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <map>

#include "stringdef.h"

enum XMLParserErrorType
{
	PARSE_NO_ERR,
	PARSE_ELEMOTAG_NOTFOUND_ERR,
	PARSE_ELEMCTAG_NOTFOUND_ERR,
	PARSE_AFTEROTAG_WHITESPACE_ERR,
	PARSE_STAG_UNCLOSED_ERR,
	PARSE_NAME_INVALID_ERR,
	PARSE_ATTR_UNASSIGNED_ERR,
	PARSE_ATTR_NOVALUE_ERR,
	PARSE_ATTRVALUE_UNQUOTED_ERR,
	PARSE_ATTRVALUE_UNCLOSEDQUOTE_ERR,
	PARSE_ATTRVALUE_INVALID_ERR,
	PARSE_BETWEENATTRS_NOWHITESPACE_ERR,
	PARSE_ELEMOTAG_UNMATCHED_ERR
};

enum XPathQueryErrorType
{
	XPATH_NO_ERR,
	XPATH_CRITERION_NAME_INVALID_ERR,
	XPATH_PREDICATE_INVALID_EXPR_ERR
};

enum XPathOperator
{
	XPATH_OR,
	XPATH_AND,
	XPATH_EQ,
	XPATH_NEQ,
	XPATH_GTE,
	XPATH_GT,
	XPATH_LTE,
	XPATH_LT,
	XPATH_MINUS,
	XPATH_PLUS,
	XPATH_DIV,
	XPATH_TIMES,
	XPATH_MOD,
	XPATH_UNARY_MINUS
};

//XPathOperator XPathOperatorPrecedenceClasses[6] = {XPATH_OR};

enum XPathBinaryOperatorResult
{
	XPATH_FALSE = false,
	XPATH_TRUE = true,
	XPATH_NOTFOUND
};

struct Error
{
	size_t pos;

	Error(size_t _pos = 0): pos(_pos) {}
};

struct XMLParserError: public Error
{
	XMLParserErrorType type;

	XMLParserError(XMLParserErrorType = PARSE_NO_ERR, size_t = 0);
};

struct XPathQueryError: public Error
{
	XPathQueryErrorType type;

	XPathQueryError(XPathQueryErrorType = XPATH_NO_ERR, size_t = 0);
};

class XMLElement;

struct XMLNode
{
    string_t name, value, ns;
    XMLElement* parent;

    XMLNode(const string_t&, const string_t&, const string_t&, XMLElement*);
};

XMLNode splitQualifiedName(const string_t&, size_t);

typedef std::map<string_t, XMLNode> XMLAttributeMap;
typedef std::list<XMLNode*> XMLNodeList;
typedef std::list<XMLElement*> XMLElementList;

class XMLElement: public XMLNode
{
//protected:
    XMLAttributeMap attributes;
    XMLElementList children;

    friend size_t parseAndAssignXML(const string_t&, XMLElement*);
    friend XMLElement* getPtrToParsedXML(const string_t&);
    friend XMLElement parseXML(const string_t&);
    void assignParsedXML(const string_t&);
public:
    XMLElement();
    XMLElement(const string_t&, bool);
    XMLElement(istream_t&);
    XMLElement(const string_t&, const string_t&, const string_t&, XMLElement* = NULL);
    XMLElement(const string_t&, const string_t&, const string_t&,
    		const XMLAttributeMap&, const std::list<XMLElement*>&,
    		XMLElement* = NULL);

    string_t getName() const;
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
    void addAttribute(const string_t&, const string_t&, const string_t&);
    bool removeAttribute(const string_t&);
    void addChild(size_t, const XMLElement&);
    void appendChild(const XMLElement&);
    void removeChild(size_t);

    XPathQueryErrorType queryAndAssignXPath(const string_t&, XMLNodeList&);
    XMLNodeList queryXPath(const string_t&);

    friend XPathBinaryOperatorResult evalXPathPredicateExprBinaryOperator(
			const string_t& expr, XMLElement* tree, const string_t& elem_name,
    		XPathOperator op, size_t offset);
    friend bool evalXPathBooleanExpr(const string_t&, XMLElement*,
    		const string_t&, size_t);
    friend ostream_t& operator<<(ostream_t&, const XMLElement&);
};

#endif /* PARSER_H_ */
