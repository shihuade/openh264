using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using EncoderApp.Resources;
using EncoderRTComponent;


namespace EncoderApp
{
    public partial class MainPage : PhoneApplicationPage
    {
        private EncoderRTC vRTCEncoder;
        // Constructor
        public MainPage()
        {
            InitializeComponent();

            vRTCEncoder = new EncoderRTC();
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {     
            int iRetVal = 0;
            float fFPS = 0.0F;
            double dEncoderTime = 0.0;
            int iEncodedFrame = 0;
            string sEncoderInfo = "Encoder performance: \n";

            iRetVal = vRTCEncoder.Encode();

            if (0 == iRetVal)
            {
                fFPS = vRTCEncoder.GetEncFPS();
                dEncoderTime = vRTCEncoder.GetEncTime();
                iEncodedFrame = vRTCEncoder.GetEncodedFrameNum();
                sEncoderInfo += "FPS         : " + fFPS.ToString() + "\n";
                sEncoderInfo += "EncTime(sec): " + dEncoderTime.ToString() + "\n";
                sEncoderInfo += "EncodedNum  : " + iEncodedFrame.ToString() + "\n";
                EncoderInfo.Text = sEncoderInfo;
            }
            else
            {
                EncoderInfo.Text = "ebcoder failed!...";
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