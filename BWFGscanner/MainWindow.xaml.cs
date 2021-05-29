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
using BWFGscanner.Core;
using System.Collections.ObjectModel;
using System.Net;
using System.Net.Sockets;

namespace BWFGscanner
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 


    public partial class MainWindow : Window
    {
        public const int SENSOR_PORT = 1025;

        private void OnSensorAdded(int id, double w, IPAddress sourceAddr)
        {

            sourceAddrArray[id] = sourceAddr;


            switch (id)
            {
                case 0:
                    tB0.Text = w.ToString();
                    break;
                case 1:
                    tB1.Text = w.ToString();
                    break;
                case 2:
                    tB2.Text = w.ToString();
                    break;
                case 3:
                    tB3.Text = w.ToString();
                    break;
                case 4:
                    tB4.Text = w.ToString();
                    break;
                case 5:
                    tB5.Text = w.ToString();
                    break;
                case 6:
                    tB6.Text = w.ToString();
                    break;
                case 7:
                    tB7.Text = w.ToString();
                    break;
                case 8:
                    tB8.Text = w.ToString();
                    break;
                case 9:
                    tB9.Text = w.ToString();
                    break;
                case 10:
                    tB10.Text = w.ToString();
                    break;
                case 11:
                    tB11.Text = w.ToString();
                    break;
                case 12:
                    tB12.Text = w.ToString();
                    break;
                case 13:
                    tB13.Text = w.ToString();
                    break;
                case 14:
                    tB14.Text = w.ToString();
                    break;
                default:

                    break;
            }
            
        }




        private IPAddress[] sourceAddrArray = new IPAddress[15];


        private Server server;


        private void Button_Click(object sender, RoutedEventArgs e)
        {
            // Получаем текст кнопки
            string s = (string)((Button)e.OriginalSource).Content;
            // Добавляем его в текстовое поле
            
            int num;
            // Пытаемся преобразовать его в число
            bool result = Int32.TryParse(s, out num);
            // Если текст - это число


            if ((result == true) & (num == 15))
            {
                for (var i = 0; i <= 14; i++)
                {
                    if (sourceAddrArray[i] != null)
                    { 
                    string message = "R";
                    UdpClient client = new UdpClient();
                    IPEndPoint ip = new IPEndPoint(sourceAddrArray[i], SENSOR_PORT);
                    byte[] bytes = Encoding.ASCII.GetBytes(message);
                    client.Send(bytes, bytes.Length, ip);
                    client.Close();
                    }
                }

                return;
            }


            if (result == true)
            {
                string message = "R";
                UdpClient client = new UdpClient();
                IPEndPoint ip = new IPEndPoint(sourceAddrArray[num], SENSOR_PORT);
                byte[] bytes = Encoding.ASCII.GetBytes(message);
                client.Send(bytes, bytes.Length, ip);
                client.Close();
            }
            

        }


            public MainWindow()
        {

            server = new Server();
            server.SensorAdded += OnSensorAdded;
            InitializeComponent();

            // Добавляем обработчик для всех кнопок на гриде
            foreach (UIElement c in LayoutRoot.Children)
            {
                if (c is Button)
                {
                    ((Button)c).Click += Button_Click;
                }
            }




            server.Start();
        }
    }
}
