#include "absolute_descriptors.h"

#include <cmath>

using namespace std;
using namespace cv;

void add_symmetry_description(std::map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		Mat box = building.second.mask(building.second.bounding_box);
		Mat vertical, horizontal, agreed;
		flip(box, horizontal, 0);
		agreed = (box > 0) == (horizontal > 0);
		double frac = countNonZero(agreed) / (box.size().width * box.size().height);
		if (frac > .8) {
			building.second.absolute_space_descriptions.emplace_back("EW-symmetric");
		}
		flip(box, vertical, 1);
		agreed = (box > 0) == (vertical > 0);
		frac = countNonZero(agreed) / (box.size().width * box.size().height);
		if (frac > .8) {
			building.second.absolute_space_descriptions.emplace_back("NS-symmetric");
		}
	}
}

void add_orientation_description(std::map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		Mat box = building.second.mask(building.second.bounding_box);
		Moments m = moments(box, true);
		if (m.m10 / m.m01 > 1.5) {
			building.second.absolute_space_descriptions.emplace_back("NS-facing");
		}
		else if (m.m01 / m.m10 > 1.5) {
			building.second.absolute_space_descriptions.emplace_back("EW-facing");
		}
	}
}

void add_centrality_description(std::map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		Size campus_dims = building.second.mask.size();
		int up_margin = building.second.upper_left.y;
		int left_margin = building.second.upper_left.x;
		int right_margin = campus_dims.width - building.second.lower_right.x;
		int down_margin = campus_dims.height - building.second.lower_right.y;
		int centroid_displacement_x = abs(campus_dims.width / 2 - building.second.centroid.x);
		int centroid_displacement_y = abs(campus_dims.height / 2 - building.second.centroid.y);
		if (!(up_margin < 15 || left_margin < 15 || right_margin < 15 || down_margin < 15)
			|| (centroid_displacement_x < campus_dims.width / 3
			&& centroid_displacement_y < campus_dims.height / 3)) {
			building.second.absolute_space_descriptions.emplace_back("central");
		}
		else {
			building.second.absolute_space_descriptions.emplace_back("peripheral");
		}
	}
}

void add_directional_description(std::map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		Size campus_dims = building.second.mask.size();
		Point centroid = building.second.centroid;
		if (centroid.x < campus_dims.width / 3) {
			building.second.absolute_space_descriptions.emplace_back("western");
		}
		if (centroid.x > 2*campus_dims.width / 3) {
			building.second.absolute_space_descriptions.emplace_back("eastern");
		}
		if (centroid.y < campus_dims.height / 3) {
			building.second.absolute_space_descriptions.emplace_back("northern");
		}
		if (centroid.y > 2 * campus_dims.height / 3) {
			building.second.absolute_space_descriptions.emplace_back("southern");
		}
	}
}

void populate_absolute_descriptors(std::map<int, BuildingInfo>& buildingMap) {
	add_symmetry_description(buildingMap);
	add_orientation_description(buildingMap);
	add_centrality_description(buildingMap);
	add_directional_description(buildingMap);
}