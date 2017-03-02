#pragma once
#include <vector>
class textbox
{
public:
	size_t len;
	std::vector<int> shapeIdx;
	std::vector<float> cursor;
	textbox(const char* str);
	~textbox();
};

