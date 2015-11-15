// LiveViewStreamParser.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <boost/asio.hpp>

#include "LiveViewPacket.h"

using namespace std;
using namespace boost;


int _tmain(int argc, _TCHAR* argv[])
{
	asio::streambuf buf;
	ifstream ifs("out.dat", ios::in | ios::binary);
	if (!ifs) {
		std::cout << "err" << std::endl;
		return 1;
	}

	char tmp[64];
	ostream ofs(&buf);
	while (!ifs.eof()){
		size_t readsize = ifs.read(&tmp[0], sizeof(tmp)).gcount();
		cout << "readsize:" << readsize << endl;
		ofs.write(&tmp[0], readsize);
	}

	LiveViewPacket packet0;
	if (!packet0.Fill(buf)) {
		cout << "packet0 parse error" << endl;
		return 1;
	}

	{
		ofstream ofs("test.jpg", ios::binary | ios::trunc);
		const VariableSizeData* img = packet0.GetImage();
		cout << "image size:" << img->Size() << endl;
		ofs.write((const char*)img->Data(), img->Size());
	}

	LiveViewPacket packet1;
	if (!packet1.Fill(buf)) {
		cout << "packet1 parse error" << endl;
		return 1;
	}

	{
		ofstream ofs("test1.jpg", ios::binary | ios::trunc);
		const VariableSizeData* img = packet0.GetImage();
		cout << "image size:" << img->Size() << endl;
		ofs.write((const char*)img->Data(), img->Size());
	}

	return 0;
}

