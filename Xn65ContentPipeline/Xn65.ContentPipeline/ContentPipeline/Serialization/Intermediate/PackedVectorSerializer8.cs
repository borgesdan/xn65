using System.Globalization;
using Xn65.Csharp.Framework.Graphics;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class PackedVectorSerializer8<T> : XmlListItemSerializer<T> where T : struct, IPackedVector<byte>
    {
        protected internal override void Serialize(IntermediateWriter output, T value, ContentSerializerAttribute format)
        {
            ArgumentNullException.ThrowIfNull(output);
            output.Xml.WriteString(value.PackedValue.ToString("X2", CultureInfo.InvariantCulture));
        }

        protected internal override T Deserialize(XmlListReader input)
        {
            ArgumentNullException.ThrowIfNull(input);
            var obj = default(T);
            obj.PackedValue = byte.Parse(input.ReadString(), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            return obj;
        }
    }

    internal class PackedVectorSerializer16<T> : XmlListItemSerializer<T> where T : struct, IPackedVector<ushort>
    {
        protected internal override void Serialize(IntermediateWriter output, T value, ContentSerializerAttribute format)
        {
            ArgumentNullException.ThrowIfNull(output);
            output.Xml.WriteString(value.PackedValue.ToString("X4", CultureInfo.InvariantCulture));
        }

        protected internal override T Deserialize(XmlListReader input)
        {
            ArgumentNullException.ThrowIfNull(input);
            T obj = default;
            obj.PackedValue = ushort.Parse(input.ReadString(), NumberStyles.HexNumber, CultureInfo.InvariantCulture);
            return obj;
        }
    }
}
