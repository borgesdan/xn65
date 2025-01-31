using System.Globalization;
using System.Xml;
using Xn65.Csharp.ContentPipeline;
using Xn65.Csharp.Serialization.Intermediate;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    /// <summary>Provides methods for reading and writing XNA intermediate XML format.</summary>
    public sealed class IntermediateSerializer
    {
        private static IntermediateSerializer? singletonInstance;
        private readonly ContentTypeSerializerFactory typeSerializerFactory = new();
        private readonly Dictionary<Type, ContentTypeSerializer> serializerInstances = [];
        internal TypeNameHelper typeNameHelper = new();
        private readonly Dictionary<Type, CollectionHelper> collectionHelpers = [];

        private IntermediateSerializer()
        {
            foreach (ContentTypeSerializer serializer in typeSerializerFactory.Initialize())
                AddTypeSerializer(serializer);
            foreach (ContentTypeSerializer contentTypeSerializer in new List<ContentTypeSerializer>(serializerInstances.Values))
                contentTypeSerializer.Initialize(this);
        }

        private static IntermediateSerializer SingletonInstance
        {
            get
            {
                singletonInstance ??= new IntermediateSerializer();
                return singletonInstance;
            }
        }

        /// <summary>Serializes an object into an intermediate XML file.</summary>
        /// <param name="output">The output XML stream.</param>
        /// <param name="value">The object to be serialized.</param>
        /// <param name="referenceRelocationPath">Final name of the output file, used to relative encode external reference filenames.</param>
        public static void Serialize<T>(XmlWriter output, T value, string referenceRelocationPath)
        {
            ArgumentNullException.ThrowIfNull(output);
            ArgumentNullException.ThrowIfNull(value);

            var uri = RelocationPathToUri(referenceRelocationPath);

            var writer = new IntermediateWriter(SingletonInstance, output, uri);

            output.WriteStartElement("XnaContent");
            new XmlNamespaces(writer).WriteNamespaces(value);

            writer.WriteObject<object>(value, new ContentSerializerAttribute()
            {
                ElementName = "Asset"
            });
            writer.WriteSharedResources();
            writer.WriteExternalReferences();
            output.WriteEndElement();
        }

        /// <summary>Deserializes an intermediate XML file into a managed object.</summary>
        /// <param name="input">Intermediate XML file.</param>
        /// <param name="referenceRelocationPath">Final name of the output file used to relative encode external reference filenames.</param>
        public static T? Deserialize<T>(XmlReader input, string referenceRelocationPath)
        {
            ArgumentNullException.ThrowIfNull(input);
            var uri = RelocationPathToUri(referenceRelocationPath);
            var intermediateReader = new IntermediateReader(SingletonInstance, input, uri);

            try
            {
                if (!intermediateReader.MoveToElement("XnaContent"))
                    throw intermediateReader.CreateInvalidContentException(SR.NotIntermediateXml);

                input.ReadStartElement();
                var obj = intermediateReader.ReadObject<T>(new ContentSerializerAttribute()
                {
                    ElementName = "Asset"
                });
                intermediateReader.ReadSharedResources();
                intermediateReader.ReadExternalReferences();
                input.ReadEndElement();
                return obj;
            }
            catch (XmlException ex)
            {
                throw intermediateReader.CreateInvalidContentException(ex, SR.XmDeserializelException, ex.Message);
            }
            catch (FormatException ex)
            {
                throw intermediateReader.CreateInvalidContentException(ex, SR.XmDeserializelException, ex.Message);
            }
            catch (OverflowException ex)
            {
                throw intermediateReader.CreateInvalidContentException(ex, SR.XmDeserializelException, ex.Message);
            }
            catch (ArgumentException ex)
            {
                throw intermediateReader.CreateInvalidContentException(ex, SR.XmDeserializelException, ex.Message);
            }
        }

        private static Uri? RelocationPathToUri(string referenceRelocationPath)
            => referenceRelocationPath == null ? null : PathUtils.GetAbsoluteUri(referenceRelocationPath);

        /// <summary>Retrieves the worker serializer for a specified type.</summary>
        /// <param name="type">The type.</param>
        public ContentTypeSerializer GetTypeSerializer(Type type)
        {
            ArgumentNullException.ThrowIfNull(type);

            if (!serializerInstances.TryGetValue(type, out ContentTypeSerializer? serializer))
            {
                serializer = typeSerializerFactory.CreateSerializer(type);
                AddTypeSerializer(serializer);
                serializer.Initialize(this);
            }
            return serializer;
        }

        private void AddTypeSerializer(ContentTypeSerializer serializer)
        {
            if (serializerInstances.TryGetValue(serializer.TargetType, out ContentTypeSerializer? value))
                throw new InvalidOperationException(string.Format(CultureInfo.CurrentCulture, SR.DuplicateTypeHandler, typeof(ContentTypeSerializer).Name, serializer.GetType().AssemblyQualifiedName, value.GetType().AssemblyQualifiedName, serializer.TargetType));

            if (serializer.XmlTypeName != null)
                typeNameHelper.AddXmlTypeName(serializer);

            serializerInstances.Add(serializer.TargetType, serializer);
        }

        internal CollectionHelper GetCollectionHelper(Type type)
        {
            ArgumentNullException.ThrowIfNull(type);

            if (!collectionHelpers.TryGetValue(type, out CollectionHelper? collectionHelper))
            {
                collectionHelper = new CollectionHelper(this, type);
                collectionHelpers[type] = collectionHelper;
            }

            return collectionHelper;
        }
    }
}
