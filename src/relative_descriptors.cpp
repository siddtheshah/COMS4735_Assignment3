#include "relative_descriptors.h"
#include <cmath>
#include <vector>

using namespace std;
using namespace cv;


double separation(const BuildingInfo& b1, const BuildingInfo& b2) {
	double vertical_sep = abs(b2.centroid.y - b1.centroid.y) - .5 * (b1.bounding_box.height + b2.bounding_box.height);
	double horizontal_sep = abs(b2.centroid.x - b1.centroid.x) - .5 * (b1.bounding_box.width + b2.bounding_box.width);
	return vertical_sep > horizontal_sep ? vertical_sep : horizontal_sep;
}

bool near(const BuildingInfo& b1, const BuildingInfo& b2) {
	return (.75*(b1.bounding_box.width + b2.bounding_box.width) > abs(b2.centroid.x - b1.centroid.x)) &&
		(.75 * (b1.bounding_box.height + b2.bounding_box.height) > abs(b2.centroid.y - b1.centroid.y));
}

// If true, b1 is north of b2.
bool north_of(const BuildingInfo& b1, const BuildingInfo& b2) {
	//imshow("Debug", .5*b1.mask + b2.mask);
	//cout << "b1: " << b1.lower_right.y << "\n";
	//cout << "b2: " << b2.upper_left.y << "\n";
	//waitKey(0);
	//destroyWindow("Debug");
	return (b1.centroid.y < b2.centroid.y && abs(b2.centroid.y - b1.centroid.y) > abs(b2.centroid.x - b1.centroid.x))
	 || b1.lower_right.y < b2.upper_left.y;
}

bool south_of(const BuildingInfo& b1, const BuildingInfo& b2) {
	return (b1.centroid.y > b2.centroid.y && abs(b2.centroid.y - b1.centroid.y) > abs(b2.centroid.x - b1.centroid.x))
	|| b1.upper_left.y > b2.lower_right.y;
}

bool east_of(const BuildingInfo& b1, const BuildingInfo& b2) {
	return (b1.centroid.x > b2.centroid.x && abs(b2.centroid.x - b1.centroid.x) > abs(b2.centroid.y - b1.centroid.y))
	|| b1.upper_left.x > b2.lower_right.x;
}
bool west_of(const BuildingInfo& b1, const BuildingInfo& b2) {
	return (b1.centroid.x < b2.centroid.x && abs(b2.centroid.x - b1.centroid.x) > abs(b2.centroid.y - b1.centroid.y))
	|| b1.lower_right.x < b2.upper_left.x;
}


Mat create_near_matrix(std::map<int, BuildingInfo>& buildingMap) {
	Mat ret(Size(buildingMap.size(), buildingMap.size()), CV_8UC1);
	for (const auto& building1 : buildingMap) {
		for (const auto& building2 : buildingMap) {
			ret.at<uchar>(building1.first, building2.first) = near(building1.second, building2.second);
		}
	}
	return ret;
}

Mat create_north_matrix(std::map<int, BuildingInfo>& buildingMap) {
	Mat ret(Size(buildingMap.size(), buildingMap.size()), CV_8UC1);
	for (const auto& building1 : buildingMap) {
		for (const auto& building2 : buildingMap) {
			ret.at<uchar>(building1.first, building2.first) = north_of(building1.second, building2.second);
		}
	}
	return ret;
}

Mat create_south_matrix(std::map<int, BuildingInfo>& buildingMap) {
	Mat ret(Size(buildingMap.size(), buildingMap.size()), CV_8UC1);
	for (const auto& building1 : buildingMap) {
		for (const auto& building2 : buildingMap) {
			ret.at<uchar>(building1.first, building2.first) = south_of(building1.second, building2.second);
		}
	}
	return ret;
}

