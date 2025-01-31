using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    [ContentTypeSerializer]
    internal class BoundingFrustumSerializer : ContentTypeSerializer<BoundingFrustum>
    {
        private static ContentSerializerAttribute matrixFormat = new ContentSerializerAttribute();

        static BoundingFrustumSerializer() => BoundingFrustumSerializer.matrixFormat.ElementName = "Matrix";

        protected internal override void Serialize(
          IntermediateWriter output,
          BoundingFrustum value,
          ContentSerializerAttribute format)
        {
            if (output == null)
                throw new ArgumentNullException(nameof(output));
            output.WriteObject<Matrix>(value.Matrix, BoundingFrustumSerializer.matrixFormat);
        }

        protected internal override BoundingFrustum Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          BoundingFrustum existingInstance)
        {
            if (input == null)
                throw new ArgumentNullException(nameof(input));
            if (format == null)
                throw new ArgumentNullException(nameof(format));
            return new BoundingFrustum(input.ReadObject<Matrix>(BoundingFrustumSerializer.matrixFormat));
        }
    }
}
