#pragma once

#include "building_info.h"

void add_symmetry_description(std::map<int, BuildingInfo>& buildingMap);
void add_orientation_description(std::map<int, BuildingInfo>& buildingMap);
void add_centrality_description(std::map<int, BuildingInfo>& buildingMap);
void add_directional_description(std::map<int, BuildingInfo>& buildingMap);

void populate_absolute_descriptors(std::map<int, BuildingInfo>& buildingMap);