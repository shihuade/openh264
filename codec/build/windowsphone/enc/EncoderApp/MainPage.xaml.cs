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
//using EncoderRTComponent;

//using  WelsEncRTComponent;
using  WindowsPhoneRuntimeComponent1;

namespace EncoderApp
{
    public partial class MainPage : PhoneApplicationPage
    {
        bool bFlag;
        // Constructor
        private WindowsPhoneRuntimeComponent vRTCEncoder;

        public MainPage()
        {
            InitializeComponent();
            bFlag = false;
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private void Button_Click_NonEncCall(object sender, RoutedEventArgs e)
        {
            if (false == bFlag)
            {
                NonEncoder.Text = "hello, Non-Dll!";
                bFlag = true;
            }
            else
            {
                NonEncoder.Text = "hello, Non-Dll! see you again!";
                bFlag = false;
            }
        }

        private void Button_Click_EncoderCall(object sender, RoutedEventArgs e)
        {
            int iRetVal = 0;
            vRTCEncoder = new WindowsPhoneRuntimeComponent();
            if (false == bFlag)
            {
                iRetVal = vRTCEncoder.Add(10, 10);
                EncoderCall.Text = "hello, Non-Dll!";
                bFlag = true;
            }
            else
            {
                iRetVal = vRTCEncoder.Add (111,111);
                EncoderCall.Text = "hello, Non-Dll! see you again!";
                bFlag = false;
            }

            EncoderCall.Text = iRetVal.ToString();
            vRTCEncoder.FileHandle();
            vRTCEncoder.Encode();

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