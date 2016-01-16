//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace CodecAllInOneApp
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		//CodecRTComponent^ vRTCCodec;
		void CallDecoder(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CallEncoder(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
