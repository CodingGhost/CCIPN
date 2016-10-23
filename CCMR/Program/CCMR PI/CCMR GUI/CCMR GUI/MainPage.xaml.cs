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
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace CCMR_GUI
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        String SERIAL_DEVICE = "\\\\?\\ACPI#BCM2836#0#{86e0d1e0-8089-11d0-9ce4-08003e301f73}";
        byte cmdid;
        byte cmdval;
        /// <summary>
        /// Private variables
        /// </summary>
        ///   Dim Mysqlconn As MySqlConnection
        String query;
        MySqlCommand COMMAND;
        MySqlConnection Mysqlconn;
        private SerialDevice serialPort = null;
        DataWriter dataWriteObject = null;
        DataReader dataReaderObject = null;
        private CancellationTokenSource ReadCancellationTokenSource;


        public MainPage()
        {
            this.InitializeComponent();
            //connect();

        }
        //serial Functions
        private async void connect()
        {

            try
            {
                serialPort = await SerialDevice.FromIdAsync(SERIAL_DEVICE);
                serialPort.WriteTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.ReadTimeout = TimeSpan.FromMilliseconds(1000);
                serialPort.BaudRate = 9600;
                serialPort.Parity = SerialParity.None;
                serialPort.StopBits = SerialStopBitCount.One;
                serialPort.DataBits = 8;
                serialPort.Handshake = SerialHandshake.None;
                ReadCancellationTokenSource = new CancellationTokenSource();
            }
            catch (Exception ex)
            {
                status.Text = ex.Message;
            }
            Listen();
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
                    await WriteAsync(_text);
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
        private async Task WriteAsync(String text)
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
                    dataReaderObject.DetachStream();
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

            // Launch the task and wait
            UInt32 bytesRead = await loadAsyncTask;
            if (bytesRead > 0)
            {
                cmdid = dataReaderObject.ReadByte();
                cmdval = dataReaderObject.ReadByte();
            }
            handleCommands();
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
            status.Text = Mysql_read("new_table","COL_A")[3];
          
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



                sqlcmd = new MySqlCommand("SELECT * FROM "+ table, connect);
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
        void Mysql_write(String table, String column,int index)
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
        //real program
        private void handleCommands()
        {
            switch (cmdid)
            {
                case 1:

                    break;
                case 45:

                    break;
                default:
                    break;
            }
        }


    }




}
