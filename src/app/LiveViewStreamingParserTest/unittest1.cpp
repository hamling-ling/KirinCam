#include "stdafx.h"
#include "CppUnitTest.h"
#include "LiveViewStreamParser.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <boost/asio.hpp>

#define	OUT_DAT_PACKET0_SIZE	39944

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

		TEST_METHOD(TestParsePacketAtATime) {

			asio::streambuf buf;
			ifstream ifs("out.dat", ios::in | ios::binary);
			LiveViewPacket packet;

			readAll(ifs, buf);

			int bufSizeBefore = buf.size();
			Assert::IsTrue(packet.Fill(buf));
			Assert::IsTrue(packet.IsFull());
			size_t packetSize = packet.Size();
			Assert::IsTrue(packetSize == OUT_DAT_PACKET0_SIZE);
			int bufSizeAfter = buf.size();
			Assert::IsTrue(bufSizeBefore == bufSizeAfter + packet.Size());
		}

		TEST_METHOD(TestParsePacketSeveralTime) {

			asio::streambuf buf;
			ifstream ifs("out.dat", ios::in | ios::binary);
			LiveViewPacket packet;

			const int loopNum = OUT_DAT_PACKET0_SIZE / 1024 + 1;
			for (int i = 0; i < loopNum*2; i++) {

				readWithSize(ifs, 1024, buf);

				if (packet.IsFull()) {
					Assert::IsTrue(i == loopNum);
					break;
				}
				Assert::IsTrue(packet.Fill(buf));
			}

			Assert::IsTrue(packet.IsFull());
		}

		TEST_METHOD(TestParse2ndPacketsAtATime) {

			asio::streambuf buf;
			ifstream ifs("out.dat", ios::in | ios::binary);
			LiveViewPacket packet0, packet1;

			readAll(ifs, buf);
			packet0.Fill(buf);

			int bufSizeBefore = buf.size();
			Assert::IsTrue(packet1.Fill(buf));
			Assert::IsTrue(packet1.IsFull());
			size_t packetSize = packet1.Size();
			int bufSizeAfter = buf.size();
			Assert::IsTrue(bufSizeBefore == bufSizeAfter + packet1.Size());
		}

		TEST_METHOD(TestBigFile) {

			asio::streambuf buf;
			ifstream ifs("out16M.dat", ios::in | ios::binary);

			for (int i = 0; i < 10; i++) {
				LiveViewPacket packet;
				Assert::IsTrue(packet.Fill(buf));
			}
		}

	private:
		void readAll(ifstream& ifs, asio::streambuf& buf) {
			char tmp[1024];
			ostream ofs(&buf);
			while (!ifs.eof()){
				size_t readsize = ifs.read(&tmp[0], sizeof(tmp)).gcount();
				ofs.write(&tmp[0], readsize);
			}
		}

		void readWithSize(ifstream& ifs, size_t size, asio::streambuf& buf) {
			vector<char> tmp(size);
			ostream ofs(&buf);
			size_t readsize = ifs.read(&tmp[0], size).gcount();
			ofs.write(&tmp[0], readsize);
		}
	};
}