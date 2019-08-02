#include "utils.h"

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

std::string building_info_text_output(BuildingInfo& buildingInfo) {
	const char comma[] = { ',', ' ', '\0' };
	std::stringstream ss;
	ss << buildingInfo.name << "\n";
	ss << "Shape: " << join(buildingInfo.shape_descriptions, comma) << "\n";
	ss << "Absolute: " << join(buildingInfo.absolute_space_descriptions, comma) << "\n";
	ss << "Relative:\n";
	ss << "\t" << "North: " << join(buildingInfo.north_of, comma) << "\n";
	ss << "\t" << "South: " << join(buildingInfo.south_of, comma) << "\n";
	ss << "\t" << "East: " << join(buildingInfo.east_of, comma) << "\n";
	ss << "\t" << "West: " << join(buildingInfo.west_of, comma) << "\n";
	ss << "\t" << "Near: " << join(buildingInfo.near, comma) << "\n";
	ss << "\n";
	return ss.str();
}