#include "basic_info.h"

using namespace cv;
using namespace std;

void get_masks(string blobFileName, map<int, BuildingInfo>& buildingMap) {
	Mat allBuildingsBlob = imread(blobFileName, CV_8UC1);
	for (auto& building : buildingMap) {
		Mat binary, mask;
		binary = 255 * (allBuildingsBlob == building.second.blob_index);
		binary.convertTo(mask, CV_8UC1);
		buildingMap[building.first].mask = mask;
	}
}

void add_basic_characteristics(map<int, BuildingInfo>& buildingMap) {
	for (auto& building : buildingMap) {
		vector<vector<Point>> contours;
		Mat edges;
		vector<Vec4i> hierarchy;
		Canny(building.second.mask, edges, 127, 255, 3);
		findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		building.second.contour = contours[0];
		Rect r = boundingRect(building.second.mask);
		building.second.bounding_box = r;
		auto m = moments(building.second.mask);

		building.second.pixel_area = contourArea(building.second.contour);
		building.second.upper_left = Point(r.x, r.y);
		building.second.lower_right = Point(r.x + r.width, r.y + r.height);
		building.second.centroid = Point(m.m10 / m.m00, m.m01 / m.m00);


		//drawContours(edges, contours, 0, Scalar(255, 0, 255), 3);
		//imshow("Contours", edges);
		//waitKey(0);
		//

		convexHull(contours[0], building.second.hull);
		convexityDefects(building.second.contour, building.second.hull, building.second.defects);

		Mat gray = building.second.mask;
		Mat corner_image, components, last, stat, centroids;
		cornerHarris(gray, corner_image, 2, 3, 0.04);
		dilate(corner_image, corner_image, Mat());
		threshold(corner_image, corner_image, 2, 255, 0);
		corner_image.convertTo(components, CV_8U);
		connectedComponentsWithStats(components, last, stat, centroids);

		// std::cout << building.second.pixel_area << "\n";
	}
}