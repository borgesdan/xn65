using System.Xml;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    [ContentTypeSerializer]
    internal class BooleanSerializer : XmlListItemSerializer<bool>
    {
        public BooleanSerializer() : base("bool")
        {
        }

        protected internal override void Serialize(IntermediateWriter output, bool value, ContentSerializerAttribute format)
        {
            ArgumentNullException.ThrowIfNull(output);
            output.Xml.WriteString(XmlConvert.ToString(value));
        }

        protected internal override bool Deserialize(XmlListReader input) 
            => input != null ? XmlConvert.ToBoolean(input.ReadString()) : throw new ArgumentNullException(nameof(input));
    }
}
