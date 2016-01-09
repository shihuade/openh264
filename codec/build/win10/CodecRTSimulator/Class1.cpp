#include "pch.h"
#include "Class1.h"

using namespace CodecRTSimulator;
using namespace Platform;

Class1::Class1()
{
}

int Class1::Encode()
{
	return 400;
}

int Class1::Decode()
{
	return 500;
}

//Get encoder info
float Class1::GetEncFPS()
{
	return 32.3;

}

double Class1::GetEncTime()
{
	return 0.72;

}

int  Class1::GetEncodedFrameNum()
{
	return 128;
}

//get decoder info
float Class1::GetDecFPS()
{
	return 86.3;

}
double Class1::GetDecTime()
{
	return 0.88;
}

int  Class1::GetDecodedFrameNum()
{
	return 128;
}
