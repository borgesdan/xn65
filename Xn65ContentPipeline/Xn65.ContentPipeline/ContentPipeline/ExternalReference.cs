namespace Xn65.Csharp.ContentPipeline
{
    /// <summary>Specifies external references to a data file for the content item.</summary>
    public sealed class ExternalReference<T> : ContentItem
    {
        private string filename;

        /// <summary>Initializes a new instance of ExternalReference.</summary>
        public ExternalReference()
        {
        }

        /// <summary>Initializes a new instance of ExternalReference.</summary>
        /// <param name="filename">The name of the referenced file.</param>
        public ExternalReference(string filename)
        {
            if (filename == null)
                return;
            this.filename = PathUtils.GetFullPath(filename);
        }

        /// <summary>Initializes a new instance of ExternalReference, specifying the file path relative to another content item.</summary>
        /// <param name="filename">The name of the referenced file.</param>
        /// <param name="relativeToContent">The content that the path specified in filename is relative to.</param>
        public ExternalReference(string filename, ContentIdentity relativeToContent)
        {
            switch (filename)
            {
                case null:
                    break;
                case "":
                    throw new ArgumentNullException(nameof(filename));
                default:
                    if (relativeToContent == null)
                        throw new ArgumentNullException(nameof(relativeToContent));
                    if (string.IsNullOrEmpty(relativeToContent.SourceFilename))
                        throw new ArgumentNullException("relativeToContent.SourceFilename");
                    this.filename = PathUtils.GetFullPath(Path.Combine(Path.GetDirectoryName(relativeToContent.SourceFilename), filename));
                    break;
            }
        }

        /// <summary>Gets and sets the file name of an ExternalReference.</summary>
        public string Filename
        {
            get => filename;
            set
            {
                if (value != null)
                    PathUtils.ThrowIfPathNotAbsolute(value);
                filename = value;
            }
        }
    }
}
