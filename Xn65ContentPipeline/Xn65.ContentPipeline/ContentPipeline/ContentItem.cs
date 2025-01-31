namespace Xn65.Csharp.ContentPipeline
{
    /// <summary>Provides properties that define various aspects of content stored using the intermediate file format of the XNA Framework.</summary>
    public class ContentItem
    {
        private string name;
        private ContentIdentity identity;
        private OpaqueDataDictionary opaqueData = new OpaqueDataDictionary();

        /// <summary>Gets or sets the name of the content item.</summary>
        [ContentSerializer(Optional = true)]
        public string Name
        {
            get => name;
            set => name = value;
        }

        /// <summary>Gets or sets the identity of the content item.</summary>
        [ContentSerializer(Optional = true)]
        public ContentIdentity Identity
        {
            get => identity;
            set => identity = value;
        }

        /// <summary>Gets the opaque data of the content item.</summary>
        [ContentSerializer(Optional = true)]
        public OpaqueDataDictionary OpaqueData => opaqueData;
    }
}
