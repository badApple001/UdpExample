using ProtoBuf;
using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace UDP_TEST
{
    class Client
    {
        UdpClient client = null;
        IPEndPoint ip = null;

        IPEndPoint sendIP = null;

        public void Init( string address, int port, int sendPort )
        {
            ip = new IPEndPoint(IPAddress.Parse(address), port);
            sendIP = new IPEndPoint(IPAddress.Parse(address), sendPort);
           
            client = new UdpClient(ip);
            new Thread(Recv).Start();
        }

        void Recv()
        {
            while (true)
            {
                byte[] bytRecv = client.Receive(ref ip);

                try
                {
                    Console.WriteLine("================\n\n");

                    MemoryStream ms = new MemoryStream(bytRecv);
                    BaseData bd = Serializer.Deserialize<BaseData>(ms);

                    Type t = typeof(BaseData);
                    foreach( var f in t.GetProperties())
                    {
                        Console.WriteLine($"{f.Name} : {f.GetValue(bd)}");
                    }

                    Console.WriteLine("\n\n================");
                }
                catch(Exception e)
                {
                    Console.WriteLine(e);
                }
            }
        }

        public void Send( string message )
        {
            byte[] sendbytes = Encoding.Unicode.GetBytes(message);
            client.Send(sendbytes, sendbytes.Length, sendIP);
        }

        public void Send( MemoryStream ms )
        {
            client.Send(ms.ToArray(), (int)ms.Length, sendIP);
        }

        ~Client()
        {
            Console.WriteLine("Destory Me ... ...");

        }
    }
}
