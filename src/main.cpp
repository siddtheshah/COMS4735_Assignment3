#include "basic_info.h"
#include "shape_descriptors.h"
#include "absolute_descriptors.h"
#include "relative_descriptors.h"
#include "map_interactive.h"
#include "utils.h"

#include <stdio.h>

using namespace cv;
using namespace std;

void show_population(map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		// cout << building_info_text_output(building.second);
		imshow("Building", building.second.mask);
		waitKey(0);
	}
}

int main() {
	string input_table = "new-table.txt";
	map<int, BuildingInfo> buildingMap = initialize_building_map(input_table);
	vector<Mat> reductions;
	get_masks("new-labeled.pgm", buildingMap);
	add_basic_characteristics(buildingMap);
	populate_shape_descriptors(buildingMap);
	populate_absolute_descriptors(buildingMap);
	ReductionMatrixHolder holder = populate_relative_descriptors(buildingMap);

	// show_population(buildingMap);
	map_interactive("new-campus.jpg", &buildingMap, holder);

	return 0;
}