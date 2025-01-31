namespace Xn65.Csharp.ContentPipeline
{
    [AttributeUsage(AttributeTargets.Class)]
    public sealed class ContentSerializerCollectionItemNameAttribute : Attribute
    {
        private string collectionItemName;

        public string CollectionItemName => collectionItemName;

        public ContentSerializerCollectionItemNameAttribute(string collectionItemName)
        {
            if (string.IsNullOrEmpty(collectionItemName))
            {
                throw new ArgumentNullException("collectionItemName");
            }

            this.collectionItemName = collectionItemName;
        }
    }
}
