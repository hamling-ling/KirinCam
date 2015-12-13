#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CameraRemoteController/src/CameraStateManager.h"
#include "../CameraRemoteController/src/ErrorStatus.h"
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using boost::property_tree::ptree;

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

		TEST_METHOD(TestErrorStatusJesonParser)
		{
			ptree pt;
			read_json("errorstatus.json", pt);

			ErrorStatus es;
			Assert::IsTrue(es.SetStatus(pt));
		}

		TEST_METHOD(TestErrorStatusJesonParserFailure)
		{
			ptree pt;
			read_json("getevent_1_0.json", pt);

			ErrorStatus es;
			Assert::IsFalse(es.SetStatus(pt));
		}
	};
}