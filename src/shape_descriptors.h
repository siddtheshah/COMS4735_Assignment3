#pragma once

#include "building_info.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <opencv2/opencv.hpp>

#pragma once

std::map<int, BuildingInfo> initialize_building_map(std::string& tableFileName);

void get_masks(std::string blobFileName, std::map<int, BuildingInfo>& buildingMap);
void add_basic_characteristics(std::map<int, BuildingInfo>& buildingMap);

void add_size_description(std::map<int, BuildingInfo>& buildingMap);
void add_thin_description(std::map<int, BuildingInfo>& buildingMap);
void add_bumpy_description(std::map<int, BuildingInfo>& buildingMap);
void add_roundness_description(std::map<int, BuildingInfo>& buildingMap);

void add_macro_descriptor(std::map<int, BuildingInfo>& buildingMap);
void evaluate_c_shape(std::map<int, BuildingInfo>& buildingMap);
void evaluate_l_shape(std::map<int, BuildingInfo>& buildingMap);
void evaluate_i_shape(std::map<int, BuildingInfo>& buildingMap);
void evaluate_macro_description(std::map<int, BuildingInfo>& buildingMap, cv::Mat template_img, std::string descriptor);

void shape_ingest(std::map<int, BuildingInfo>& buildingMap);
