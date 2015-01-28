using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using CodecUTApp.Resources;

using Codec_UT_RTComponent;

namespace CodecUTApp
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        bool bFlag;
        Codec_UT_RTComponent.CodecUTTest cCodecUTHandler;
        public MainPage()
        {
            InitializeComponent();

            bFlag = false;
            cCodecUTHandler = new CodecUTTest();
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (false == bFlag)
            {
                UTInfo.Text = "Hello! UT test cases!";
                bFlag = true;
            }
            else
            {
                UTInfo.Text = "Hello! UT test cases again!";
                bFlag = false;
            }

            cCodecUTHandler.TestAllCases();
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