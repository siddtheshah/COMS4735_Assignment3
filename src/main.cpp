#include "basic_info.h"
#include "shape_descriptors.h"
#include "absolute_descriptors.h"
#include "relative_descriptors.h"
#include "utils.h"

#include <stdio.h>

using namespace cv;
using namespace std;

void show_population(map<int, BuildingInfo>& buildingMap) {
	const char comma[] = { ',', ' ', '\0' };
	for (auto& building : buildingMap) {
		cout << building.second.name << "\n";
		cout << "Shape: " << join(building.second.shape_descriptions, comma) << "\n";
		cout << "Absolute: " << join(building.second.absolute_space_descriptions, comma) << "\n";
		cout << "Relative:\n";
		cout << "\t" << "North: " << join(building.second.north_of, comma) << "\n";
		cout << "\t" << "South: " << join(building.second.south_of, comma) << "\n";
		cout << "\t" << "East: " << join(building.second.east_of, comma) << "\n";
		cout << "\t" << "West: " << join(building.second.west_of, comma) << "\n";
		cout << "\t" << "Near: " << join(building.second.near, comma) << "\n";
		cout << "\n";
		imshow("Building", building.second.mask);
		waitKey(0);
	}
}

int main() {
	string input_table = "new-table.txt";
	map<int, BuildingInfo> buildingMap = initialize_building_map(input_table);
	get_masks("new-labeled.pgm", buildingMap);
	add_basic_characteristics(buildingMap);
	populate_shape_descriptors(buildingMap);
	populate_absolute_descriptors(buildingMap);
	populate_relative_descriptors(buildingMap);

	show_population(buildingMap);

	return 0;
}