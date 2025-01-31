using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal abstract class XmlListItemSerializer<T> : ContentTypeSerializer<T>, IXmlListItemSerializer
    {
        protected XmlListItemSerializer()
        {
        }

        protected XmlListItemSerializer(string xmlTypeName)
          : base(xmlTypeName)
        {
        }

        protected internal abstract T Deserialize(XmlListReader input);

        object IXmlListItemSerializer.Deserialize(XmlListReader list)
            => Deserialize(list);

        protected internal override T Deserialize(IntermediateReader input, ContentSerializerAttribute format, T existingInstance)
        {
            var input1 = input != null ? new XmlListReader(input) : throw new ArgumentNullException(nameof(input));

            T obj = Deserialize(input1);

            if (!input1.AtEnd)
                throw input.CreateInvalidContentException(SR.TooManyEntriesInXmlList);

            return obj;
        }
    }
}
