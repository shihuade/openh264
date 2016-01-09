#pragma once

namespace CodecRTSimulator
{
    public ref class Class1 sealed
    {
    public:
        Class1();

		int Encode();
		int Decode();

		//Get encoder info
		float GetEncFPS();
		double GetEncTime();
		int  GetEncodedFrameNum();

		//get decoder info
		float GetDecFPS();
		double GetDecTime();
		int  GetDecodedFrameNum();

    };
}
