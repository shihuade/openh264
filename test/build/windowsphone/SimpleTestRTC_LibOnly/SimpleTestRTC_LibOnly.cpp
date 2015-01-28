// SimpleTestRTC_LibOnly.cpp



#include <windows.h>
#include <iostream>
#include "SimpleTestRTC_LibOnly.h"

using namespace Platform;
using namespace Windows;
using namespace Windows::Storage;
using namespace SimpleCaseTestRTC;


extern int CodecUtMain(int argc, char** argv);

CodecSimpleCasesRTC::CodecSimpleCasesRTC()
{
}

int CodecSimpleCasesRTC::TestAllCasesRTC()
{
	int   argc = 1;
	int   iRet = 0;
	char *argv[6];

	// output xml file location
	char OutputPath[256] = { 0 };
	Windows::Storage::StorageFolder^ OutputLocation;
	Platform::String^ OutputLocationPath;

	OutputLocation = ApplicationData::Current->LocalFolder;
	OutputLocationPath = Platform::String::Concat(OutputLocation->Path, "\\");
	const wchar_t *pWcharOutputFile = OutputLocationPath->Data();

	int size = wcslen(pWcharOutputFile);
	OutputPath[size] = 0;
	for (int y = 0; y<size; y++)
	{
		OutputPath[y] = (char)pWcharOutputFile[y];
	}

	
	// test all cases
	argv[0] = "CodecSimpleCaseAPP";
	argv[1] = OutputPath;
	argc = 2;
	iRet = CodecUtMain(argc, argv);

	return iRet;

}