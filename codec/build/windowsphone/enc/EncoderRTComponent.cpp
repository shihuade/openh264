// EncoderRTComponent.cpp
#include <string.h>
#include <iostream>
#include "typedefs.h"
#include "EncoderRTComponent.h"

using namespace EncoderRTComponent;
using namespace Platform;
using namespace Windows::Storage;

extern "C" int EncMain(int argc, char** argv);
//encoder info
extern float   g_fFPS;
extern double  g_dEncoderTime;
extern int     g_iEncodedFrame;

EncoderRTC::EncoderRTC()
{
}

float EncoderRTC::GetEncFPS()
{
	return g_fFPS;
}
double EncoderRTC::GetEncTime()
{
	return g_dEncoderTime;
}
int  EncoderRTC::GetEncodedFrameNum()
{
	return g_iEncodedFrame;
}

int EncoderRTC::Encode()
{
	int iRet = 0;
	int argc = 6;
	char *argv[6];
	int iSize = 0;

	//App test data files' path
	char InputPath[256] = { 0 };
	char InputWelsEncCfgPath[256] = { 0 };
	char InputCfgPath[256] = { 0 };
	char InputYUVPath[256] = { 0 };
	char OutputPath[256] = { 0 };
	char Output264Path[256] = { 0 };

	//App data location
	Windows::Storage::StorageFolder^ InputLocation;
	Platform::String^ InputLocationPath;
	Windows::Storage::StorageFolder^ OutputLocation;
	Platform::String^ OutputLocationPath;

	//set input file path
	InputLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;
	InputLocationPath = Platform::String::Concat(InputLocation->Path, "\\");
	const wchar_t *pWcharInputFile = InputLocationPath->Data();

	iSize = wcslen(pWcharInputFile);
	InputPath[iSize] = 0;
	for (int y = 0; y<iSize; y++)
	{
		InputPath[y] = (char)pWcharInputFile[y];
	}

	//set output file path
	OutputLocation = ApplicationData::Current->LocalFolder;
	OutputLocationPath = Platform::String::Concat(OutputLocation->Path, "\\");
	const wchar_t *pWcharOutputFile = OutputLocationPath->Data();

	iSize = wcslen(pWcharOutputFile);
	OutputPath[iSize] = 0;
	for (int y = 0; y<iSize; y++)
	{
		OutputPath[y] = (char)pWcharOutputFile[y];
	}

	//App 
	strcpy(InputWelsEncCfgPath, InputPath);
	strcat(InputWelsEncCfgPath, "welsenc.cfg");

	strcpy(InputYUVPath, InputPath);
	strcat(InputYUVPath, "CiscoVT2people_160x96_6fps.yuv");

	// single layer only
	strcpy(InputCfgPath, InputPath);
	strcat(InputCfgPath, "layer2.cfg");
	// for multiple layers
	/*
	strcpy(InputCfg0Path, InputPath);
	strcat(InputCfg0Path, "layer0.cfg");
	strcpy(InputCfg1Path, InputPath);
	strcat(InputCfg1Path, "layer1.cfg");
	...
	*/

	strcpy(Output264Path, OutputPath);
	strcat(Output264Path, "WP8_Test_CiscoVT2people_160x96_6fps.264");

	argv[0] = (char *)("EncoderApp");
	argv[1] = InputWelsEncCfgPath;
	argv[2] = (char *)("-org");
	argv[3] = InputYUVPath;
	argv[4] = (char *)("-bf");
	argv[5] = Output264Path;

	argc = 6;
	iRet = EncMain(argc, argv);

	return iRet;
}