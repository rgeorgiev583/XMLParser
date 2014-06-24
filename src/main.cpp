/*
 * main.cpp
 *
 *  Created on: 04.06.2014
 *      Author: radoslav
 */

#include <iostream>
#include <fstream>
#define NOTHROW
#include "../headers/parser.h"
//#include "../headers/word-tokenizer.h"
using namespace std;

int main()
{
	ifstream_t f("/home/radoslav/projects/XMLParser/test.xml", ifstream_t::in);
	if (!f.is_open())
	{
		cout << "ERROR: Cannot open file!" << endl;
		return 1;
	}
    XMLElement elem(f);
	//cout_t << elem;
	//cout_t << endl;
    //cout_t << endl;

	ifstream_t f1("/home/radoslav/projects/XMLParser/bookstore.xml", ifstream_t::in);
	if (!f1.is_open())
	{
		cout << "ERROR: Cannot open file!" << endl;
		return 1;
	}
	XMLElement elem1(f1);
	//cout_t << elem1;

	/*WordTokenizer wt("funny or die", "or");
	cout_t << wt.nextToken() << endl;
	cout_t << wt.getToken() << endl;
	cout_t << wt.nextToken() << endl;
	cout_t << wt.getToken() << endl;
	cout_t << wt.nextToken() << endl;*/
	return 0;
}
