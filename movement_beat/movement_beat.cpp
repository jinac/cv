#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <math.h>
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;

Point2f point;
bool addRemovePt = false;


int main(int argc, const char** argv) {
	VideoCapture cam;
	cam.open(argv[1]);

	/*
	cam.open("dwts_test.mp4");
	double fps = cam.get(CV_CAP_PROP_FPS);
	printf("Frame rate = %2f\n", fps);
	double diff_time = ((double) 1) / fps;
	printf("Time b/w frames = %2f\n", diff_time);
	*/

	const int MAX_COUNT = 500;
	TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 40, 0.01);
	Size subPixWinSize(10, 10), winSize(31, 31);

	if (!cam.isOpened()) {
		cout << "Could not initialize video capture...\n";
		return 0;
	}

	namedWindow("LK Tracking", 1);

	Mat gray, prevGray, image;
	vector<Point2f> points[2];

	Mat frame;
	bool read_success;

	read_success = cam.read(frame);

	int j = 0;
	while (j < 100) {
		read_success = cam.read(frame);
		imshow("LK Tracking", frame);
		j += 1;
	}

	frame.copyTo(image);
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("LK Tracking", gray);

	if (read_success && !frame.empty()) {
		imshow("LK Tracking", gray);
		goodFeaturesToTrack(gray, points[1], MAX_COUNT, 0.01, 10, Mat(), 3, 0, 0.04);
		cornerSubPix(gray, points[1], subPixWinSize, Size(-1, -1), termcrit);
	}

	//float diff_time;

	while (read_success && !frame.empty()) {
		frame.copyTo(image);
		cvtColor(image, gray, COLOR_BGR2GRAY);

		if (prevGray.empty()) {
			gray.copyTo(prevGray);
		}
		if (!points[0].empty()) {
			vector<uchar> status;
			vector<float> err;
			calcOpticalFlowPyrLK(prevGray, gray, points[0], points[1],
								 status, err, winSize, 3, termcrit,
								 0, 0.001);

			size_t i, k;
			for (i = k = 0; i < points[1].size(); i++) {
				if (!status[i]) {
					continue;
				}

				//points[1][k++] = points[1][i];
				circle(image, points[1][i], 3, Scalar(0, 255, 0), -1, 8);
				printf("%f,%f\n", points[1][i]-points[0][i]);
			}
			printf("\n");
			//points[1].resize(k);
		}


		imshow("LK Tracking", image);

		// exit check
		char c = (char) waitKey(10);
		if (c == 27)
			break;

		// read next frame for next cycle
		read_success = cam.read(frame);

		std::swap(points[1], points[0]);
		cv::swap(prevGray, gray);
	}

	return 0;
}
