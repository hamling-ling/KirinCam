#include <iostream>
#include <string>
#include <memory>
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
using namespace boost;
using boost::lexical_cast;

int main()
{
	try
	{
		DeviceFinder finder;
		finder.Start(NULL);
		std::shared_ptr<CameraController> cp;
		string input;

		while (true) {
			cin >> input;

			if ("x" == input) {
				break;
			}
			else if ("g" == input) {
				cp = make_shared<CameraController>(finder.GetDeviceDescription());
				cp->StartStreaming();
			}
			else if ("p" == input ) {
				if (cp) {
					cp->Stop();
				}
			}
			else if ("s" == input) {
				vector<uint8_t> image;
				cp->GetImage(image);
				if (!image.empty()) {
					Dump(image, "out.jpg");
					cv::Mat dst_img = cv::imdecode(cv::Mat(image), 1);

					cv::namedWindow("both flip image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
					cv::imshow("both flip image", dst_img);
				}
			}
			else if ("w" == input) {
				cv::Mat img = cv::imread("./out.jpg", 1);
				cv::namedWindow("both flip image", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
				cv::imshow("both flip image", img);
			}
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
