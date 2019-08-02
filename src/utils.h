#pragma once
#include "building_info.h"

#include <vector>
#include <string>
#include <iterator>
#include <fstream>
#include <ostream>
#include <sstream>

std::string join(const std::vector<std::string>& vec, const char* delim);
std::map<int, BuildingInfo> initialize_building_map(std::string& tableFileName);

std::string building_info_basic_output(BuildingInfo& buildingInfo);
std::string building_info_descriptor_output(BuildingInfo& buildingInfo);