#include <iostream>
#include <string>
#include <memory>
#include <boost/regex.hpp>
#include "opencvlib.h"
#include <opencv2/opencv.hpp>

#include "DeviceFinder.h"
#include "CameraController.h"

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
		}
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
