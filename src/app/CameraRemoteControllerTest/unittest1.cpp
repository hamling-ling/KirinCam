#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CameraRemoteController/src/CameraStateManager.h"
#include "../CameraRemoteController/src/ErrorStatus.h"
#include <fstream>
#include <array>

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
		
		TEST_METHOD(JsonParserGetEvent)
		{
			ifstream ifs("getevent_1_0.json", ios::in);
			istreambuf_iterator<char> it(ifs);
			istreambuf_iterator<char> last;
			string strjson(it, last);

			CameraStateManager csm;
			array<bool, EVENT_IDX_MAX> updatedObjIndexes;
			for (int i = 0; i < updatedObjIndexes.size(); i++) {
				updatedObjIndexes[i] = false;
			}

			csm.UpdateState(strjson, updatedObjIndexes);
		}

		TEST_METHOD(JsonParserErrorStatus)
		{
			ptree pt;
			read_json("errorstatus.json", pt);

			ErrorStatus es;
			Assert::IsTrue(es.SetStatus(pt));
		}

		TEST_METHOD(JsonParserErrorStatusFailure)
		{
			ptree pt;
			read_json("getevent_1_0.json", pt);

			ErrorStatus es;
			Assert::IsFalse(es.SetStatus(pt));
		}

		TEST_METHOD(JsonParserRecStartStatusChanged)
		{
			string strJson1 = readJson("getevent_1_0.json");
			string strJson2 = readJson("getevent_1_0_recstart.json");

			CameraStateManager csm;
			array<bool, EVENT_IDX_MAX> updatedObjIndexes;
			for (int i = 0; i < updatedObjIndexes.size(); i++) {
				updatedObjIndexes[i] = false;
			}
			csm.UpdateState(strJson1, updatedObjIndexes);

			for (int i = 0; i < updatedObjIndexes.size(); i++) {
				updatedObjIndexes[i] = false;
			}
			csm.UpdateState(strJson2, updatedObjIndexes);
		}

	private:
		string readJson(const char* fileName)
		{
			ifstream ifs(fileName, ios::in);
			istreambuf_iterator<char> it(ifs);
			istreambuf_iterator<char> last;
			string strjson(it, last);

			return strjson;
		}
	};
}