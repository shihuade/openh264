//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace CodecAllInOneApp;

using namespace Platform;
using namespace Windows;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
//using namespace CodecRTSimulator;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	//GiraffeFactory gf;
	//Giraffe* g = gf.Create();
	//int id = g->GetID();
}



void CodecAllInOneApp::MainPage::CallDecoder(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	int iRetVal = 0;
	float fFPS = 0.0F;
	double dDecoderTime = 0.0;
	int iDecodedFrame = 0;

	String^ sDecoderInfo = "Decoder performance: \n";

	/*
	auto vRTCCodec = ref new CodecRTComponent();
	iRetVal = vRTCCodec->Decode();

	if (0 == iRetVal) {
		fFPS = vRTCCodec->GetDecFPS();
		dDecoderTime = vRTCCodec->GetDecTime();
		iDecodedFrame = vRTCCodec->GetDecodedFrameNum();
		sDecoderInfo += "FPS : " + fFPS.ToString() + "\n";
		sDecoderInfo += "DecTime(sec): " + dDecoderTime.ToString() + "\n";
		sDecoderInfo += "DecodedFrameNum: " + iDecodedFrame.ToString() + "\n";
		DecoderInfo->Text  = sDecoderInfo;
	}
	else {
		DecoderInfo->Text = "Decoded failed!...";
	}
	*/
}


void CodecAllInOneApp::MainPage::CallEncoder(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	int iRetVal = 0;
	float fFPS = 0.0F;
	double dEncoderTime = 0.0;
	int iEncodedFrame = 0;
	String^ sEncoderInfo = "Encoder performance: \n";

	/*
	auto vRTCCodec = ref new CodecRTComponent();
	iRetVal = vRTCCodec->Encode();

	if (0 == iRetVal) {
		fFPS = vRTCCodec->GetEncFPS();
		dEncoderTime = vRTCCodec->GetEncTime();
		iEncodedFrame = vRTCCodec->GetEncodedFrameNum();
		sEncoderInfo += "FPS: " + fFPS.ToString() + "\n";
		sEncoderInfo += "EncTime(sec): " + dEncoderTime.ToString() + "\n";
		sEncoderInfo += "EncodedFrameNum: " + iEncodedFrame.ToString() + "\n";
		EncoderInfo->Text  = sEncoderInfo;
	}
	else {
		EncoderInfo->Text = "Encoded failed!...";
	}
	*/
}

