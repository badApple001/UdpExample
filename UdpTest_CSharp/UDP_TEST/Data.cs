using System;
using System.Collections.Generic;
using System.Text;

[ProtoBuf.ProtoContract]
class BaseData
{
    [ProtoBuf.ProtoMember(1)]
    public int ID { get; set; }


    [ProtoBuf.ProtoMember(2)]
    public string Name { get; set; }


    [ProtoBuf.ProtoMember(3)]
    public Extend other { get; set; }
}

[ProtoBuf.ProtoContract] 
class Extend
{
    [ProtoBuf.ProtoMember(1)]
    public int Type { get; set; }


    [ProtoBuf.ProtoMember(2)]
    public string Data { get; set; }
}