// DecoderRTComponent.cpp

#include <string.h>
#include <iostream>
#include "typedefs.h"
#include "DecoderRTComponent.h"


using namespace DecoderRTComponent;
using namespace Platform;
using namespace Windows::Storage;

extern int32_t DecMain(int32_t iArgC, char* pArgV[]);

RTCDecoder::RTCDecoder()
{
}

int RTCDecoder::decode()
{
	int argc = 2;
	char *argv[6];
	int size = 0;

	////////////////////////////////////////////////////////////////
	char InputPath[256] = { 0 };
	char OutputPath[256] = { 0 };

	char InputBitstreamPath[256] = { 0 };
	char OutputYUVPath[256] = { 0 };

	/////////////////////////////////
	Windows::Storage::StorageFolder^ InputLocation;
	Platform::String^ InputLocationPath;

	Windows::Storage::StorageFolder^ OutputLocation;
	Platform::String^ OutputLocationPath;

	/////////////////////////////////
	//get input file path
	InputLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;
	InputLocationPath = Platform::String::Concat(InputLocation->Path, "\\");
	const wchar_t *pWcharInputFile = InputLocationPath->Data();

	size = wcslen(pWcharInputFile);
	InputPath[size] = 0;
	for (int y = 0; y<size; y++)
	{
		InputPath[y] = (char)pWcharInputFile[y];
	}

	/////////////////////////////////
	//get output file path
	OutputLocation = ApplicationData::Current->LocalFolder;
	OutputLocationPath = Platform::String::Concat(OutputLocation->Path, "\\");
	const wchar_t *pWcharOutputFile = OutputLocationPath->Data();

	size = wcslen(pWcharOutputFile);
	OutputPath[size] = 0;
	for (int y = 0; y<size; y++)
	{
		OutputPath[y] = (char)pWcharOutputFile[y];
	}

	////////////////////////////////////////////////////////////////

	strcpy_s(InputBitstreamPath, InputPath);
	strcat_s(InputBitstreamPath, "BA_MW_D.264");

	strcpy_s(OutputYUVPath, OutputPath);
	strcat_s(OutputYUVPath, "WP8_Dec_BA_MW_D.yuv");


	FILE  *pcInputBitstreamFile = NULL;
	FILE  *pcOutputYUVFile = NULL;

	char  Buffer[30];

	pcInputBitstreamFile = fopen(InputBitstreamPath, "r");
	pcOutputYUVFile = fopen(OutputYUVPath, "w");

	if (NULL == pcOutputYUVFile || NULL == pcInputBitstreamFile)
	{
		std::cout << "" << std::endl;
		return 1;
	}

	fclose(pcInputBitstreamFile);
	fclose(pcOutputYUVFile);


	argv[0] = (char *)("DecoderApp");
	argv[1] = InputBitstreamPath;  //(char *)("test.264");
	argv[2] = OutputYUVPath;       //(char *)("Dec_Test.yuv");
	
	argc = 3;
	DecMain(argc, argv);
	return 0;

}