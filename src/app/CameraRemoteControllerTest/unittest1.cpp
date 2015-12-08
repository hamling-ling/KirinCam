#include "stdafx.h"
#include "CppUnitTest.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace CameraRemoteControllerTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestJsonFileExist)
		{
			ifstream ifs("getevent_1_0.json", ios::in);
			if (!ifs) {
				Assert::Fail(L"failed to load getevent_1_0.jsont");
			}
		}

	};
}