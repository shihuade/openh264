using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using CodecRTSimulator;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace CodecApp
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {   private CodecRTComponent vRTCCodec;
        public MainPage()
        {
            this.InitializeComponent();
            vRTCCodec =  new CodecRTComponent();
        }

        private void CallDecoder(object sender, RoutedEventArgs e)
        {
            int iRetVal = 0;
            float fFPS = 0.0F;
            double dDecoderTime = 0.0;
            int iDecodedFrame = 0;

            string  sDecoderInfo = "Decoder performance: \n";

            iRetVal = vRTCCodec.Decode();

            if (0 == iRetVal)
            {
                fFPS = vRTCCodec.GetDecFPS();
                dDecoderTime = vRTCCodec.GetDecTime();
                iDecodedFrame = vRTCCodec.GetDecodedFrameNum();
                sDecoderInfo += "FPS : " + fFPS.ToString() + "\n";
                sDecoderInfo += "DecTime(sec): " + dDecoderTime.ToString() + "\n";
                sDecoderInfo += "DecodedFrameNum: " + iDecodedFrame.ToString() + "\n";
                DecoderInfo.Text = sDecoderInfo;
            }
            else {
                DecoderInfo.Text = "Decoded failed!...";
            }

        }

        private void CallEncoder(object sender, RoutedEventArgs e)
        {
            int iRetVal = 0;
            float fFPS = 0.0F;
            double dEncoderTime = 0.0;
            int iEncodedFrame = 0;
            string  sEncoderInfo = "Encoder performance: \n";

            iRetVal = vRTCCodec.Encode();

            if (0 == iRetVal)
            {
                fFPS = vRTCCodec.GetEncFPS();
                dEncoderTime = vRTCCodec.GetEncTime();
                iEncodedFrame = vRTCCodec.GetEncodedFrameNum();
                sEncoderInfo += "FPS: " + fFPS.ToString() + "\n";
                sEncoderInfo += "EncTime(sec): " + dEncoderTime.ToString() + "\n";
                sEncoderInfo += "EncodedFrameNum: " + iEncodedFrame.ToString() + "\n";
                EncoderInfo.Text = sEncoderInfo;
            }
            else {
                EncoderInfo.Text = "Encoded failed!...";
            }

        }
    }
}
