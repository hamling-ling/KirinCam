
#include <opencv2/opencv.hpp>

#ifdef _DEBUG        
#pragma comment(lib,"opencv_core310d.lib")
#pragma comment(lib,"opencv_imgproc310d.lib")
#pragma comment(lib,"opencv_highgui310d.lib")
//#pragma comment(lib,"opencv_hal310d.lib")
#pragma comment(lib,"opencv_imgcodecs310d.lib")
//#pragma comment(lib,"ippicvmt.lib")
//#pragma comment(lib,"IlmImfd.lib")
//#pragma comment(lib,"libjasperd.lib")
//#pragma comment(lib,"libjpegd.lib")
//#pragma comment(lib,"libpngd.lib")
//#pragma comment(lib,"libtiffd.lib")
//#pragma comment(lib,"libwebpd.lib")
//#pragma comment(lib,"zlibd.lib")
#else    
#pragma comment(lib,"opencv_core310.lib")
#pragma comment(lib,"opencv_imgproc310.lib")
#pragma comment(lib,"opencv_highgui310.lib")
#pragma comment(lib,"opencv_hal310.lib")
#pragma comment(lib,"opencv_imgcodecs310.lib")
#pragma comment(lib,"ippicvmt.lib")
#pragma comment(lib,"IlmImf.lib")
#pragma comment(lib,"libjasper.lib")
#pragma comment(lib,"libjpeg.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"libtiff.lib")
#pragma comment(lib,"libwebp.lib")
#pragma comment(lib,"zlib.lib")  
#endif
