#pragma once
#include "building_info.h"

#include <iostream>
#include <fstream>
#include <sstream>

void get_masks(std::string blobFileName, std::map<int, BuildingInfo>& buildingMap);
void add_basic_characteristics(std::map<int, BuildingInfo>& buildingMap);