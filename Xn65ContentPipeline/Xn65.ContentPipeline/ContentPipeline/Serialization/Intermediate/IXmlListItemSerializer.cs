namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal interface IXmlListItemSerializer
    {
        object Deserialize(XmlListReader list);
    }
}
