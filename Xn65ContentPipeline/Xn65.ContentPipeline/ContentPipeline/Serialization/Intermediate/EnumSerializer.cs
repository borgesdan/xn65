using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class EnumSerializer : ContentTypeSerializer
    {
        public EnumSerializer(Type targetType)
          : base(targetType)
        {
        }

        protected internal override void Serialize(
          IntermediateWriter output,
          object value,
          ContentSerializerAttribute format)
        {
            if (output == null)
                throw new ArgumentNullException(nameof(output));
            if (value == null)
                throw new ArgumentNullException(nameof(value));
            if (!IsTargetType(value))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.WrongArgumentType, (object)TargetType, (object)value.GetType()));
            output.Xml.WriteString(value.ToString());
        }

        protected internal override object Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          object existingInstance)
        {
            string str = input != null ? input.Xml.ReadContentAsString() : throw new ArgumentNullException(nameof(input));
            try
            {
                return Enum.Parse(TargetType, str);
            }
            catch (ArgumentException ex)
            {
                throw input.CreateInvalidContentException((Exception)ex, Resources.InvalidEnumValue, (object)str, (object)TargetType);
            }
        }
    }
}
