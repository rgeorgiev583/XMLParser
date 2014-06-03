#include <string>
#pragma once

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

bool isValidName(string);
bool isValidName(wstring);
