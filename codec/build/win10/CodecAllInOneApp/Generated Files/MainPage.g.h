﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class StackPanel;
                ref class Button;
                ref class TextBox;
            }
        }
    }
}

namespace CodecAllInOneApp
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::StackPanel^ contentPanel;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ DecoderCall;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ contentPanel_Copy2;
        private: ::Windows::UI::Xaml::Controls::Button^ button2;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBox2;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ inputPanel2;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ EncoderCall;
        private: ::Windows::UI::Xaml::Controls::Button^ button1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ textBox1;
        private: ::Windows::UI::Xaml::Controls::StackPanel^ inputPanel1;
    };
}

