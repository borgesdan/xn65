using System.Globalization;
using System.Resources;
using Xn65.Csharp.ContentPipeline;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    /// <summary>Provides methods for serializing and deserializing a specific managed type.</summary>
    public abstract class ContentTypeSerializer
    {
        readonly Type targetType;
        readonly string? xmlTypeName;

        /// <summary>Initializes a new instance of the ContentTypeSerializer class for serializing the specified type.</summary>
        /// <param name="targetType">The target type.</param>
        protected ContentTypeSerializer(Type targetType)
          : this(targetType, null)
        {
        }

        /// <summary>Initializes a new instance of the ContentTypeSerializer class for serializing the specified type using the specified XML shortcut name.</summary>
        /// <param name="targetType">The target type.</param>
        /// <param name="xmlTypeName">The XML shortcut name.</param>
        protected ContentTypeSerializer(Type targetType, string? xmlTypeName)
        {
            this.targetType = targetType ?? throw new ArgumentNullException(nameof(targetType));
            this.xmlTypeName = xmlTypeName;
        }

        /// <summary>Retrieves and caches any nested type serializers and allows reflection over the target data type.</summary>
        /// <param name="serializer">The content serializer.</param>
        protected internal virtual void Initialize(IntermediateSerializer serializer)
        {
        }

        /// <summary>Gets the type handled by this serializer component.</summary>
        public Type TargetType => targetType;

        internal virtual Type BoxedTargetType => targetType;

        /// <summary>Gets a short-form XML name for the target type, or null if there is none.</summary>
        public string? XmlTypeName => xmlTypeName;

        /// <summary>Serializes an object to intermediate XML format.</summary>
        /// <param name="output">Specifies the intermediate XML location, and provides various serialization helpers.</param>
        /// <param name="value">The object to be serialized.</param>
        /// <param name="format">Specifies the content format for this object.</param>
        protected internal abstract void Serialize(
          IntermediateWriter output,
          object value,
          ContentSerializerAttribute format);

        /// <summary>Deserializes an object from intermediate XML format.</summary>
        /// <param name="input">Location of the intermediate XML and various deserialization helpers.</param>
        /// <param name="format">Specifies the intermediate source XML format.</param>
        /// <param name="existingInstance">The object containing the received data, or null if the deserializer should construct a new instance.</param>
        protected internal abstract object Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          object? existingInstance);

        /// <summary>Gets a value indicating whether this component may load data into an existing object or if it must it construct a new instance of the object before loading the data.</summary>
        public virtual bool CanDeserializeIntoExistingObject => false;

        /// <summary>Queries whether an object contains data to be serialized.</summary>
        /// <param name="value">The object to query.</param>
        public virtual bool ObjectIsEmpty(object value) => false;

        internal bool IsTargetType(Type type) => targetType.IsAssignableFrom(type);

        internal bool IsTargetType(object value) => value != null && IsTargetType(value.GetType());

        /// <summary>Examines the children of the specified object, passing each to a callback delegate.</summary>
        /// <param name="serializer">The content serializer.</param>
        /// <param name="callback">The method to be called for each examined child.</param>
        /// <param name="value">The object whose children are being scanned.</param>
        protected internal virtual void ScanChildren(
          IntermediateSerializer serializer,
          ChildCallback callback,
          object value)
        {
        }

        /// <summary>Callback delegate for the ScanChildren method.</summary>
        /// <param name="typeSerializer">The serializer component used to read or write the child object.</param>
        /// <param name="value">The child object currently being scanned.</param>
        protected internal delegate void ChildCallback(
          ContentTypeSerializer typeSerializer,
          object value);
    }

    /// <summary>Provides a generic implementation of ContentTypeSerializer methods and properties for serializing and deserializing a specific managed type.</summary>
    public abstract class ContentTypeSerializer<T> : ContentTypeSerializer
    {
        /// <summary>Initializes a new instance of the ContentTypeSerializer class.</summary>
        protected ContentTypeSerializer()
          : base(typeof(T))
        {
        }

        /// <summary>Initializes a new instance of the ContentTypeSerializer class using the specified XML shortcut name.</summary>
        /// <param name="xmlTypeName">The XML shortcut name.</param>
        protected ContentTypeSerializer(string xmlTypeName)
          : base(typeof(T), xmlTypeName)
        {
        }

        /// <summary>Serializes an object to intermediate XML format.</summary>
        /// <param name="output">Specifies the intermediate XML location, and provides various serialization helpers.</param>
        /// <param name="value">The strongly typed object to be serialized.</param>
        /// <param name="format">Specifies the content format for this object.</param>
        protected internal abstract void Serialize(
          IntermediateWriter output,
          T value,
          ContentSerializerAttribute format);

        /// <summary>Serializes an object to intermediate XML format.</summary>
        /// <param name="output">Specifies the intermediate XML location, and provides various serialization helpers.</param>
        /// <param name="value">The object to be serialized.</param>
        /// <param name="format">Specifies the content format for this object.</param>
        protected internal override void Serialize(
          IntermediateWriter output,
          object value,
          ContentSerializerAttribute format)
        {
            Serialize(output, ContentTypeSerializer<T>.CastType(value), format);
        }

        /// <summary>Deserializes a strongly typed object from intermediate XML format.</summary>
        /// <param name="input">Location of the intermediate XML and various deserialization helpers.</param>
        /// <param name="format">Specifies the intermediate source XML format.</param>
        /// <param name="existingInstance">The strongly typed object containing the received data, or null if the deserializer should construct a new instance.</param>
        protected internal abstract T Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          T existingInstance);

        /// <summary>Deserializes an object from intermediate XML format.</summary>
        /// <param name="input">Location of the intermediate XML and various deserialization helpers.</param>
        /// <param name="format">Specifies the intermediate source XML format.</param>
        /// <param name="existingInstance">The object containing the received data, or null if the deserializer should construct a new instance.</param>
        protected internal override object Deserialize(
          IntermediateReader input,
          ContentSerializerAttribute format,
          object existingInstance)
        {
            T existingInstance1 = existingInstance != null ? ContentTypeSerializer<T>.CastType(existingInstance) : default;
            return Deserialize(input, format, existingInstance1);
        }

        /// <summary>Queries whether an object contains data to be serialized.</summary>
        /// <param name="value">The object to query.</param>
        public virtual bool ObjectIsEmpty(T value) => base.ObjectIsEmpty(value);

        /// <summary>Queries whether a strongly-typed object contains data to be serialized.</summary>
        /// <param name="value">The object to query.</param>
        public override bool ObjectIsEmpty(object value) => ObjectIsEmpty(ContentTypeSerializer<T>.CastType(value));

        /// <summary>Examines the children of the specified object, passing each to a callback delegate.</summary>
        /// <param name="serializer">The content serializer.</param>
        /// <param name="callback">The method to be called for each examined child.</param>
        /// <param name="value">The strongly typed object whose children are being scanned.</param>
        protected internal virtual void ScanChildren(
          IntermediateSerializer serializer,
          ChildCallback callback,
          T value)
        {
        }

        /// <summary>Examines the children of the specified object, passing each to a callback delegate.</summary>
        /// <param name="serializer">The content serializer.</param>
        /// <param name="callback">The method to be called for each examined child.</param>
        /// <param name="value">The object whose children are being scanned.</param>
        protected internal override void ScanChildren(
          IntermediateSerializer serializer,
          ChildCallback callback,
          object value)
        {
            ScanChildren(serializer, callback, ContentTypeSerializer<T>.CastType(value));
        }

        private static T CastType(object value)
        {
            if (value == null)
                throw new ArgumentNullException(nameof(value));
            return value is T obj ? obj : throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.WrongArgumentType, (object)typeof(T), (object)value.GetType()));
        }
    }
}
