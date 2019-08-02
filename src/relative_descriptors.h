#pragma once
#include "building_info.h"

bool near(const BuildingInfo& b1, const BuildingInfo& b2);
bool north_of(const BuildingInfo& b1, const BuildingInfo& b2);
bool south_of(const BuildingInfo& b1, const BuildingInfo& b2);
bool east_of(const BuildingInfo& b1, const BuildingInfo& b2);
bool west_of(const BuildingInfo& b1, const BuildingInfo& b2);

void populate_relative_descriptors(std::map<int, BuildingInfo>& buildingMap);