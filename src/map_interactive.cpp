#include "map_interactive.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace std;
using namespace cv;

void MapInteractive::add_point_cloud(const BuildingInfo& source, Scalar color) {
	circle(shown_, source.centroid, 20, color, 3);


}

BuildingInfo MapInteractive::create_fake_building(Point point_source) {
	BuildingInfo fake;
	fake.upper_left = point_source;
	fake.centroid = point_source;
	fake.lower_right = point_source;
	fake.bounding_box = Rect(point_source.x, point_source.y, 1, 1);


	return fake;
}


void handle_event(int event, int x, int y, int _, void* map_interactive)
{
	MapInteractive* mp = static_cast<MapInteractive*>(map_interactive);
	if (event == EVENT_LBUTTONDOWN && mp->state_ == CLEAN) {
		mp->s_ = mp->create_fake_building(Point(x, y));
		mp->add_point_cloud(mp->s_, Scalar(0, 255, 0));
		cout << building_info_text_output(mp->s_);
		mp->state_ = FIRST_CLICK;
	}
	else if (event == EVENT_LBUTTONDOWN && mp->state_ == FIRST_CLICK) {
		mp->t_ = mp->create_fake_building(Point(x, y));
		mp->add_point_cloud(mp->t_, Scalar(0, 0, 255));
		cout << building_info_text_output(mp->s_);
		mp->state_ = SECOND_CLICK;
	} 
	else if (event == EVENT_RBUTTONDOWN || 
		(event == EVENT_LBUTTONDOWN && mp->state_ == SECOND_CLICK)) {
		mp->clear();
		mp->state_ = CLEAN;
	}
	mp->update();
}

void map_interactive(string mapFileName, std::map<int, BuildingInfo>* buildings, ReductionMatrixHolder reductions) {
	Mat map = imread(mapFileName, CV_16F);
	threshold(map, map, 10, 255, THRESH_BINARY_INV);

	MapInteractive interactive(map, buildings, reductions);
	/*namedWindow(kInteractive, 1);
	setMouseCallback(kInteractive, handle_event, &interactive);
	imshow(kInteractive, interactive.get_shown());*/
	waitKey(0);
}
