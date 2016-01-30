#pragma once

#include <opencv2/opencv.hpp>

#ifdef _DEBUG        
#pragma comment(lib,"opencv_core310d.lib")
#pragma comment(lib,"opencv_imgproc310d.lib")
#pragma comment(lib,"opencv_highgui310d.lib")
#pragma comment(lib,"opencv_imgcodecs310d.lib")
#else    
#pragma comment(lib,"opencv_core310.lib")
#pragma comment(lib,"opencv_imgproc310.lib")
#pragma comment(lib,"opencv_highgui310.lib")
#pragma comment(lib,"opencv_imgcodecs310.lib")
#endif
