using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading;
using System.Threading.Tasks;
using Windows.Devices.SerialCommunication;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Storage.Streams;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using MySql;
using MySql.Data;
using MySql.Data.MySqlClient;
using Windows.UI;
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace CCMR_GUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        //BINDINGS\\
        public string gauge_ovenTemp = "10";
        public string gauge_CO2pressure = "10";
        public string gauge_CO2 = "10";
        public string gauge_H2 = "10";
        public string gauge_H2pressure = "10";
        public string gauge_6 = "10";

        public string P000 = "Nothing";
        public string P001 = "Nothing";
        public string P002 = "Nothing";
        public string P003 = "Nothing";
        public string P004 = "Nothing";
        public string FT000 = "Nothing";
        public string FT002 = "Nothing";
        public string FT003 = "Nothing";
        public string FL001 = "Nothing";
        public string STM001 = "Nothing";
        public string STM000 = "Nothing";
        public string FS001 = "Nothing";
        public string GK004 = "Nothing";
        public string FL000 = "Nothing";
        public string OV_TMP = "Nothing";
        public string WAT_O2 = "Nothing";
        public string WAT_H2 = "Nothing";
        public string WAT_CH4 = "Nothing";
        
        //END BINDINGS\\
        String SERIAL_DEVICE = "\\\\?\\ACPI#BCM2836#0#{86e0d1e0-8089-11d0-9ce4-08003e301f73}";
        byte cmdid;
        ushort cmdval;
        Boolean[] Valve_stat = new Boolean[6];
        /// <summary>
        /// Private variables
        Boolean CO2VALVE = false;
        Boolean H2INVALVE = false;
        Boolean H2OUTVALVE = false;
        Boolean O2INVALVE = false;
        Boolean O2OUTVALVE = false;
        Boolean H2REFLUX = false;
        Boolean O2REFLUX = false;
        Boolean CHWATER = false;
        Boolean PUMP = false;
        Boolean FLUSH = false;
        /// </summary>
        ///   Dim Mysqlconn As MySqlConnection
        String query;
        MySqlCommand COMMAND;
        MySqlConnection Mysqlconn;
        private SerialDevice serialPort = null;
        DataWriter dataWriteObject = null;
        DataReader dataReaderObject = null;
        private CancellationTokenSource ReadCancellationTokenSource;
        SolidColorBrush Green = new SolidColorBrush(Color.FromArgb(102, 0, 255, 0));
        SolidColorBrush Red = new SolidColorBrush(Color.FromArgb(102, 255, 0, 0));

        byte active = 1;
        byte inactive = 0;


        public MainPage()
        {
            this.InitializeComponent();
            this.DataContextChanged += (s, e) => this.Bindings.Update();
            RAWCONSOLE.TextChanged += new TextChangedEventHandler(TextBox_TextChanged);

            //init
            status.Text = "GUI Initialized";
            //showmode.IsChecked = true;

            //scroller.MouseRightClickedAndHoverChildControl += OnMouseHoverChildControl;

            connect();
        }
   
        void App_UnhandledException(object sender, UnhandledExceptionEventArgs e) { }
        //public void tweakmode_Checked(object sender, RoutedEventArgs e)
        //{
        //    Num1.IsEnabled = true;
        //    Num2.IsEnabled = true;
        //    Num3.IsEnabled = true;
        //    //buttonx.Background = new SolidColorBrush(Color.FromArgb(40, 255, 0, 0));
        //}
        //public void showmode_Checked(object sender, RoutedEventArgs e)
        //{
        //    Num1.IsEnabled = false;
        //    Num2.IsEnabled = false;
        //    Num3.IsEnabled = false;
        //    // buttonx.Background = new SolidColorBrush(Color.FromArgb(40, 0, 255, 0));
        //}
        //serial Functions
        private async void connect()
        {

            try
            {

                serialPort = await SerialDevice.FromIdAsync(SERIAL_DEVICE);
                serialPort.WriteTimeout = TimeSpan.FromMilliseconds(30);
                serialPort.ReadTimeout = TimeSpan.FromMilliseconds(30); //IMPORTANT!!!!!!
                serialPort.BaudRate = 115200;
                serialPort.Parity = SerialParity.None;
                serialPort.StopBits = SerialStopBitCount.One;
                serialPort.DataBits = 8;
                serialPort.Handshake = SerialHandshake.None;
                ReadCancellationTokenSource = new CancellationTokenSource();
                dataReaderObject = new DataReader(serialPort.InputStream);
                dataReaderObject.InputStreamOptions = InputStreamOptions.Partial;
                dataWriteObject = new DataWriter(serialPort.OutputStream);

               
            }
            catch (Exception ex)
            {
                status.Text = ex.Message;
            }
            //  Listen();
            StartReceive();
            System.Diagnostics.Debug.WriteLine("TERMINATION");
        }
        public async void StartReceive()
        {

            ReadCancellationTokenSource = new CancellationTokenSource();

            while (true)
            {
                await ListenT();
                if ((ReadCancellationTokenSource.Token.IsCancellationRequested) || (serialPort == null))
                    break;
            }
        }
        private async Task ListenT()
        {
            const int NUMBER_OF_BYTES_TO_RECEIVE = 3;           //<<<<<SET THE NUMBER OF BYTES YOU WANT TO WAIT FOR

            Task<UInt32> loadAsyncTask;
            byte[] ReceiveData;
            UInt32 bytesRead;

            try
            {
                if (serialPort != null)
                {
                    while (true)
                    {
                        //###### WINDOWS IoT MEMORY LEAK BUG 2017-03 - USING CancellationToken WITH LoadAsync() CAUSES A BAD MEMORY LEAK.  WORKAROUND IS
                        //TO BUILD RELEASE WITHOUT USING THE .NET NATIVE TOOLCHAIN OR TO NOT USE A CancellationToken IN THE CALL #####
                        //bytesRead = await dataReaderObject.LoadAsync(NUMBER_OF_BYTES_TO_RECEIVE).AsTask(ReadCancellationTokenSource.Token);	//Wait until buffer is full
                        bytesRead = await dataReaderObject.LoadAsync(NUMBER_OF_BYTES_TO_RECEIVE).AsTask();  //Wait until buffer is full

                        if ((ReadCancellationTokenSource.Token.IsCancellationRequested) || (serialPort == null))
                            break;

                        if (bytesRead > 0)
                        {
                            ReceiveData = new byte[NUMBER_OF_BYTES_TO_RECEIVE];
                            dataReaderObject.ReadBytes(ReceiveData);

                            cmdid = ReceiveData[0];//dataReaderObject.ReadBytes();

                            cmdval = (UInt16)((ReceiveData[2] << 8) + ReceiveData[1]);
                            System.Diagnostics.Debug.Write(cmdid + " || ");
                            System.Diagnostics.Debug.WriteLine(cmdval);
                            RAWCONSOLE.Text += (cmdid + " || ");
                            //dataReaderObject.ReadByte();
                            RAWCONSOLE.Text += (cmdval + "\n");

                        }
                        handleCommands();
                    }
                }
            }
            catch (Exception e)
            {
                //We will get here often if the USB serial cable is removed so reset ready for a new connection (otherwise a never ending error occurs)
                if (ReadCancellationTokenSource != null)
                    ReadCancellationTokenSource.Cancel();
                System.Diagnostics.Debug.WriteLine("UART ReadAsync Exception: {0}", e.Message);
            }
        }
        private async void Serial_send(String _text)
        {
            try
            {
                if (serialPort != null)
                {
                    // Create the DataWriter object and attach to OutputStream
                    dataWriteObject = new DataWriter(serialPort.OutputStream);

                    //Launch the WriteAsync task to perform the write
                    await Writetext(_text);
                }
                else
                {
                    status.Text = "No Device Found";
                }
            }
            catch (Exception ex)
            {
                status.Text = ex.Message;
            }
            finally
            {
                // Cleanup once complete
                if (dataWriteObject != null)
                {
                    dataWriteObject.DetachStream();
                    dataWriteObject = null;
                }
            }
        }
        private async Task Writetext(String text)
        {
            Task<UInt32> storeAsyncTask;

            if (text.Length != 0)
            {
                // Load the text from the sendText input text box to the dataWriter object
                dataWriteObject.WriteString(text);

                // Launch an async task to complete the write operation
                storeAsyncTask = dataWriteObject.StoreAsync().AsTask();

                UInt32 bytesWritten = await storeAsyncTask;
                if (bytesWritten > 0)
                {
                    status.Text = text + ", ";
                    status.Text += "bytes written successfully!";
                }

            }
        }

        private async Task Send_to_CCMR(byte cmdid, byte cmdval)
        {
            Task<UInt32> storeAsyncTask;
            try
            {
                if (serialPort != null)
                {

                    // Create the DataWriter object and attach to OutputStream
                    dataWriteObject = new DataWriter(serialPort.OutputStream);

                    //Launch the WriteAsync task to perform the write
                    dataWriteObject.WriteByte(cmdid);
                    dataWriteObject.WriteByte(cmdval);


                }
                else
                {
                    status.Text = "No Device Found";
                }
            }
           catch(Exception ex)
            {
                status.Text = ex.Message;
            }
            finally
            {
                storeAsyncTask = dataWriteObject.StoreAsync().AsTask();
                UInt32 bytesWritten = await storeAsyncTask;
                if (bytesWritten > 0)
                {
                    status.Text += "||bytes written successfully!";

                }
                // Cleanup once complete
                if (dataWriteObject != null)
                {
                    dataWriteObject.DetachStream();
                    dataWriteObject = null;
                }
            }


            

            
            

        }
        private async void Listen()
        {
            try
            {
                if (serialPort != null)
                {
                    dataReaderObject = new DataReader(serialPort.InputStream);

                    // keep reading the serial input
                    while (true)
                    {
                        //System.Diagnostics.Debug.WriteLine("Loop");
                        await ReadAsync(ReadCancellationTokenSource.Token);
                    }
                }
            }
            catch (Exception ex)
            {
                if (ex.GetType().Name == "TaskCanceledException")
                {
                    status.Text = "Reading task was cancelled, closing device and cleaning up";
                    CloseDevice();
                }
                else
                {
                    status.Text = ex.Message;
                }
            }
            finally
            {
                // Cleanup once complete
                if (dataReaderObject != null)
                {
                    dataReaderObject.DetachStream(); //HIER!!!!!
                    dataReaderObject = null;
                }
            }
        }
        private async Task ReadAsync(CancellationToken cancellationToken)
        {
            Task<UInt32> loadAsyncTask;
            
                uint ReadBufferLength = 1024;

            // If task cancellation was requested, comply
            cancellationToken.ThrowIfCancellationRequested();
            
                // Set InputStreamOptions to complete the asynchronous read operation when one or more bytes is available
                dataReaderObject.InputStreamOptions = InputStreamOptions.Partial;
            
                // Create a task object to wait for data on the serialPort.InputStream
                loadAsyncTask = dataReaderObject.LoadAsync(ReadBufferLength).AsTask(cancellationToken);
            
                uint t = dataReaderObject.UnconsumedBufferLength;
       
            buffersize.Text = Convert.ToString(t);
           
                System.Diagnostics.Debug.WriteLine("BUF: " + t);
            
                if (t > 1000)
                {
                    throw new OverflowException();
                }
            // Launch the task and wait
           
                UInt32 bytesRead = await loadAsyncTask;
                if (bytesRead >= 3)
                {
                    byte[] readb = new byte[bytesRead];
                    dataReaderObject.ReadBytes(readb);



                    cmdid = readb[0];//dataReaderObject.ReadBytes();

                    cmdval = (UInt16)((readb[2] << 8) + readb[1]);
                    System.Diagnostics.Debug.Write(cmdid + " || ");
                    System.Diagnostics.Debug.WriteLine(cmdval);
                    RAWCONSOLE.Text += (cmdid + " || ");
                    //dataReaderObject.ReadByte();
                    RAWCONSOLE.Text += (cmdval + "\n");
                

               // handleCommands();
            }
        }
         void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            var grid = (Grid)VisualTreeHelper.GetChild(RAWCONSOLE, 0);
            for (var i = 0; i <= VisualTreeHelper.GetChildrenCount(grid) - 1; i++)
            {
                object obj = VisualTreeHelper.GetChild(grid, i);
                if (!(obj is ScrollViewer)) continue;
                ((ScrollViewer)obj).ChangeView(0.0f, ((ScrollViewer)obj).ExtentHeight, 1.0f);
                break;
            }
        }
        private void CancelReadTask()
        {
            if (ReadCancellationTokenSource != null)
            {
                if (!ReadCancellationTokenSource.IsCancellationRequested)
                {
                    ReadCancellationTokenSource.Cancel();
                }
            }
        }
        private void CloseDevice()
        {
            if (serialPort != null)
            {
                serialPort.Dispose();
            }
            serialPort = null;




        }
        private void closeDevice_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                status.Text = "";
                CancelReadTask();
                CloseDevice();

            }
            catch (Exception ex)
            {
                status.Text = ex.Message;

            }
        }
        private void button_Click(object sender, RoutedEventArgs e)
        {
            status.Text = Mysql_read("new_table", "COL_A")[3];

        }
        //Mysql functions
        List<String> Mysql_read(String table, String column)
        {
            MySqlConnection connect = new MySqlConnection("Server=192.168.1.102; Database=test; Uid=root; Pwd=root;SslMode=none");

            List<String> output = new List<string>();


            try
            {
                connect.Open();

                status.Text = "connected";
                MySqlCommand sqlcmd;
                MySqlDataReader dr;



                sqlcmd = new MySqlCommand("SELECT * FROM " + table, connect);
                dr = sqlcmd.ExecuteReader();

                while (dr.Read())
                {
                    output.Add(dr.GetString(column));
                }
            }
            catch (Exception ex)
            {
                status.Text = ex.Message;
            }
            connect.Close();
            return output;
        }
        void Mysql_write(String table, String column, int index)
        {
            MySqlConnection connect = new MySqlConnection("Server=192.168.1.102; Database=test; Uid=root; Pwd=root;SslMode=none");
            try
            {
                connect.Open();

                status.Text = "connected";
                MySqlCommand sqlcmd;




                sqlcmd = new MySqlCommand("UPDATE `new_table` SET `COL_A`='3' WHERE `COL_A`='2'", connect);

            }
            catch (Exception ex)
            {
                status.Text = ex.Message;
            }
            connect.Close();
        }
        private void setBinding(String t, ref String upd)
        {
            upd = t;
            this.Bindings.Update();
        }
        //real program

        private async void handleCommands()
        {
          // Send_to_CCMR(201, 1);
            switch (cmdid)
            {
                case 002:
                    status.Text = " >> Connection established";
                    await Send_to_CCMR(002, 1);
                    break;
                case 101: //oven temp
                    gauge_ovenTemp = cmdval.ToString();
                    OV_TMP = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 110: //CO2 pressure
                    gauge_CO2pressure = cmdval.ToString();
                    P004 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 106: //CO2
                    gauge_CO2 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 107: //h2
                    gauge_H2 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 109: //H2 stor pressure
                    gauge_H2pressure = cmdval.ToString();
                    P003 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 103: //peltier temp
                    gauge_6 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 104: //H2 water
                    WAT_H2 = cmdval.ToString();
                    WATER_H2.Value = cmdval;
                    this.Bindings.Update();
                    break;
                case 105: //CH water
                    WAT_CH4 = cmdval.ToString();
                    //WATER_CH4.Value = cmdval;
                    this.Bindings.Update();
                    break;
                case 108: //O2 stor press
                    P002 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 111: //O2 water
                    WAT_O2 = cmdval.ToString();
                    WATER_O2.Value = cmdval;
                    this.Bindings.Update();
                    break;
                case 112: //H2 valve
                    STM000 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 113: //CO2 valve
                    STM001 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 114: //O2 elec press
                    P000 = cmdval.ToString();
                    this.Bindings.Update();
                    break;
                case 115: //H2 elec press
                    P001 = cmdval.ToString();
                    this.Bindings.Update();
                    break;


                case 200:
                    CO2VALVE = cmdval == 1 ? true : false;
                    break;
                case 201:
                    H2INVALVE = cmdval == 1 ? true : false;
                    break;
                case 202:
                    H2OUTVALVE = cmdval == 1 ? true : false;
                    break;
                case 203:
                    O2INVALVE = cmdval == 1 ? true : false;
                    break;
                case 204:
                    O2OUTVALVE = cmdval == 1 ? true : false;
                    break;
                case 205:
                    H2REFLUX = cmdval == 1 ? true : false;
                    break;
                case 206:
                    O2REFLUX = cmdval == 1 ? true : false;
                    break;
                case 207:
                    PUMP = cmdval == 1 ? true : false;
                    break;
                case 208:
                    break;
                case 209:
                    FLUSH = cmdval == 1 ? true : false;
                    break;
                case 210:
                    CHWATER = cmdval == 1 ? true : false;
                    break;
                default:
                    status.Text = "ERROR #404 - Command not found!";
                    break;
            }

            btn_CO2OUT.Background = CO2VALVE ? Green : Red;
            btn_H2IN.Background = H2INVALVE ? Green : Red;
            btn_H2OUT.Background = H2OUTVALVE ? Green : Red;
            btn_O2IN.Background = O2INVALVE ? Green : Red;
            btn_O2OUT.Background = O2OUTVALVE ? Green : Red;
            btn_H2WATER.Background = H2REFLUX ? Green : Red;
            btn_O2WATER.Background = O2REFLUX ? Green : Red;
            btn_FLUSH.Background = FLUSH ? Green : Red;
            btn_CH4WATER.Background = CHWATER ? Green : Red;
            btn_PM_000.Background = PUMP ? Green : Red;

        }



        private async void btn_02IN_Click(object sender, RoutedEventArgs e)
        {
            await Send_to_CCMR(203, O2INVALVE ? inactive : active);

        }
        private async void btn_H2IN_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(201, H2INVALVE ? inactive : active);

        }
        private async void btn_H2WATER_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(205, H2REFLUX ? inactive : active);

        }
        private async void btn_O2WATER_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(206, O2REFLUX ? inactive : active);

        }
        private async void btn_O2OUT_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(204, O2OUTVALVE ? inactive : active);

        }
        private async void btn_H2OUT_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(202, H2OUTVALVE ? inactive : active);

        }
        private async void btn_CO2OUT_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(200, CO2VALVE ? inactive : active);

        }
        private async void btn_PM000_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(207, PUMP ? inactive : active);

        }

        private async void btn_CH4WATER_Click(object sender, RoutedEventArgs e)
        {

            await Send_to_CCMR(210, CHWATER ? inactive : active);

        }

        private async void btn_FLUSH_Click(object sender, RoutedEventArgs e)
        {
            await Send_to_CCMR(209, FLUSH ? inactive : active);
        }

        
    }
}
