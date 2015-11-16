#include "stdafx.h"
#include "CppUnitTest.h"
#include "LiveViewStreamParser.h"
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace boost;

namespace LiveViewStreamingParserTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestDataFileExist)
		{
			asio::streambuf buf;
			ifstream ifs("out.dat", ios::in | ios::binary);
			if (!ifs) {
				Assert::Fail(L"failed to load out.dat");
			}
			
		}

		TEST_METHOD(TestParsePacket) {

			asio::streambuf buf;
			ifstream ifs("out.dat", ios::in | ios::binary);

			LiveViewPacket packet0;
			Assert::IsTrue(packet0.Fill(buf));

			LiveViewPacket packet1;
			Assert::IsTrue(packet1.Fill(buf));
		}
	};
}