#pragma once

namespace DecoderRTComponent
{
    public ref class RTCDecoder sealed
    {
    public:
        RTCDecoder();
		int Decode();

		//get decoder info
		float GetDecFPS();
		double GetDecTime();
		int  GetDecodedFrameNum();
    };
}