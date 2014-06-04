/*
 * name-validation.h
 *
 *  Created on: 03.06.2014
 *      Author: radoslav
 */

#ifndef NAME_VALIDATION_H_
#define NAME_VALIDATION_H_

#include <string>

bool isBaseChar(char);
bool isBaseChar(wchar_t);
bool isIdeographic(wchar_t);
bool isCombiningChar(wchar_t);
bool isDigit(char);
bool isDigit(wchar_t);
bool isExtender(wchar_t);

bool isLetter(char);
bool isLetter(wchar_t);

bool isNameChar(char);
bool isNameChar(wchar_t);

bool isValidName(std::string);
bool isValidName(std::wstring);

std::string getDelimiterChars();
std::wstring getDelimiterWchars();

#endif /* NAME_VALIDATION_H_ */
