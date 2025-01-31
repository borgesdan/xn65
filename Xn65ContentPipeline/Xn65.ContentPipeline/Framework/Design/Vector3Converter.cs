using System.Collections;
using System.ComponentModel;
using System.ComponentModel.Design.Serialization;
using System.Globalization;

namespace Xn65.Csharp.Framework.Design
{
    public class Vector3Converter : MathTypeConverter
    {
        public Vector3Converter()
        {
            Type typeFromHandle = typeof(Vector3);
            PropertyDescriptorCollection propertyDescriptorCollection = new PropertyDescriptorCollection(new PropertyDescriptor[3]
            {
            new FieldPropertyDescriptor(typeFromHandle.GetField("X")),
            new FieldPropertyDescriptor(typeFromHandle.GetField("Y")),
            new FieldPropertyDescriptor(typeFromHandle.GetField("Z"))
            });
            propertyDescriptions = propertyDescriptorCollection.Sort(["X", "Y", "Z"]);
        }

        public override object? ConvertFrom(ITypeDescriptorContext? context, CultureInfo? culture, object value)
        {
            var array = ConvertToValues<float>(context, culture, value, 3, ["X", "Y", "Z"]);
            
            if (array != null)
            {
                return new Vector3(array[0], array[1], array[2]);
            }

            return base.ConvertFrom(context, culture, value);
        }

        public override object? ConvertTo(ITypeDescriptorContext? context, CultureInfo? culture, object? value, Type destinationType)
        {
            ArgumentNullException.ThrowIfNull(destinationType);

            if (destinationType == typeof(string) && value is Vector3 vector)
            {
                return ConvertFromValues(context, culture, [vector.X, vector.Y, vector.Z]);
            }

            if (destinationType == typeof(InstanceDescriptor) && value is Vector3 vector2)
            {
                var constructor = typeof(Vector3).GetConstructor(
                [
                    typeof(float),
                    typeof(float),
                    typeof(float)
                ]);

                if (constructor != null)
                {
                    return new InstanceDescriptor(constructor, new object[3] { vector2.X, vector2.Y, vector2.Z });
                }
            }

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override object CreateInstance(ITypeDescriptorContext? context, IDictionary propertyValues)
        {
            ArgumentNullException.ThrowIfNull(propertyValues, nameof(propertyValues));
            return new Vector3((float)propertyValues["X"], (float)propertyValues["Y"], (float)propertyValues["Z"]);
        }
    }
}
