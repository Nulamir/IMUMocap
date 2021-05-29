using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Media.Media3D;
using System.Windows.Threading;
using System.Diagnostics;
using System.Net;
using System.IO.MemoryMappedFiles;

namespace BWFGscanner.Core
{
    public class Server
    {
        // used for both udp and tcp (websocket) server.
        public const int DATA_PORT = 5555;

        // the synchronisation context that was used when the server was started.
        // used to invoke events on the main thread
        private Dispatcher startedDispatcher;

        private Task udpListenerTask;

        public event Action<int, double, IPAddress> SensorAdded;

        
        public void Start()
        {
            if (udpListenerTask != null)
                throw new InvalidOperationException("Task is already running");

            // start udp listener
            startedDispatcher = Dispatcher.CurrentDispatcher;
            udpListenerTask = UdpListenAsync();


            int size = 4 * 6 * 15;

            byte[] zero = new byte[15 * 6 * 4 ]; 

            MemoryMappedFile sharedMemory = MemoryMappedFile.CreateOrOpen("MemoryFile", size);
            //Создаем объект для записи в разделяемый участок памяти
            using (MemoryMappedViewAccessor writer = sharedMemory.CreateViewAccessor(0, size))
            {
                //запись в разделяемую память
                //запись размера с нулевого байта в разделяемой памяти
                // writer.Write(0, size);
                //запись сообщения с четвертого байта в разделяемой памяти
                writer.WriteArray<byte>(0, zero, 0, 360);
            }


        }

      

        private Task UdpListenAsync()
        {
            var task = new Task(async () =>
            {
                UdpClient listener = new UdpClient(DATA_PORT);

                while (true)
                {
                    UdpReceiveResult result = await listener.ReceiveAsync();

                    int sensorId = BitConverter.ToInt32(result.Buffer, 0);

                    var quat = new Quaternion();
                    int i = 1;
                    quat.W = BitConverter.ToInt32(result.Buffer, i++ * sizeof(int));

                    // quat.X = BitConverter.ToInt32(result.Buffer, i++ * sizeof(int));
                    // quat.Y = BitConverter.ToInt32(result.Buffer, i++ * sizeof(int));
                    // quat.Z = BitConverter.ToInt32(result.Buffer, i++ * sizeof(int));
                    // uint timestamp = BitConverter.ToUInt32(result.Buffer, i++ * sizeof(int));

                    // quat.Normalize();

                    //var value = new SensorValue(quat, accel, gyro, DateTime.Now, timestamp);

                    //var sourceAddr = result.RemoteEndPoint.Address;

                    // TODO: check performance. this creates a closure on every iteration?
                    int size = 4 * 6 * 15;

                    MemoryMappedFile sharedMemory = MemoryMappedFile.CreateOrOpen("MemoryFile", size);
                    //Создаем объект для записи в разделяемый участок памяти
                    using (MemoryMappedViewAccessor writer = sharedMemory.CreateViewAccessor(0, size))
                    {
                        //запись в разделяемую память
                        //запись размера с нулевого байта в разделяемой памяти
                       // writer.Write(0, size);
                        //запись сообщения с четвертого байта в разделяемой памяти
                        writer.WriteArray<byte>(sensorId * 4 * 6, result.Buffer, 0, 24);
                    }

                    var sourceAddr = result.RemoteEndPoint.Address;

                    startedDispatcher.BeginInvoke(SensorAdded, sensorId, quat.W, sourceAddr);


                    

                  //  return ;
                    
                }
            }, TaskCreationOptions.LongRunning);
            task.Start();

            return task;
        }
    }
}


