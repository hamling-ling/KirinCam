#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <chrono>
#include <conio.h>
#include <boost/regex.hpp>

#include "opencvlib.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "CameraRemoteController.h"
#include "LiveViewStreamParser.h"
#include "Common.h"
#include "ImageSourceApi.h"

using namespace std;
using boost::lexical_cast;

bool g_stop = false;

void playImages(std::shared_ptr<CameraController>& cp)
{
	cv::namedWindow("ImageWindow", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	vector<uint8_t> image;
	while (!g_stop) {
		cp->GetImage(image);
		if (image.empty()) {
			//somethimes empty. why ?
			continue;
		}

		cv::Mat dst_img = cv::imdecode(cv::Mat(image), 1);
		try {
			cv::imshow("ImageWindow", dst_img);
			cv::waitKey(10);
		}
		catch (cv::Exception &ex) {
			cout << ex.what() << endl;
			// 1st imgshow throws exception. don't know why ?
			//break;
		}
	}
	cv::destroyAllWindows();
}

int main()
{

	try
	{
		DeviceFinder finder;
		finder.Start(NULL);
		std::shared_ptr<CameraController> cp;

		std::string input;
		std::thread playThread;
		while (true) {
			cin >> input;

			if ("x" == input) {
				g_stop = true;
				if (playThread.joinable()) {
					playThread.join();
				}

				if (cp) {
					cp->Stop();
				}
				break;
			}
			else if ("g" == input) {
				g_stop = false;
				cp = make_shared<CameraController>(finder.GetDeviceDescription());
				cp->StartStreaming();
			}
			else if ("s" == input) {
				// if not started yet
				if (!playThread.joinable()) {
					// draw different thread. hope it's okey.
					playThread = std::thread(playImages, cp);
				}
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
