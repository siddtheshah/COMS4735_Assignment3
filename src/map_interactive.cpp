#include "relative_descriptors.h"
#include "map_interactive.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <deque>

using namespace std;
using namespace cv;

bool MapInteractive::addable_neighbor(Point p, Vec3b color) {
	auto base = shown_.at<Vec3b>(p);
	return base != color && base != restrict_color_ && is_valid_point(p);
}

bool MapInteractive::is_valid_point(cv::Point p) {
	return p.x >= 0 && p.x < shown_.cols && p.y >= 0 && p.y < shown_.rows;
}

int MapInteractive::add_point_cloud(const BuildingInfo& source, Vec3b color) {
	Point initial = source.centroid;
	std::deque<Point> explore_queue;
	explore_queue.emplace_back(initial);
	int size = 0;

	while (!explore_queue.empty()) {
		Point p = explore_queue.front();
		explore_queue.pop_front();
		if (!addable_neighbor(p, color)) {
			continue;  // If we can't color it anyway, don't do the expensive computation.
		}
		BuildingInfo b_info = create_fake_building(p); // Most expensive step.
		if (b_info.north_of == source.north_of
			&& b_info.south_of == source.south_of
			&& b_info.east_of == source.east_of
			&& b_info.west_of == source.west_of
			&& b_info.near == source.near) {

			shown_.at<Vec3b>(p) = color;
			Point p1(p.x - 1, p.y);
			Point p2(p.x + 1, p.y);
			Point p3(p.x, p.y - 1);
			Point p4(p.x, p.y + 1);
			if (addable_neighbor(p1, color))
				explore_queue.emplace_back(p1);
			if (addable_neighbor(p2, color))
				explore_queue.emplace_back(p2);
			if (addable_neighbor(p3, color))
				explore_queue.emplace_back(p3);
			if (addable_neighbor(p4, color))
				explore_queue.emplace_back(p4);
			size++;
		}
		else {
			shown_.at<Vec3b>(p) = restrict_color_;
		}
	}
	return size;
}

BuildingInfo MapInteractive::create_fake_building(Point point_source) {
	BuildingInfo fake;
	fake.upper_left = point_source;
	fake.centroid = point_source;
	fake.lower_right = point_source;
	fake.bounding_box = Rect(point_source.x, point_source.y, 1, 1);

	vector<bool> north_vec(buildings_->size(), true);
	for (int i = 0; i < buildings_->size(); i++) {
		if (north_vec[i]) {
			if (north_of(fake, (*buildings_)[i])) {
				for (int j = 0; j < buildings_->size(); j++) {
					if (reductions_.north.at<uchar>(i, j)) {
						north_vec[j] = false;
					}
				}
			}
			else {
				north_vec[i] = false;
			}
		}
	}
	for (int i = 0; i < buildings_->size(); i++) {
		if (north_vec[i]) fake.north_of.emplace_back((*buildings_)[i].name);
	}

	vector<bool> south_vec(buildings_->size(), true);
	for (int i = 0; i < buildings_->size(); i++) {
		if (south_vec[i]) {
			if (south_of(fake, (*buildings_)[i])) {
				for (int j = 0; j < buildings_->size(); j++) {
					if (reductions_.south.at<uchar>(i, j)) {
						south_vec[j] = false;
					}
				}
			}
			else {
				south_vec[i] = false;
			}
		}
	}
	for (int i = 0; i < buildings_->size(); i++) {
		if (south_vec[i]) fake.south_of.emplace_back((*buildings_)[i].name);
	}

	vector<bool> west_vec(buildings_->size(), true);
	for (int i = 0; i < buildings_->size(); i++) {
		if (west_vec[i]) {
			if (west_of(fake, (*buildings_)[i])) {
				for (int j = 0; j < buildings_->size(); j++) {
					if (reductions_.west.at<uchar>(i, j)) {
						west_vec[j] = false;
					}
				}
			}
			else {
				west_vec[i] = false;
			}
		}
	}
	for (int i = 0; i < buildings_->size(); i++) {
		if (west_vec[i]) fake.west_of.emplace_back((*buildings_)[i].name);
	}

	vector<bool> east_vec(buildings_->size(), true);
	for (int i = 0; i < buildings_->size(); i++) {
		if (east_vec[i]) {
			if (east_of(fake, (*buildings_)[i])) {
				for (int j = 0; j < buildings_->size(); j++) {
					if (reductions_.east.at<uchar>(i, j)) {
						east_vec[j] = false;
					}
				}
			}
			else {
				east_vec[i] = false;
			}
		}
	}
	for (int i = 0; i < buildings_->size(); i++) {
		if (east_vec[i]) fake.east_of.emplace_back((*buildings_)[i].name);
	}

	for (int i = 0; i < buildings_->size(); i++) {
		if (near(fake, (*buildings_)[i])) {
			fake.near.emplace_back((*buildings_)[i].name);
		}
	}

	return fake;
}


void handle_event(int event, int x, int y, int _, void* map_interactive)
{
	MapInteractive* mp = static_cast<MapInteractive*>(map_interactive);
	if (event == EVENT_LBUTTONDOWN && mp->state_ == CLEAN) {
		mp->s_ = mp->create_fake_building(Point(x, y));
		int size = mp->add_point_cloud(mp->s_, Vec3b(0, 255, 0));
		cout << building_info_descriptor_output(mp->s_);
		cout << "S Cloud Size: " << size << "\n";
		cout << "Origin: (" << x << "," << y << ")\n";
		mp->state_ = FIRST_CLICK;
	}
	else if (event == EVENT_LBUTTONDOWN && mp->state_ == FIRST_CLICK) {
		mp->t_ = mp->create_fake_building(Point(x, y));
		int size = mp->add_point_cloud(mp->t_, Vec3b(0, 0, 255));
		cout << building_info_descriptor_output(mp->t_);
		cout << "T Cloud Size: " << size << "\n";
		cout << "Origin: (" << x << "," << y << ")\n";
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
	Mat map = imread(mapFileName, CV_32F);
	threshold(map, map, 10, 255, THRESH_BINARY_INV);

	MapInteractive interactive(map, buildings, reductions);
	waitKey(0);
}
