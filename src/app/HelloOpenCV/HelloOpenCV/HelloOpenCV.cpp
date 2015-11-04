// HelloOpenCV.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencvlib.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *window_input = "input";
	cv::namedWindow(window_input, CV_WINDOW_AUTOSIZE);

	char *window_output = "output";
	cv::namedWindow(window_output, CV_WINDOW_AUTOSIZE);

	cv::VideoCapture cap("http://192.168.1.7/snapshot.cgi");
	cap.open(0);
	if (!cap.isOpened()) {
		std::cerr << "cannot find camera" << std::endl;
		return -1;
	}

	while (1) {
		cv::Mat frame;
		cap >> frame;
		cv::Mat gray;
		cv::cvtColor(frame, gray, CV_BGR2GRAY);

		cv::imshow(window_input, frame);
		cv::imshow(window_output, gray);

		int key = cv::waitKey(10);
		if (key == 3 || key == 27 || key == 'q') {
			break;
		}
	}
}

