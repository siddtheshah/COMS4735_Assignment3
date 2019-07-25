#include "shape_ingest.h"
#include <cmath>

using namespace cv;
using namespace std;

map<int, BuildingInfo> initialize_building_map(string tableFileName) {
	string line;
	ifstream tableFile(tableFileName);
	map<int, BuildingInfo> name_map;
	if (tableFile.is_open())
	{
		while (getline(tableFile, line))
		{
			std::istringstream iss(line);
			iss >> std::ws;
			string building_int_as_str;
			BuildingInfo building;
			std::getline(iss, building_int_as_str, ' ');
			iss >> building.name;
			name_map[atoi(building_int_as_str.c_str())] = building;

			cout << building_int_as_str << ": " << building.name << "\n";
		}
		tableFile.close();
	}
	return name_map;
}

void get_masks(string blobFileName, map<int, BuildingInfo>& buildingMap) {
	Mat allBuildingsBlob = imread(blobFileName, CV_8UC1);
	for (auto building : buildingMap) {
		Mat binary, mask;
		binary = 255*(allBuildingsBlob == building.first);
		binary.convertTo(mask, CV_8UC1);
		buildingMap[building.first].mask = mask;
	}
}

void add_basic_characteristics(map<int, BuildingInfo>& buildingMap) {
	for (auto building : buildingMap) {
		vector<vector<Point>> contours;
		Mat edges;
		vector<Vec4i> hierarchy;
		Canny(building.second.mask, edges, 127, 255, 3);
		findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		building.second.contour = contours[0];
		Rect r = boundingRect(building.second.mask);
		auto m = moments(building.second.mask);

		building.second.pixel_area = contourArea(building.second.contour);
		building.second.upper_left = Point(r.x, r.y);
		building.second.lower_right = Point(r.x + r.width, r.y + r.height);
		building.second.centroid = Point(m.m10 / m.m00, m.m01 / m.m00);
		
		/*
		drawContours(edges, contours, 0, Scalar(255, 0, 0));
		imshow("Contours", edges);
		waitKey(0);
		*/

		std::cout << building.second.pixel_area << "\n";
	}
}

void add_size_description(map<int, BuildingInfo>& buildingMap) {
	for (auto building : buildingMap) {
		if (building.second.pixel_area > 2000) {
			building.second.shape_descriptions.emplace_back("large");
		}
		else if (building.second.pixel_area > 1000) {
			building.second.shape_descriptions.emplace_back("mid-size");
		}
		else {
			building.second.shape_descriptions.emplace_back("small");
		}
	}
}

void add_thin_description(map<int, BuildingInfo>& buildingMap) {
	for (auto building : buildingMap) {
		double width = abs(building.second.lower_right.x - building.second.upper_left.x);
		double height = abs(building.second.lower_right.y - building.second.upper_left.y);
		double ratio = width / height;
		if (ratio > 1) {
			ratio = 1 / ratio;
		}
		if (ratio < .33) {
			building.second.shape_descriptions.emplace_back("long");
		}
		else {
			building.second.shape_descriptions.emplace_back("evenly-proportioned");
		}
	}
}

void add_bumpy_description(map<int, BuildingInfo>& buildingMap) {
	for (auto building : buildingMap) {
		Mat extra;
		vector<Point> hull;
		convexHull(building.second.contour, hull);

	}
}

void add_symmetry_description(map<int, BuildingInfo>& buildingMap) {

}

void add_macro_shape_description(map<int, BuildingInfo>& buildingMap) {

}

void add_roundness_description(map<int, BuildingInfo>& buildingMap) {

}