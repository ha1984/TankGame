#pragma once
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class loadCSV
{
public:
	std::vector<int> load(const char* filename);
	loadCSV();
	~loadCSV();
};

