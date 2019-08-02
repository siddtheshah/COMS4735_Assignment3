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


std::map<int, BuildingInfo> initialize_building_map(std::string& tableFileName) {
	std::string line;
	std::ifstream tableFile(tableFileName);
	std::map<int, BuildingInfo> name_map;
	if (tableFile.is_open())
	{
		int building_number = 0;
		while (std::getline(tableFile, line))
		{
			std::istringstream iss(line);
			iss >> std::ws;
			std::string building_int_as_str;
			BuildingInfo building;
			std::getline(iss, building_int_as_str, ' ');
			iss >> building.name;
			building.blob_index = atoi(building_int_as_str.c_str());
			name_map[building_number] = building;
			++building_number;
			// cout << building_int_as_str << ": " << building.name << "\n";
		}
		tableFile.close();
	}
	else {
		std::cout << "Table file could not be read!\n";
	}
	return name_map;
}
