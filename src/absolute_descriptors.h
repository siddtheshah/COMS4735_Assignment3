#pragma once

#include "building_info.h"

#include <opencv2/opencv.hpp>

void add_symmetry_description(std::map<int, BuildingInfo>& buildingMap);
void add_orientation_description(std::map<int, BuildingInfo>& buildingMap);
void add_centrality_description(std::map<int, BuildingInfo>& buildingMap);
void add_directional_description(std::map<int, BuildingInfo>& buildingMap);

void absolute_ingest(std::map<int, BuildingInfo>& buildingMap);