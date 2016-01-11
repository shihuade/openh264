#include "pch.h"
#include "Class1.h"

using namespace CodecRTSimulator;
using namespace Platform;

CodecRTComponent::CodecRTComponent()
{
}

int CodecRTComponent::Encode()
{
	return 0;  // 400;
}

int CodecRTComponent::Decode()
{
	return 500;
}

//Get encoder info
float CodecRTComponent::GetEncFPS()
{
	return 32.3;

}

double CodecRTComponent::GetEncTime()
{
	return 0.72;

}

int  CodecRTComponent::GetEncodedFrameNum()
{
	return 128;
}

//get decoder info
float CodecRTComponent::GetDecFPS()
{
	return 86.3;

}
double CodecRTComponent::GetDecTime()
{
	return 0.88;
}

int  CodecRTComponent::GetDecodedFrameNum()
{
	return 128;
}
