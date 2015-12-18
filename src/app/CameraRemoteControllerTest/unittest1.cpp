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

			for (int i = 0; i < updatedObjIndexes.size(); i++) {
				bool expectation = false;
				switch (i) {
				case 0:
				case 1:
				case 3:
				case 10:
				case 12:
				case 13:
				case 16:
				case 17:
				case 21:
					expectation = true;
					break;
				default:
					break;
				}

				Assert::IsTrue(updatedObjIndexes[i]==expectation);
			}
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

			Assert::IsFalse(updatedObjIndexes[0]);
			Assert::IsTrue(updatedObjIndexes[1]);
			for (int i = 2; i < updatedObjIndexes.size(); i++) {
				Assert::IsFalse(updatedObjIndexes[i]);
			}
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