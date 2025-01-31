using System.ComponentModel.DataAnnotations;

namespace Xn65.Csharp.ContentPipeline
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
    public sealed class ContentSerializerAttribute : Attribute
    {
        string collectionItemName = null!;

        public string ElementName { get; set; } = null!;
        public bool FlattenContent { get; set; }
        public bool Optional { get; set; }
        public bool AllowNull { get; set; } = true;
        public bool SharedResource { get; set; }

        public string CollectionItemName
        {
            get
            {
                if (string.IsNullOrEmpty(collectionItemName))
                {
                    return "Item";
                }

                return collectionItemName;
            }
            set
            {
                if (string.IsNullOrEmpty(value))
                {
                    throw new ArgumentNullException(nameof(value));
                }

                collectionItemName = value;
            }
        }

        public bool HasCollectionItemName => !string.IsNullOrEmpty(collectionItemName);

        public ContentSerializerAttribute Clone()
        {
            var contentSerializerAttribute = new ContentSerializerAttribute
            {
                ElementName = ElementName,
                FlattenContent = FlattenContent,
                Optional = Optional,
                AllowNull = AllowNull,
                SharedResource = SharedResource,
                CollectionItemName = CollectionItemName
            };
            return contentSerializerAttribute;
        }
    }
}
