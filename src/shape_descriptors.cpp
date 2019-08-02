#include "shape_descriptors.h"
#include <cmath>

using namespace cv;
using namespace std;

void add_size_description(map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
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
	for (auto& building : buildingMap) {
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
	for (auto& building : buildingMap) {
		int major_defect_count = 0;

		for (auto defect : building.second.defects) {
			if (defect[3] > 5) {
				major_defect_count += 1;
			}
		}
		if (major_defect_count > 3) {
			building.second.shape_descriptions.emplace_back("bumpy");
		}
		else {
			building.second.shape_descriptions.emplace_back("smooth");
		}
	}
}

void add_roundness_description(map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		if (building.second.contour.size() > 20 && building.second.corners.size() < 20) {
			building.second.shape_descriptions.emplace_back("rounded");
		}
		else {
			building.second.shape_descriptions.emplace_back("straight-edged");
		}
	}
}

void add_macro_descriptor(std::map<int, BuildingInfo>& buildingMap) {
	evaluate_c_shape(buildingMap);
	evaluate_l_shape(buildingMap);
	evaluate_i_shape(buildingMap);
	for (auto& building : buildingMap) {
		if (building.second.macro_score > .73) {
			building.second.shape_descriptions.emplace_back(building.second.macro_descriptor);
		}
	}
}

void evaluate_c_shape(std::map<int, BuildingInfo>& buildingMap) {
	Mat template_img = imread("Templates/template_c.png", CV_8UC1);
	threshold(template_img, template_img, 10, 255, THRESH_BINARY);
	evaluate_macro_description(buildingMap, template_img, "c-shaped");
}

void evaluate_l_shape(std::map<int, BuildingInfo>& buildingMap) {
	Mat template_img = imread("Templates/template_l.png", CV_8UC1);
	threshold(template_img, template_img, 10, 255, THRESH_BINARY);
	evaluate_macro_description(buildingMap, template_img, "l-shaped");
}

void evaluate_i_shape(std::map<int, BuildingInfo>& buildingMap) {
	Mat template_img = imread("Templates/template_i.png", CV_8UC1);
	threshold(template_img, template_img, 10, 255, THRESH_BINARY);
	evaluate_macro_description(buildingMap, template_img, "i-shaped");
}

void evaluate_macro_description(map<int, BuildingInfo>& buildingMap, Mat template_img, string descriptor) {
	// cout << descriptor << " ";
	for (auto& building : buildingMap) {
		Mat rotated = building.second.mask(building.second.bounding_box);
		Mat resized, agreed;
		double best = 0.0;
		Size size = template_img.size();
		for (int i = 0; i < 4; i++) {
			rotate(rotated, rotated, ROTATE_90_CLOCKWISE);
			resize(rotated, resized, size);
			agreed = (resized > 0) == (template_img > 0);
			// imshow("agree", 255*agreed);
			// waitKey(0);
			int count = countNonZero(agreed);
			if (count > best) best = count;
		}
		double frac = best / (size.width * size.height);
		// cout << building.second.name << ": " << frac << "\n";
		// imshow(building.second.name, building.second.mask);
		// waitKey(0);
		// destroyWindow(building.second.name);
		if (frac > building.second.macro_score) {
			building.second.macro_score = frac;
			building.second.macro_descriptor = descriptor;
		}
	}
	// cout << "\n";
}

void populate_shape_descriptors(std::map<int, BuildingInfo>& buildingMap) {
	add_size_description(buildingMap);
	add_thin_description(buildingMap);
	add_bumpy_description(buildingMap);
	add_roundness_description(buildingMap);
	add_macro_descriptor(buildingMap);
}