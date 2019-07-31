#include "shape_descriptors.h"
#include "absolute_descriptors.h"
#include "utils.h"

#include <stdio.h>

using namespace cv;
using namespace std;

int main() {
	string input_table = "new-table.txt";
	map<int, BuildingInfo> buildingMap = initialize_building_map(input_table);
	get_masks("new-labeled.pgm", buildingMap);
	add_basic_characteristics(buildingMap);
	shape_ingest(buildingMap);
	absolute_ingest(buildingMap);

	const char delim[] = {',', ' ', '\0'};
	for (auto building : buildingMap) {
		cout << building.second.name << ": " << join(building.second.shape_descriptions, delim) << "\n";
		cout << building.second.name << ": " << join(building.second.absolute_space_descriptions, delim) << "\n";
		// imshow(building.second.name, building.second.mask);
		// waitKey(0);
	}

	return 0;
}