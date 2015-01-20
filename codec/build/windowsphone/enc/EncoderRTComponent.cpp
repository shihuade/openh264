// WindowsPhoneRuntimeComponent1.cpp

#include <string.h>
#include <iostream>
#include "EncoderRTComponent.h"

using namespace WindowsPhoneRuntimeComponent1;
using namespace Platform;
using namespace Windows::Storage;

//extern int main_demo(int argc, char **argv);
extern "C" int EncMain(int argc, char** argv);

WindowsPhoneRuntimeComponent::WindowsPhoneRuntimeComponent()
{
}

int WindowsPhoneRuntimeComponent::Add(int a, int b)
{

	return 10000 + a + b;
}

int WindowsPhoneRuntimeComponent::FileHandle()
{
	int argc = 2;
	char *argv[6];
	int size = 0;
	

	////////////////////////////////////////////////////////////////
	char inputPath[256] = { 0 };
	char outputPath[256] = { 0 };


	char inputWbxencCfgPath[256] = { 0 };
	char inputYUVPath[256] = { 0 };
	char inputGold264Path[256] = { 0 };
	char inputCfgPath[256] = { 0 };
	char output264Path[256] = { 0 };

	/////////////////////////////////
	Windows::Storage::StorageFolder^ m_location;
	Platform::String^ m_locationPath;

	IStorageFile^ TestFile;
	/////////////////////////////////
	//get input file path
	m_location = Windows::ApplicationModel::Package::Current->InstalledLocation;
	m_locationPath = Platform::String::Concat(m_location->Path, "\\Test.txt");
	const wchar_t *pWcharInputFile  = m_locationPath->Data();
	char cTestFileName[200];
	wcstombs(cTestFileName, pWcharInputFile, wcslen(pWcharInputFile) + 1);

	m_location = ApplicationData::Current->LocalFolder;
	m_locationPath = Platform::String::Concat(m_location->Path, "\\TestOutput.txt");
	const wchar_t *pWcharOutputFile = m_locationPath->Data();

	char cTestOutFileName[200];
	wcstombs(cTestOutFileName, pWcharOutputFile, wcslen(pWcharOutputFile) + 1);
	//String^ FileContent = TestFile.

	FILE  *pcFile = NULL;
	FILE  *pcOutFile = NULL;

	char Buffer[30];

	pcFile = fopen(cTestFileName,"r");
	pcOutFile = fopen(cTestOutFileName, "w+");
	if (NULL == pcFile || NULL == pcOutFile)
	{
		std::cout << "" << std::endl;
		return 1;
	}
	fread(Buffer,1,29,pcFile);

	fwrite(Buffer, 1, 29, pcOutFile);

	fclose(pcFile);
	fclose(pcOutFile);


	pcFile = fopen(cTestFileName, "r");
	pcOutFile = fopen(cTestOutFileName, "w+");
	if (NULL == pcFile || NULL == pcOutFile)
	{
		std::cout << "" << std::endl;
		return 1;
	}
	fread(Buffer, 1, 29, pcFile);

	fwrite(Buffer, 1, 29, pcOutFile);

	fclose(pcFile);
	fclose(pcOutFile);



	return 1;



}

int WindowsPhoneRuntimeComponent::Encode()
{
	int argc = 2;
	char *argv[6];
	int size = 0;

	////////////////////////////////////////////////////////////////
	char InputPath[256] = { 0 };
	char OutputPath[256] = { 0 };


	char InputWelsEncCfgPath[256] = { 0 };
	char InputYUVPath[256] = { 0 };

	char InputCfgPath[256] = { 0 };
	char Output264Path[256] = { 0 };

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

	strcpy(InputWelsEncCfgPath, InputPath);
	strcat(InputWelsEncCfgPath, "welsenc.cfg"); //"openh264.dll");// "welsenc.cfg");

	strcpy(InputYUVPath,		InputPath);
	strcat(InputYUVPath, "CiscoVT2people_160x96_6fps.yuv");

	strcpy(InputCfgPath, InputPath);
	strcat(InputCfgPath, "layer2.cfg");

	strcpy(Output264Path, OutputPath);
	strcat(Output264Path, "WP8_Test_CiscoVT2people_160x96_6fps.264");

	/*
	strcpy(InputYUVPath,		InputPath);
	strcat(InputYUVPath,		"CiscoVTcmplxbg_160x96_64k.yuv");

	strcpy(inputGold264Path,	InputPath);
	strcat(inputGold264Path,	"T_CiscoVTcmplxbg_160x96_64k.264");

	strcpy(InputCfgPath,		InputPath);
	strcat(InputCfgPath,		"layer0_160x96.cfg");

	strcpy(Output264Path,		OutputPath);
	strcat(Output264Path,		"CiscoVTcmplxbg_160x96_64k.264");
	*/

	FILE  *pcInputYUVFile      = NULL;
	FILE  *pcInputEncCfgFile   = NULL;
	FILE  *pcInputLayerCfgFile = NULL;
	FILE  *pcOutput264File     = NULL;

	char  Buffer[30];

	pcInputEncCfgFile   = fopen(InputWelsEncCfgPath, "r");
	pcInputLayerCfgFile = fopen(InputCfgPath, "r");
	pcInputYUVFile      = fopen(InputYUVPath, "r");
	pcOutput264File = fopen(Output264Path, "w");

	if (NULL == pcInputEncCfgFile )
	{
	
		return 1;
	}

	if (NULL == pcInputEncCfgFile || NULL == pcInputLayerCfgFile 
		|| NULL == pcInputYUVFile )//|| NULL == pcOutput264File)
	{
		std::cout << "" << std::endl;
		return 1;
	}
	fclose(pcInputEncCfgFile);
	fclose(pcInputLayerCfgFile);
	fclose(pcInputYUVFile);
	fclose(pcOutput264File);


	argv[0] = (char *)("EncoderApp");
	argv[1] = InputWelsEncCfgPath;  //(char *)("welsenc.cfg");
	argv[2] = (char *)("-org");     //(char *)("-org");
	argv[3] = InputYUVPath;         //(char *)("C:\\LocalData\\...");
	argv[4] = (char *)("-bf");      //(char *)("-bf");
	argv[5] = Output264Path;        //(char *)("layer2.cfg");

	argc = 6;
	//main_demo(argc, argv);
	EncMain(argc, argv);
	return 0;
}