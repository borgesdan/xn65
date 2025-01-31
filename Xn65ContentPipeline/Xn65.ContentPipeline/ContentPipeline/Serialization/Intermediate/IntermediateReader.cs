using System.Globalization;
using System.Xml;
using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    /// <summary>Provides an implementation of many of the methods of IntermediateSerializer. Deserializes and tracks state for shared resources and external references.</summary>
    public sealed class IntermediateReader
    {
        private readonly IntermediateSerializer serializer;
        private XmlReader xmlReader;
        private readonly Uri? baseUri;
        private readonly Dictionary<string, List<Action<object>>> sharedResourceFixups = [];
        private readonly List<ExternalReferenceFixup> externalReferenceFixups = [];

        internal IntermediateReader(
          IntermediateSerializer serializer,
          XmlReader xmlReader,
          Uri? baseUri)
        {
            this.serializer = serializer;
            this.xmlReader = xmlReader;
            this.baseUri = baseUri;
        }

        /// <summary>Gets the parent serializer.</summary>
        public IntermediateSerializer Serializer
            => serializer;

        /// <summary>Gets the XML input stream.</summary>
        public XmlReader Xml
            => xmlReader;

        /// <summary>Reads a single object from the input XML stream.</summary>
        /// <param name="format">The format expected by the type serializer.</param>
        public T? ReadObject<T>(ContentSerializerAttribute format)
            => ReadObjectInternal<T>(format, Serializer.GetTypeSerializer(typeof(T)), null);

        /// <summary>Reads a single object from the input XML stream, optionally specifying an existing instance to receive the data.</summary>
        /// <param name="format">The format expected by the type serializer.</param>
        /// <param name="existingInstance">The object receiving the data, or null if a new instance should be created.</param>
        public T? ReadObject<T>(ContentSerializerAttribute format, T existingInstance)
            => ReadObjectInternal<T>(format, Serializer.GetTypeSerializer(typeof(T)), existingInstance);

        /// <summary>Reads a single object from the input XML stream, using the specified type hint.</summary>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        public T? ReadObject<T>(ContentSerializerAttribute format, ContentTypeSerializer typeSerializer)
            => ReadObjectInternal<T>(format, typeSerializer, null);

        /// <summary>Reads a single object from the input XML stream using the specified type hint, optionally specifying an existing instance to receive the data.</summary>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        /// <param name="existingInstance">The object receiving the data, or null if a new instance should be created.</param>
        public T? ReadObject<T>(
          ContentSerializerAttribute format,
          ContentTypeSerializer typeSerializer,
          T? existingInstance)
        {
            return ReadObjectInternal<T>(format, typeSerializer, existingInstance);
        }

        private T? ReadObjectInternal<T>(
          ContentSerializerAttribute format,
          ContentTypeSerializer typeSerializer,
          object? existingInstance)
        {
            ArgumentNullException.ThrowIfNull(format);
            ArgumentNullException.ThrowIfNull(typeSerializer);

            if (!format.FlattenContent)
            {
                if (!MoveToElement(format.ElementName))
                    throw CreateInvalidContentException(SR.ElementNotFound, format.ElementName);

                var attribute = Xml.GetAttribute("Null");

                if (attribute != null && XmlConvert.ToBoolean(attribute))
                {
                    if (!format.AllowNull)
                        throw CreateInvalidContentException(SR.NullElementNotAllowed, format.ElementName);

                    Xml.Skip();

                    return default;
                }
                if (Xml.MoveToAttribute("Type"))
                {
                    var type = ReadTypeName();

                    typeSerializer = typeSerializer.IsTargetType(type)
                        ? Serializer.GetTypeSerializer(type)
                        : throw CreateInvalidContentException(SR.WrongXmlType, typeSerializer.TargetType, type);

                    Xml.MoveToElement();
                }
                else if (typeSerializer.TargetType == typeof)
                    throw CreateInvalidContentException(SR.UnknownDeserializationType);
            }

            return ReadRawObjectInternal<T>(format, typeSerializer, existingInstance);
        }

        /// <summary>Reads a single object from the input XML stream as an instance of the specified type, optionally specifying an existing instance to receive the data.</summary>
        /// <param name="format">The format of the XML.</param>
        public T ReadRawObject<T>(ContentSerializerAttribute format)
            => ReadRawObjectInternal<T>(format, Serializer.GetTypeSerializer(typeof(T)), null);

        /// <summary>Reads a single object from the input XML stream, as an instance of the specified type.</summary>
        /// <param name="format">The object.</param>
        /// <param name="existingInstance">The object receiving the data, or null if a new instance should be created.</param>
        public T ReadRawObject<T>(ContentSerializerAttribute format, T existingInstance)
            => ReadRawObjectInternal<T>(format, Serializer.GetTypeSerializer(typeof(T)), existingInstance);

        /// <summary>Reads a single object from the input XML stream as an instance of the specified type using the specified type hint.</summary>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        public T ReadRawObject<T>(
          ContentSerializerAttribute format,
          ContentTypeSerializer typeSerializer)
        {
            return ReadRawObjectInternal<T>(format, typeSerializer, null);
        }

        /// <summary>Reads a single object from the input XML stream as an instance of the specified type using the specified type hint, optionally specifying an existing instance to receive the data.</summary>
        /// <param name="format">The format of the XML.</param>
        /// <param name="typeSerializer">The type serializer.</param>
        /// <param name="existingInstance">The object receiving the data, or null if a new instance should be created.</param>
        public T ReadRawObject<T>(
          ContentSerializerAttribute format,
          ContentTypeSerializer typeSerializer,
          T existingInstance)
        {
            return ReadRawObjectInternal<T>(format, typeSerializer, existingInstance);
        }

        private T ReadRawObjectInternal<T>(
          ContentSerializerAttribute format,
          ContentTypeSerializer typeSerializer,
          object? existingInstance)
        {
            ArgumentNullException.ThrowIfNull(format);
            ArgumentNullException.ThrowIfNull(typeSerializer);

            object obj1;

            if (format.FlattenContent)
            {
                Xml.MoveToContent();
                obj1 = typeSerializer.Deserialize(this, format, existingInstance);
            }
            else
            {
                if (!MoveToElement(format.ElementName))
                    throw CreateInvalidContentException(SR.ElementNotFound, format.ElementName);

                XmlReader xmlReader = this.xmlReader;

                if (Xml.IsEmptyElement)
                    this.xmlReader = FakeEmptyElementReader.Instance;

                xmlReader.ReadStartElement();
                obj1 = typeSerializer.Deserialize(this, format, existingInstance);

                if (this.xmlReader == xmlReader)
                    this.xmlReader.ReadEndElement();
                else
                    this.xmlReader = xmlReader;
            }

            if (obj1 == null)
                throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.DeserializerReturnedNull, typeSerializer.GetType(), typeSerializer.TargetType));

            if (existingInstance != null && obj1 != existingInstance)
                throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.DeserializerConstructedNewInstance, typeSerializer.GetType(), typeSerializer.TargetType));

            return obj1 is T obj2
                ? obj2
                : throw CreateInvalidContentException(SR.WrongXmlType, typeof(T), obj1.GetType());
        }

        /// <summary>Reads a shared resource ID and records it for subsequent operations.</summary>
        /// <param name="format">The format of the XML.</param>
        /// <param name="fixup">The fixup operation to perform.</param>
        public void ReadSharedResource<T>(ContentSerializerAttribute format, Action<T> fixup)
        {
            ArgumentNullException.ThrowIfNull(format);
            ArgumentNullException.ThrowIfNull(fixup);
            string key;

            if (format.FlattenContent)
            {
                key = Xml.ReadContentAsString();
            }
            else
            {
                if (!MoveToElement(format.ElementName))
                    throw CreateInvalidContentException(SR.ElementNotFound, format.ElementName);

                key = Xml.ReadElementContentAsString();
            }
            if (string.IsNullOrEmpty(key))
            {
                if (!format.AllowNull)
                    throw CreateInvalidContentException(SR.NullElementNotAllowed, format.ElementName);
            }
            else
            {
                if (!sharedResourceFixups.ContainsKey(key))
                    sharedResourceFixups.Add(key, []);

                sharedResourceFixups[key].Add(value =>
                {
                    if (value is not T obj2)
                        throw CreateInvalidContentException(SR.WrongSharedResourceType, typeof(T), value.GetType());

                    fixup(obj2);
                });
            }
        }

        internal void ReadSharedResources()
        {
            if (MoveToElement("Resources"))
            {
                var dictionary = new Dictionary<string, object>();
                var format = new ContentSerializerAttribute
                {
                    ElementName = "Resource"
                };
                Xml.ReadStartElement();

                while (MoveToElement("Resource"))
                {
                    var attribute = Xml.GetAttribute("ID");

                    if (string.IsNullOrEmpty(attribute))
                        throw CreateInvalidContentException(SR.MissingAttribute, "ID");

                    if (dictionary.ContainsKey(attribute))
                        throw CreateInvalidContentException(SR.DuplicateID, attribute);

                    var obj = ReadObject<object>(format);
                    dictionary.Add(attribute, obj!);
                }

                Xml.ReadEndElement();

                foreach (KeyValuePair<string, List<Action<object>>> sharedResourceFixup in sharedResourceFixups)
                {

                    if (!dictionary.TryGetValue(sharedResourceFixup.Key, out object? obj))
                        throw CreateInvalidContentException(SR.MissingResource, sharedResourceFixup.Key);

                    foreach (Action<object> action in sharedResourceFixup.Value)
                        action(obj);
                }
            }
            else if (sharedResourceFixups.Count > 0)
                throw CreateInvalidContentException(SR.ElementNotFound, "Resources");
        }

        /// <summary>Reads an external reference ID and records it for subsequent operations.</summary>
        /// <param name="existingInstance">The object receiving the data, or null if a new instance of the object should be created.</param>
        public void ReadExternalReference<T>(ExternalReference<T> existingInstance)
        {
            ArgumentNullException.ThrowIfNull(existingInstance);

            if (!MoveToElement("Reference"))
                return;

            var str = Xml.ReadElementContentAsString();

            if (string.IsNullOrEmpty(str))
                return;

            ExternalReferenceFixup externalReferenceFixup;
            externalReferenceFixup.ID = str;
            externalReferenceFixup.TargetType = typeof(T);
            externalReferenceFixup.Fixup = filename => existingInstance.Filename = filename;
            externalReferenceFixups.Add(externalReferenceFixup);
        }

        internal void ReadExternalReferences()
        {
            if (MoveToElement("ExternalReferences"))
            {
                var dictionary1 = new Dictionary<string, Type>();
                var dictionary2 = new Dictionary<string, string>();
                Xml.ReadStartElement();

                while (MoveToElement("ExternalReference"))
                {
                    var attribute = Xml.GetAttribute("ID");

                    if (string.IsNullOrEmpty(attribute))
                        throw CreateInvalidContentException(SR.MissingAttribute, "ID");

                    if (dictionary1.ContainsKey(attribute))
                        throw CreateInvalidContentException(SR.DuplicateID, attribute);

                    if (!Xml.MoveToAttribute("TargetType"))
                        throw CreateInvalidContentException(SR.MissingAttribute, "TargetType");

                    dictionary1.Add(attribute, ReadTypeName());
                    Xml.MoveToElement();
                    var absolutePath = PathUtils.GetAbsolutePath(baseUri, Xml.ReadElementString());
                    dictionary2.Add(attribute, absolutePath);
                }

                Xml.ReadEndElement();

                foreach (ExternalReferenceFixup externalReferenceFixup in externalReferenceFixups)
                {
                    if (!dictionary1.TryGetValue(externalReferenceFixup.ID, out Type? value))
                        throw CreateInvalidContentException(SR.MissingExternalReference, externalReferenceFixup.ID);

                    if (value != externalReferenceFixup.TargetType)
                        throw CreateInvalidContentException(SR.WrongExternalReferenceType, externalReferenceFixup.ID);

                    externalReferenceFixup.Fixup(dictionary2[externalReferenceFixup.ID]);
                }
            }
            else if (externalReferenceFixups.Count > 0)
                throw CreateInvalidContentException(SR.ElementNotFound, "ExternalReferences");
        }

        /// <summary>Reads and decodes a type descriptor from the XML input stream.</summary>
        public Type ReadTypeName()
            => Serializer.typeNameHelper.ParseTypeName(Xml, Xml.ReadContentAsString());

        /// <summary>Moves to the specified element if the element name exists.</summary>
        /// <param name="elementName">The element name.</param>
        public bool MoveToElement(string elementName)
        {
            if (string.IsNullOrEmpty(elementName))
                throw new ArgumentException(SR.NullElementName);

            return Xml.MoveToContent() == XmlNodeType.Element && Xml.Name == elementName;
        }

        internal Exception CreateInvalidContentException(string message, params object[] messageArgs)
            => CreateInvalidContentException(null, message, messageArgs);

        internal Exception CreateInvalidContentException(
          Exception? innerException,
          string message,
          params object[] messageArgs)
        {
            var contentIdentity = new ContentIdentity
            {
                SourceFilename = !(baseUri != null) ? Xml.BaseURI : baseUri.LocalPath
            };

            if (Xml is IXmlLineInfo xml)
                contentIdentity.FragmentIdentifier = string.Format(CultureInfo.InvariantCulture, "{0},{1}", xml.LineNumber, xml.LinePosition);

            return new InvalidContentException(string.Format(CultureInfo.CurrentCulture, message, messageArgs), contentIdentity, innerException);
        }

        private struct ExternalReferenceFixup
        {
            public string ID;
            public Type TargetType;
            public Action<string> Fixup;
        }
    }
}
