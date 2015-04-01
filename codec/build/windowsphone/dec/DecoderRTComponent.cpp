// DecoderRTComponent.cpp

#include <string.h>
#include <iostream>
#include "typedefs.h"
#include "DecoderRTComponent.h"


using namespace DecoderRTComponent;
using namespace Platform;
using namespace Windows::Storage;

extern int32_t DecMain(int32_t iArgC, char* pArgV[]);

//decoder info
extern double g_dDecTime;
extern float  g_fDecFPS;
extern int    g_iDecodedFrameNum;

RTCDecoder::RTCDecoder()
{
}

float RTCDecoder::GetDecFPS()
{
	return g_fDecFPS;
}
double RTCDecoder::GetDecTime()
{
	return g_dDecTime;
}
int RTCDecoder::GetDecodedFrameNum()
{
	return g_iDecodedFrameNum;
}
int RTCDecoder::Decode()
{
	int iRet = 0;
	int argc = 3;
	char *argv[3];
	int size = 0;

	//App data files' path
	char InputPath[256] = { 0 };
	char OutputPath[256] = { 0 };
	char InputBitstreamPath[256] = { 0 };
	char OutputYUVPath[256] = { 0 };

	//App data location
	Windows::Storage::StorageFolder^ InputLocation;
	Platform::String^ InputLocationPath;

	Windows::Storage::StorageFolder^ OutputLocation;
	Platform::String^ OutputLocationPath;

	//set input file path
	InputLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;
	InputLocationPath = Platform::String::Concat(InputLocation->Path, "\\");
	const wchar_t *pWcharInputFile = InputLocationPath->Data();

	size = wcslen(pWcharInputFile);
	InputPath[size] = 0;
	for (int y = 0; y<size; y++)
	{
		InputPath[y] = (char)pWcharInputFile[y];
	}

	//set output file path
	OutputLocation = ApplicationData::Current->LocalFolder;
	OutputLocationPath = Platform::String::Concat(OutputLocation->Path, "\\");
	const wchar_t *pWcharOutputFile = OutputLocationPath->Data();

	size = wcslen(pWcharOutputFile);
	OutputPath[size] = 0;
	for (int y = 0; y<size; y++)
	{
		OutputPath[y] = (char)pWcharOutputFile[y];
	}

	//App test setting
	strcpy_s(InputBitstreamPath, InputPath);
	strcat_s(InputBitstreamPath, "BA_MW_D.264");

	strcpy_s(OutputYUVPath, OutputPath);
	strcat_s(OutputYUVPath, "WP8_Dec_BA_MW_D.yuv");

	argv[0] = (char *)("DecoderApp");
	argv[1] = InputBitstreamPath;
	argv[2] = OutputYUVPath;

	argc = 3;
	iRet = DecMain(argc, argv);

	return iRet;
}