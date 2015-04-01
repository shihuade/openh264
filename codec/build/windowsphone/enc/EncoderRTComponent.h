#pragma once

namespace EncoderRTComponent
{
    //public ref class WindowsPhoneRuntimeComponent sealed
	public ref class EncoderRTC sealed
    {
    public:
		EncoderRTC();

		int Encode();
		//Get encoder info
		float GetEncFPS();
		double GetEncTime();
		int  GetEncodedFrameNum();
    };
}