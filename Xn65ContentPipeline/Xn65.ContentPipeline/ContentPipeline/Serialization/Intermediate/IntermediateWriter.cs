using System.Globalization;
using System.Xml;
using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    /// <summary>Provides an implementation of many of the methods of IntermediateSerializer including serialization and state tracking for shared resources and external references.</summary>
    public sealed class IntermediateWriter
    {
        private readonly IntermediateSerializer serializer;
        private readonly XmlWriter xmlWriter;
        private readonly Uri baseUri;
        private readonly Dictionary<object, bool> recurseDetector = new(new ReferenceEqualityComparer<object>());
        private readonly Dictionary<object, string> sharedResourceNames = new(new ReferenceEqualityComparer<object>());
        private readonly Queue<object> sharedResources = new();
        private readonly List<ExternalReference> externalReferences = [];

        internal IntermediateWriter(IntermediateSerializer serializer, XmlWriter xmlWriter, Uri baseUri)
        {
            this.serializer = serializer;
            this.xmlWriter = xmlWriter;
            this.baseUri = baseUri;
        }

        /// <summary>Gets the parent serializer.</summary>
        public IntermediateSerializer Serializer
            => serializer;

        /// <summary>Gets the XML output stream.</summary>
        public XmlWriter Xml
            => xmlWriter;

        /// <summary>Writes a single object to the output XML stream.</summary>
        /// <param name="value">The value to write.</param>
        /// <param name="format">The format of the XML.</param>
        public void WriteObject<T>(T value, ContentSerializerAttribute format)
            => WriteObject(value, format, Serializer.GetTypeSerializer(typeof(T)));

        /// <summary>Writes a single object to the output XML stream, using the specified type hint.</summary>
        /// <param name="value">The value to write.</param>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        public void WriteObject<T>(T value, ContentSerializerAttribute format, ContentTypeSerializer typeSerializer)
        {
            ArgumentNullException.ThrowIfNull(format);
            ArgumentNullException.ThrowIfNull(typeSerializer);

            if (!format.FlattenContent)
            {
                if (string.IsNullOrEmpty(format.ElementName))
                    throw new ArgumentException(SR.NullElementName);

                Xml.WriteStartElement(format.ElementName);
            }
            if (value == null)
            {
                if (format.FlattenContent)
                    throw new InvalidOperationException(SR.CantWriteNullInFlattenContentMode);
                Xml.WriteAttributeString("Null", "true");
            }
            else
            {
                Type type = value.GetType();

                if (type.IsSubclassOf(typeof(Type)))
                    type = typeof(Type);

                if (type != typeSerializer.BoxedTargetType)
                {
                    if (format.FlattenContent)
                        throw new InvalidOperationException(SR.CantWriteDynamicTypesInFlattenContentMode);

                    typeSerializer = Serializer.GetTypeSerializer(type);

                    Xml.WriteStartAttribute("Type");
                    WriteTypeName(typeSerializer.TargetType);
                    Xml.WriteEndAttribute();
                }

                if (recurseDetector.ContainsKey(value))
                    throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.FoundCyclicReference, value));

                recurseDetector.Add(value, true);
                typeSerializer.Serialize(this, value, format);
                recurseDetector.Remove(value);
            }

            if (format.FlattenContent)
                return;

            Xml.WriteEndElement();
        }

        /// <summary>Writes a single object to the output XML stream using the specified serializer worker.</summary>
        /// <param name="value">The value to write.</param>
        /// <param name="format">The format of the XML.</param>
        public void WriteRawObject<T>(T value, ContentSerializerAttribute format)
            => WriteRawObject(value, format, Serializer.GetTypeSerializer(typeof(T)));

        /// <summary>Writes a single object to the output XML stream as an instance of the specified type.</summary>
        /// <param name="value">The value to write.</param>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        public void WriteRawObject<T>(T value, ContentSerializerAttribute format, ContentTypeSerializer typeSerializer)
        {
            ArgumentNullException.ThrowIfNull(value);
            ArgumentNullException.ThrowIfNull(format);
            ArgumentNullException.ThrowIfNull(typeSerializer);

            if (!format.FlattenContent)
            {
                if (string.IsNullOrEmpty(format.ElementName))
                    throw new ArgumentException(SR.NullElementName);

                Xml.WriteStartElement(format.ElementName);
            }

            typeSerializer.Serialize(this, value, format);

            if (format.FlattenContent)
                return;

            Xml.WriteEndElement();
        }

        /// <summary>Adds a shared reference to the output XML and records the object to be serialized later.</summary>
        /// <param name="value">The value to write.</param>
        /// <param name="format">The format of the XML.</param>
        public void WriteSharedResource<T>(T value, ContentSerializerAttribute format)
        {
            ArgumentNullException.ThrowIfNull(format);

            if (!format.FlattenContent)
            {
                if (string.IsNullOrEmpty(format.ElementName))
                    throw new ArgumentException(SR.NullElementName);

                Xml.WriteStartElement(format.ElementName);
            }

            if (value != null)
            {
                if (!sharedResourceNames.TryGetValue(value, out string? text))
                {
                    text = "#Resource" + (sharedResourceNames.Count + 1).ToString(CultureInfo.InvariantCulture);
                    sharedResourceNames.Add(value, text);
                    sharedResources.Enqueue(value);
                }

                Xml.WriteString(text);
            }

            if (format.FlattenContent)
                return;

            Xml.WriteEndElement();
        }

        internal void WriteSharedResources()
        {
            if (sharedResources.Count <= 0)
                return;

            Xml.WriteStartElement("Resources");

            var format = new ContentSerializerAttribute
            {
                ElementName = "Resource",
                FlattenContent = true
            };

            while (sharedResources.Count > 0)
            {
                object key = sharedResources.Dequeue();
                Type type = key.GetType();
                ContentTypeSerializer typeSerializer = Serializer.GetTypeSerializer(type);
                Xml.WriteStartElement("Resource");
                Xml.WriteAttributeString("ID", sharedResourceNames[key]);
                Xml.WriteStartAttribute("Type");
                WriteTypeName(type);
                Xml.WriteEndAttribute();
                WriteRawObject(key, format, typeSerializer);
                Xml.WriteEndElement();
            }

            Xml.WriteEndElement();
        }

        /// <summary>Adds an external reference to the output XML, and records the filename to be serialized later.</summary>
        /// <param name="value">The external reference to add.</param>
        public void WriteExternalReference<T>(ExternalReference<T> value)
        {
            if (value == null || value.Filename == null)
                return;

            var relativePath = PathUtils.GetRelativePath(baseUri, value.Filename);
            string? str = null;

            foreach (ExternalReference externalReference in externalReferences)
            {
                if (externalReference.TargetType == typeof(T) && externalReference.Filename == relativePath)
                {
                    str = externalReference.ID;
                    break;
                }
            }
            if (str == null)
            {
                str = "#External" + (externalReferences.Count + 1).ToString(CultureInfo.InvariantCulture);
                ExternalReference externalReference;
                externalReference.TargetType = typeof(T);
                externalReference.Filename = relativePath;
                externalReference.ID = str;
                externalReferences.Add(externalReference);
            }

            Xml.WriteElementString("Reference", str);
        }

        internal void WriteExternalReferences()
        {
            if (externalReferences.Count <= 0)
                return;

            Xml.WriteStartElement("ExternalReferences");

            foreach (ExternalReference externalReference in externalReferences)
            {
                Xml.WriteStartElement("ExternalReference");
                Xml.WriteAttributeString("ID", externalReference.ID);
                Xml.WriteStartAttribute("TargetType");
                WriteTypeName(externalReference.TargetType);
                Xml.WriteEndAttribute();
                Xml.WriteString(externalReference.Filename);
                Xml.WriteEndElement();
            }

            Xml.WriteEndElement();
        }

        /// <summary>Writes a managed type descriptor to the XML output stream.</summary>
        /// <param name="type">The type.</param>
        public void WriteTypeName(Type type)
        {
            ArgumentNullException.ThrowIfNull(type);
            Serializer.typeNameHelper.WriteTypeName(Xml, type);
        }

        private struct ExternalReference
        {
            public Type TargetType;
            public string Filename;
            public string ID;
        }
    }
}
