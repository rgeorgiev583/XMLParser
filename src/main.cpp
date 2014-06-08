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
	cout_t << "Fuck this shit!" << endl;
	ifstream_t f("/home/radoslav/projects/XMLParser/test.xml", ifstream_t::in);
	if (!f.is_open())
	{
		cout << "ERROR: Cannot open file!" << endl;
		return 1;
	}
    XMLElement elem(f);
	cout_t << elem;
	cout_t << "Hello world!" << endl;
	return 0;
}
