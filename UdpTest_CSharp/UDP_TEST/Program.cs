using ProtoBuf;
using System;
using System.IO;

namespace UDP_TEST
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            var ips = File.ReadAllText("./config.txt").Split(',');
            var recvIP = Convert.ToInt32(ips[0]);
            var sendIP = Convert.ToInt32(ips[1]);

            var client = new Client();

            client.Init("127.0.0.1", recvIP, sendIP);


            int index = 0;
            while (true)
            {

                var str = Console.ReadLine();

                var data = new BaseData();
                data.ID = index;
                data.Name = str;
                data.other = null;

                MemoryStream ms = new MemoryStream();
                Serializer.Serialize<BaseData>(ms, data);

                client.Send(ms);

                ++index;
            }
        
            
        }
    }
}
