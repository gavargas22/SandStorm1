using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.IO.Ports;
using System.Threading;

namespace SandStorm1_Commander
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public int baudRate = 115200;
        public string comPortName;
        SerialPort sandStormSerialPort = new SerialPort();
        public bool continueReadingSerialPort = true;



        public MainWindow()
        {
            InitializeComponent();
            // Get all the ports available.
            string[] serialPortsAvailable = SerialPort.GetPortNames();
            //List all of the serial ports to initiate a connection
            serialPortsList.ItemsSource = serialPortsAvailable;
            
        }

        // A function to connect to the Serial port using the parameters desired.
        public void connectToSerialPort(string port)
        {
            // Connect with the parameters.
            sandStormSerialPort.PortName = comPortName;
            sandStormSerialPort.BaudRate = baudRate;
            sandStormSerialPort.Open();

            comPortConnectButton.IsEnabled = false;
            comPortCloseButton.IsEnabled = true;

            while (continueReadingSerialPort == true)
            {
                string data = sandStormSerialPort.ReadLine();
                Console.WriteLine(data);
            }
        }

        private void connectToSelectedComPort(object sender, RoutedEventArgs e)
        {
            //Console.WriteLine("Setting the Port and connecting...");
            connectToSerialPort(comPortName);
        }

        private void serialPortsList_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            comPortName = serialPortsList.SelectedValue.ToString();
            Console.WriteLine(comPortName);

        }

        private void closeSelectedComPort(object sender, RoutedEventArgs e)
        {
            sandStormSerialPort.Close();
            comPortConnectButton.IsEnabled = true;
            this.IsEnabled = false;
        }


        
    }

    
}
