#pragma once

#include "building_info.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>

std::map<int, BuildingInfo> initialize_building_map(std::string tableFileName);

void get_masks(std::string blobFileName, std::map<int, BuildingInfo>& buildingMap);
void add_basic_characteristics(std::map<int, BuildingInfo>& buildingMap);
void add_size_description(std::map<int, BuildingInfo>& buildingMap);
void add_thin_description(std::map<int, BuildingInfo>& buildingMap);
void add_bumpy_description(std::map<int, BuildingInfo>& buildingMap);
void add_symmetry_description(std::map<int, BuildingInfo>& buildingMap);
