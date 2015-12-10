#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CameraRemoteController/src/CameraStateManager.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace CameraRemoteControllerTest
{		
	TEST_CLASS(JesonParseTest)
	{
	public:
		
		TEST_METHOD(TestJsonFileExist)
		{
			ifstream ifs("getevent_1_0.json", ios::in);
			if (!ifs) {
				Assert::Fail(L"failed to load getevent_1_0.jsont");
			}
		}

		TEST_METHOD(TestGetEventJesonParser)
		{
			ifstream ifs("getevent_1_0.json", ios::in);
			istreambuf_iterator<char> it(ifs);
			istreambuf_iterator<char> last;
			string strjson(it, last);

			CameraStateManager csm;
			set<string> updatedObjNames;
			csm.UpdateState(strjson, updatedObjNames);
		}
	};
}