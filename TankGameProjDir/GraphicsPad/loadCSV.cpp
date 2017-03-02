#pragma once
#include "loadCSV.h"

using namespace std;

loadCSV::loadCSV()
{
}

std::vector<int> loadCSV::load(const char* filename)
{
	vector<int> record;
	ifstream infile(filename);

	while (infile)
	{
		string s;
		if (!getline(infile, s)) break;

		istringstream ss(s);
		
		while (ss)
		{
			string s;
			if (!getline(ss, s, ',')) break;
			if (!(atoi(s.c_str()) == 0)){
				record.push_back(atoi(s.c_str()));
			}
		}

	}
	if (!infile.eof())
	{
		cerr << "Fooey!\n";
	}
	return record;
}


loadCSV::~loadCSV()
{
}
