/*
 * main.cpp
 *
 *  Created on: 04.06.2014
 *      Author: radoslav
 */

#include <iostream>
#include <fstream>
#include "../headers/xmldom.h"
using namespace std;

int main()
{
	cout << "Fuck this shit!" << endl;
	wcout << "Fuck this shit!" << endl;
	std::ifstream f("/home/radoslav/projects/XMLParser/test.xml", std::ifstream::in);
	if (!f.is_open())
	{
		cout << "ERROR: Cannot open file!" << endl;
		return 1;
    }
	XMLElement elem(f);
	wcout << elem;
	wcout << "Hello world!" << endl;
	return 0;
}
