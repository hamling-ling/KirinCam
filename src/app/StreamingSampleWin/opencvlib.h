
//プロジェクトのプロパティ⇒C/C++⇒全般　の追加のインクルードディレクトリに
// 『C:\OpenCV2.2\include』を追加のこと
#include <opencv2/opencv.hpp>

#ifdef _DEBUG        
#pragma comment(lib,"opencv_core300d.lib")
#pragma comment(lib,"opencv_imgproc300d.lib")
#pragma comment(lib,"opencv_highgui300d.lib")
#pragma comment(lib,"opencv_hal300d.lib")
#pragma comment(lib,"opencv_imgcodecs300d.lib")
#pragma comment(lib,"ippicvmt.lib")
#pragma comment(lib,"IlmImfd.lib")
#pragma comment(lib,"libjasperd.lib")
#pragma comment(lib,"libjpegd.lib")
#pragma comment(lib,"libpngd.lib")
#pragma comment(lib,"libtiffd.lib")
#pragma comment(lib,"libwebpd.lib")
#pragma comment(lib,"zlibd.lib")
#else    
#pragma comment(lib,"opencv_core300.lib")
#pragma comment(lib,"opencv_imgproc300.lib")
#pragma comment(lib,"opencv_highgui300.lib")
#pragma comment(lib,"opencv_hal300.lib")
#pragma comment(lib,"opencv_imgcodecs300.lib")
#pragma comment(lib,"ippicvmt.lib")
#pragma comment(lib,"IlmImf.lib")
#pragma comment(lib,"libjasper.lib")
#pragma comment(lib,"libjpeg.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"libtiff.lib")
#pragma comment(lib,"libwebp.lib")
#pragma comment(lib,"zlib.lib")  
#endif
