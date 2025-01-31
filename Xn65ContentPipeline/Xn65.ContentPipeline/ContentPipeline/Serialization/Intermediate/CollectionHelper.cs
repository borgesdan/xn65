using System.Collections;
using System.Globalization;
using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class CollectionHelper
    {
        private Type targetType;
        private ContentTypeSerializer contentSerializer;
        private ReflectionEmitUtils.GetterMethod countPropertyGetter;
        private ReflectionEmitUtils.SetterMethod addToCollection;

        internal CollectionHelper(IntermediateSerializer serializer, Type type)
        {
            targetType = type;
            Type type1 = CollectionUtils.CollectionElementType(type, false);
            contentSerializer = !(type1 == null) ? serializer.GetTypeSerializer(type1) : throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.NotACollectionType, (object)type));
            Type collectionType = typeof(ICollection<>).MakeGenericType(type1);
            countPropertyGetter = ReflectionEmitUtils.GenerateGetter(collectionType.GetProperty("Count"));
            addToCollection = ReflectionEmitUtils.GenerateAddToCollection(collectionType, type1);
        }

        public void Serialize(
          IntermediateWriter output,
          ContentSerializerAttribute format,
          object collection)
        {
            if (output == null)
                throw new ArgumentNullException(nameof(output));
            if (format == null)
                throw new ArgumentNullException(nameof(format));
            ValidateCollectionType(collection);
            IEnumerable enumerable = (IEnumerable)collection;
            if (contentSerializer is IXmlListItemSerializer)
            {
                ContentSerializerAttribute format1 = new ContentSerializerAttribute();
                format1.FlattenContent = true;
                bool flag = true;
                foreach (object obj in enumerable)
                {
                    if (flag)
                        flag = false;
                    else
                        output.Xml.WriteWhitespace(" ");
                    output.WriteRawObject(obj, format1, contentSerializer);
                }
            }
            else
            {
                ContentSerializerAttribute format2 = new ContentSerializerAttribute();
                format2.ElementName = format.CollectionItemName;
                foreach (object obj in enumerable)
                    output.WriteObject(obj, format2, contentSerializer);
            }
        }

        public void Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          object collection)
        {
            if (input == null)
                throw new ArgumentNullException(nameof(input));
            if (format == null)
                throw new ArgumentNullException(nameof(format));
            ValidateCollectionType(collection);
            if (this.contentSerializer is IXmlListItemSerializer contentSerializer)
            {
                XmlListReader list = new XmlListReader(input);
                while (!list.AtEnd)
                    this.addToCollection(collection, contentSerializer.Deserialize(list));
            }
            else
            {
                ContentSerializerAttribute format1 = new ContentSerializerAttribute();
                format1.ElementName = format.CollectionItemName;
                while (input.MoveToElement(format.CollectionItemName))
                    this.addToCollection(collection, input.ReadObject<object>(format1, this.contentSerializer));
            }
        }

        public bool ObjectIsEmpty(object collection) => (int)this.countPropertyGetter(collection) == 0;

        public void ScanElements(ContentTypeSerializer.ChildCallback callback, object collection)
        {
            ValidateCollectionType(collection);
            foreach (object obj in (IEnumerable)collection)
            {
                if (obj != null)
                    callback(contentSerializer, obj);
            }
        }

        private void ValidateCollectionType(object collection)
        {
            if (collection == null)
                throw new ArgumentNullException(nameof(collection));
            if (!targetType.IsAssignableFrom(collection.GetType()))
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.WrongArgumentType, (object)targetType, (object)collection.GetType()));
        }
    }
}
