using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace CCMR_GUI
{
    public sealed partial class Scrollitem : UserControl
    {
        public Scrollitem()
        {
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {

            
        }
        public void setbtncolor(String btn, String col)
        {
            //button1.Background = new SolidColorBrush(Color.FromArgb(40, 0, 255, 0));
            // if (col == "Red") testa = "#66FF0000";
            // if (col == "Green") testa = "#6600FF00";
        }

        /// <summary>
        /// The event that you will throw when the mouse hover the control while being clicked 
        /// </summary>
       // public event EventHandler MouseRightClickedAndHoverChildControl;
    }

}
