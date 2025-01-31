namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class ArraySerializer<T> : ContentTypeSerializer<T[]>
    {
        private CollectionHelper? arrayHelper;
        private CollectionHelper? listHelper;

        protected internal override void Initialize(IntermediateSerializer serializer)
        {
            arrayHelper = serializer.GetCollectionHelper(typeof(T[]));
            listHelper = serializer.GetCollectionHelper(typeof(List<T>));
        }

        protected internal override void Serialize(IntermediateWriter output, T[] value, ContentSerializerAttribute format)
        {
            arrayHelper?.Serialize(output, format, value);
        }

        protected internal override T[] Deserialize(IntermediateReader input, ContentSerializerAttribute format, T[] existingInstance)
        {
            var collection = new List<T>();
            listHelper?.Deserialize(input, format, collection);
            return [.. collection];
        }

        protected internal override void ScanChildren(IntermediateSerializer serializer, ChildCallback callback, T[] value)
        {
            arrayHelper?.ScanElements(callback, value);
        }
    }
}
