using System.ComponentModel;
using System.ComponentModel.Design.Serialization;
using System.Globalization;

namespace Xn65.Csharp.Framework.Design
{
    public class MathTypeConverter : ExpandableObjectConverter
    {
        protected PropertyDescriptorCollection? propertyDescriptions;

        protected bool supportStringConvert = true;

        public override bool CanConvertFrom(ITypeDescriptorContext? context, Type sourceType)
        {
            if (supportStringConvert && sourceType == typeof(string))
            {
                return true;
            }

            return base.CanConvertFrom(context, sourceType);
        }

        public override bool CanConvertTo(ITypeDescriptorContext? context, Type? destinationType)
        {
            if (destinationType == typeof(InstanceDescriptor))
            {
                return true;
            }

            return base.CanConvertTo(context, destinationType);
        }

        public override bool GetCreateInstanceSupported(ITypeDescriptorContext? context)
            => true;

        public override bool GetPropertiesSupported(ITypeDescriptorContext? context)
            => true;

        internal static T[]? ConvertToValues<T>(ITypeDescriptorContext? context, CultureInfo? culture, object value, int arrayCount, params string[] expectedParams)
        {
            if (value is string text)
            {
                var text2 = text.Trim();
                culture ??= CultureInfo.CurrentCulture;
                string[] array = text2.Split([culture.TextInfo.ListSeparator], StringSplitOptions.None);
                var array2 = new T[array.Length];
                var converter = TypeDescriptor.GetConverter(typeof(T));

                for (int i = 0; i < array2.Length; i++)
                {
                    try
                    {
                        array2[i] = (T?)converter?.ConvertFromString(context, culture, array[i]) ?? throw new InvalidOperationException();
                    }
                    catch (Exception innerException)
                    {
                        string message = string.Format(CultureInfo.CurrentCulture, SR.InvalidStringFormat, string.Join(culture.TextInfo.ListSeparator, expectedParams));
                        throw new ArgumentException(message, innerException);
                    }
                }

                if (array2.Length == arrayCount)
                {
                    return array2;
                }

                var message2 = string.Format(CultureInfo.CurrentCulture, SR.InvalidStringFormat, string.Join(culture.TextInfo.ListSeparator, expectedParams));
                throw new ArgumentException(message2);
            }

            return null;
        }

        internal static string ConvertFromValues<T>(ITypeDescriptorContext? context, CultureInfo? culture, T[] values)
        {
            culture ??= CultureInfo.CurrentCulture;
            var separator = culture.TextInfo.ListSeparator + " ";
            var converter = TypeDescriptor.GetConverter(typeof(T));
            string[] array = new string[values.Length];

            for (int i = 0; i < values.Length; i++)
            {
                array[i] = converter.ConvertToString(context, culture, values[i]) ?? throw new InvalidOperationException();
            }

            return string.Join(separator, array);
        }

        public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext? context, object value, Attribute[]? attributes)
        {
            return propertyDescriptions ?? throw new InvalidOperationException();
        }
    }
}
