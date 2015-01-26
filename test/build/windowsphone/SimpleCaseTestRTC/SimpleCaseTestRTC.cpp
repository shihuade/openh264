// SimpleCaseTestRTC.cpp
#include <windows.h>
#include <iostream>
#include "SimpleCaseTestRTC.h"
//#include "SimpleTestCase.h"
#include "SimpleCaseDll.h"

using namespace Platform;
using namespace Windows;
using namespace Windows::Storage;
using namespace SimpleCaseTestRTC;

typedef int(*pfGTestHandler)();
typedef  int(*pfTestAllCases)(int argc, char** argv);

CodecSimpleCasesRTC::CodecSimpleCasesRTC()
{
}

int CodecSimpleCasesRTC::NonTest()
{

	return 0;
}

int CodecSimpleCasesRTC::TestAllCasesRTC()
{
	int   c = 0;
	int   argc = 1;
	int   iRet = 0;
	char *argv[6];

	HMODULE                   phTestCasesDllHandler = NULL;
	pfTestAllCases            pGtestTestHandler = NULL;
	LPCWSTR                   cTestCasesDllDLLName = L"SimpleCaseDll.dll";

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

	// load dynamic library
	phTestCasesDllHandler = LoadPackagedLibrary(cTestCasesDllDLLName, 0);
	DWORD dw = GetLastError();
	if (NULL == phTestCasesDllHandler)
	{
		std::cout << "failed to load dll,error code is : " << dw << std::endl;
		return 1;
	}

	pGtestTestHandler = (pfTestAllCases)GetProcAddress(phTestCasesDllHandler, "TestAllCases");

	if (NULL == pGtestTestHandler)
	{
		std::cout << "failed to load function" << std::endl;
		return 2;
	}


	// test all cases
	argv[0] = "CodecSimpleCaseAPP";
	argv[1] = OutputPath;
	argc = 2;
	iRet = pGtestTestHandler(argc,argv);

	return c;

}

int CodecSimpleCasesRTC::SimpleTest()
{

	int   c       = 0;
	int   argc    = 1;
	int   iRet    = 0;
	char *argv[6];

	argv[0] = "CodecSimpleCaseAPP";


	HMODULE                   phTestCasesDllHandler   = NULL;
	pfGTestHandler            pGtestTestHandler       = NULL;
	LPCWSTR                   cTestCasesDllDLLName    = L"SimpleCaseDll.dll";

	phTestCasesDllHandler = LoadPackagedLibrary(cTestCasesDllDLLName, 0);
	DWORD dw = GetLastError();
	if (NULL == phTestCasesDllHandler)
	{
		std::cout << "failed to load dll,error code is : " << dw << std::endl;
		return 1;
	}

	pGtestTestHandler = (pfGTestHandler)GetProcAddress(phTestCasesDllHandler, "MySimpleTest");
	
	if (NULL == pGtestTestHandler)
	{
		std::cout << "failed to load function" << std::endl;
		return 2;
	}

	iRet = pGtestTestHandler();

	return c;

}
