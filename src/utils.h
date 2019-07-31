#pragma once
#include "building_info.h"

#include <vector>
#include <string>
#include <iterator>
#include <ostream>
#include <sstream>

std::string join(const std::vector<std::string>& vec, const char* delim)
{
	std::stringstream res;
	copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim));
	return res.str();
}


