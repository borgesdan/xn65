using System.Collections;
using Xn65.Csharp.ContentPipeline;
using Xn65.Csharp.Serialization.Intermediate;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class ContentTypeSerializerFactory : TypeHandlerFactory<ContentTypeSerializer>
    {
        protected override Type AttributeType => typeof(ContentTypeSerializerAttribute);

        protected override Type GenericType => typeof(ContentTypeSerializer<>);

        public ContentTypeSerializer CreateSerializer(Type type)
        {
            ContentTypeSerializer handler = CreateHandler(type);
            if (handler != null)
                return handler;
            if (type.IsArray)
                return InstantiateArraySerializer(type);
            if (type.IsEnum)
                return new EnumSerializer(type);
            if (typeof(IDictionary).IsAssignableFrom(type) && !CollectionUtils.IsCollection(type, true))
                return (ContentTypeSerializer)new NonGenericIDictionarySerializer(type);
            return typeof(IList).IsAssignableFrom(type) && !CollectionUtils.IsCollection(type, true) ? (ContentTypeSerializer)new NonGenericIListSerializer(type) : (ContentTypeSerializer)new ReflectiveSerializer(type);
        }

        private static ContentTypeSerializer InstantiateArraySerializer(Type type) => type.GetArrayRank() == 1 ? (ContentTypeSerializer)Activator.CreateInstance(typeof(ArraySerializer<>).MakeGenericType(type.GetElementType())) : throw new RankException(Resources.CantSerializeMultidimensionalArrays);
    }
}
