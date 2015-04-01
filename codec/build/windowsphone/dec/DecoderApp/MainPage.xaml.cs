using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using DecoderApp.Resources;
using DecoderRTComponent;

namespace DecoderApp
{
    public partial class MainPage : PhoneApplicationPage
    {
        DecoderRTComponent.RTCDecoder cRTCDecoder;
        // Constructor
        public MainPage()
        {
            InitializeComponent();
     
            cRTCDecoder = new RTCDecoder();

            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            int iRetVal = 0;
            float fFPS = 0.0F;
            double dDecoderTime = 0.0;
            int iDecodedFrame = 0;
            string sDecoderInfo = "Decoder performance: \n";

            iRetVal = cRTCDecoder.Decode();

            if (0 == iRetVal)
            {
                fFPS = cRTCDecoder.GetDecFPS();
                dDecoderTime = cRTCDecoder.GetDecTime();
                iDecodedFrame = cRTCDecoder.GetDecodedFrameNum();
                sDecoderInfo += "FPS         : " + fFPS.ToString() + "\n";
                sDecoderInfo += "DecTime(sec): " + dDecoderTime.ToString() + "\n";
                sDecoderInfo += "DecodedNum  : " + iDecodedFrame.ToString() + "\n";
                DecoderInfo.Text = sDecoderInfo;
            }
            else
            {
                DecoderInfo.Text = "decoder failed!...";
            }               
        }

        // Sample code for building a localized ApplicationBar
        //private void BuildLocalizedApplicationBar()
        //{
        //    // Set the page's ApplicationBar to a new instance of ApplicationBar.
        //    ApplicationBar = new ApplicationBar();

        //    // Create a new button and set the text value to the localized string from AppResources.
        //    ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
        //    appBarButton.Text = AppResources.AppBarButtonText;
        //    ApplicationBar.Buttons.Add(appBarButton);

        //    // Create a new menu item with the localized string from AppResources.
        //    ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.AppBarMenuItemText);
        //    ApplicationBar.MenuItems.Add(appBarMenuItem);
        //}
    }
}