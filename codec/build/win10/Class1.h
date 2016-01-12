#pragma once

namespace CodecRTSimulator
{
    public ref class CodecRTComponent sealed
    {
    public:
        CodecRTComponent();

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
