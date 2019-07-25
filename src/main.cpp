#include "shape_ingest.h"

#include <stdio.h>

using namespace cv;
using namespace std;

int main() {
	map<int, BuildingInfo> buildingMap = initialize_building_map("new-table.txt");
	get_masks("new-labeled.pgm", buildingMap);
	add_basic_characteristics(buildingMap);

	return 0;
}