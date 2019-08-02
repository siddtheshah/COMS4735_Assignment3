#include "basic_info.h"
#include "shape_descriptors.h"
#include "absolute_descriptors.h"
#include "relative_descriptors.h"
#include "map_interactive.h"
#include "utils.h"

#include <stdio.h>
#include <fstream>

using namespace cv;
using namespace std;

void show_population(map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		cout << building_info_descriptor_output(building.second);
		imshow("Building", building.second.mask);
		waitKey(0);
	}
}

void record_population(map<int, BuildingInfo>& buildingMap) {
	std::ofstream basic_info_file("basic_info.txt");
	std::ofstream descriptor_file("descriptors.txt");
	for (auto& building : buildingMap) {
		basic_info_file << building_info_basic_output(building.second);
		descriptor_file << building_info_descriptor_output(building.second);
	}
}

int main() {
	string input_table = "new-table.txt";
	map<int, BuildingInfo> buildingMap = initialize_building_map(input_table);
	vector<Mat> reductions;
	get_masks("new-labeled.pgm", buildingMap);
	add_basic_characteristics(buildingMap);
	populate_shape_descriptor_(buildingMap);
	populate_absolute_descriptors(buildingMap);
	ReductionMatrixHolder holder = populate_relative_descriptors(buildingMap);

	// record_population(buildingMap);
	// show_population(buildingMap);
	cout << "Entering Interactive Mode\n";
	map_interactive("new-campus.jpg", &buildingMap, holder);

	return 0;
}