Mat create_east_matrix(std::map<int, BuildingInfo>& buildingMap) {
	Mat ret(Size(buildingMap.size(), buildingMap.size()), CV_8UC1);
	for (const auto& building1 : buildingMap) {
		for (const auto& building2 : buildingMap) {
			ret.at<uchar>(building1.first, building2.first) = east_of(building1.second, building2.second);
		}
	}
	return ret;
}

Mat create_west_matrix(std::map<int, BuildingInfo>& buildingMap) {
	Mat ret(Size(buildingMap.size(), buildingMap.size()), CV_8UC1);
	for (const auto& building1 : buildingMap) {
		for (const auto& building2 : buildingMap) {
			ret.at<uchar>(building1.first, building2.first) = west_of(building1.second, building2.second);
		}
	}
	return ret;
}

void cubic_transitive_reduction_directed(Mat m) {
	// reflexive reduction
	for (int i = 0; i < m.rows; ++i)
		m.at<uchar>(i, i) = 0;

	// transitive reduction
	for (int i = 0; i < m.rows; ++i)
		for (int j = 0; j < m.rows; ++j)
			if (m.at<uchar>(i, j))
				for (int k = 0; k < m.rows; ++k)
					if (m.at<uchar>(j, k))
						m.at<uchar>(i, k) = false;
}

void populate_relative_descriptors(std::map<int, BuildingInfo>& buildingMap) {
	Mat north_matrix = create_north_matrix(buildingMap);
	cubic_transitive_reduction_directed(north_matrix);
	for (int i = 0; i < north_matrix.rows; ++i)
		for (int j = 0; j < north_matrix.cols; ++j)
			if (north_matrix.at<uchar>(i, j)) buildingMap[i].north_of.emplace_back(buildingMap[j].name);

	Mat south_matrix = create_south_matrix(buildingMap);
	cubic_transitive_reduction_directed(south_matrix);
	for (int i = 0; i < south_matrix.rows; ++i)
		for (int j = 0; j < south_matrix.cols; ++j)
			if (south_matrix.at<uchar>(i, j)) buildingMap[i].south_of.emplace_back(buildingMap[j].name);

	Mat east_matrix = create_east_matrix(buildingMap);
	cubic_transitive_reduction_directed(east_matrix);
	for (int i = 0; i < east_matrix.rows; ++i)
		for (int j = 0; j < east_matrix.cols; ++j)
			if (east_matrix.at<uchar>(i, j)) buildingMap[i].east_of.emplace_back(buildingMap[j].name);

	Mat west_matrix = create_west_matrix(buildingMap);
	cubic_transitive_reduction_directed(west_matrix);
	for (int i = 0; i < west_matrix.rows; ++i)
		for (int j = 0; j < west_matrix.cols; ++j)
			if (west_matrix.at<uchar>(i, j)) buildingMap[i].west_of.emplace_back(buildingMap[j].name);
	
	// Filtering near based on building size. This allows length 2 cycles to remain.
	// Of the b2s that are near, keep the biggest one. 
	Mat near_matrix = create_near_matrix(buildingMap);
	for (int i = 0; i < near_matrix.rows; ++i)
		near_matrix.at<uchar>(i, i) = 0;
	//for (int i = 0; i < near_matrix.rows; ++i) {
	//	double largest = -1;
	//	int largest_ind = -1;
	//	for (int j = 0; j < near_matrix.cols; ++j) {
	//		if (near_matrix.at<uchar>(i, j) && buildingMap[i].pixel_area > largest) {
	//			largest = buildingMap[j].pixel_area;
	//			largest_ind = j;
	//		}
	//	}
	//	if (largest_ind >= 0) {
	//		buildingMap[i].near.emplace_back(buildingMap[largest_ind].name);
	//	}
	//}
	for (int i = 0; i < near_matrix.rows; ++i) {
		for (int j = 0; j < near_matrix.cols; ++j) {
			if (near_matrix.at<uchar>(i, j))
				buildingMap[i].near.emplace_back(buildingMap[j].name);
		}
	}

}

