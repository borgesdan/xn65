namespace Xn65.Csharp
{
    internal class SR
    {
        //
        // Pipeline
        //

        public static readonly string CantLoadPipelineAssembly =
            "Error loading pipeline assembly {0}";
        public static readonly string CantLoadPipelineAssemblyOffShare =
            "Error loading pipeline assembly {0}. Pipeline assemblies cannot be loaded from a network share.";
        public static readonly string AssemblyReferenceWrongVersion =
            "Assembly {0} was built with a reference to {1} with an incorrect version {2} when version {3} was expected.";
        public static readonly string TypeScannerError =
            "{0} is not valid on type {1}. The target should be a non-abstract class which derives from {2} and has a default public constructor.";
        public static readonly string BadGenericTypeHandler =
            "Intermediate {0} {1} is an invalid generic. It should be in the form My{0}<T> : {0}<MyType<T>>.";
        public static readonly string DuplicateTypeHandler =
            "Intermediate '{0}' '{1}' conflicts with existing handler '{2}' for '{3}'.";
        public static readonly string BadTypePointerOrReference =
            "Cannot serialize type '{0}'. Pointers and references are not supported.";
        public static readonly string BadTypeOpenGeneric =
            "Type '{0}' cannot be serialized because not all the generic type parameters have been specified.";
        public static readonly string BadTypeDelegate =
            "Cannot serialize type '{0'}. Delegates are not supported.";
        public static readonly string ElementNotFound =
            "XML element '{0}' not found.";
        public static readonly string NullElementNotAllowed =
            "XML element '{0}' is not allowed to be null.";
        public static readonly string WrongXmlType =
            "XML 'Type' attribute is invalid. Expecting a subclass of {0}, but XML contains {1}.";
        public static readonly string UnknownDeserializationType =
            "XML is missing a 'Type' attribute.";
        public static readonly string DeserializerReturnedNull =
            "Intermediate ContentTypeSerializer {0} (handling type {1}) returned a null value from its Deserialize method.";
        public static readonly string DeserializerConstructedNewInstance =
            "Intermediate ContentTypeSerializer {0} (handling type {1}) returned a new object instance from its Deserialize method. This should have loaded data into the existingInstance parameter.";
        public static readonly string WrongSharedResourceType =
            "XML specifies invalid type for shared resource. Expecting a subclass of '{0}', but XML contains '{1}'.";
        public static readonly string MissingAttribute =
            "XML attribute \"{0}\" was not found.";
        public static readonly string DuplicateID =
            "Duplicate XML ID attribute \"{0}\".";
        public static readonly string MissingResource =
            "Missing shared resource \"{0}\".";
        public static readonly string MissingExternalReference =
            "Missing external reference \"{0}\".";
        public static readonly string WrongExternalReferenceType =
            "XML specifies wrong type for external reference \"{0}\".";
        public static readonly string NullElementName =
            "ContentSerializerAttribute has a null ElementName property.";
        public static readonly string XmDeserializelException =
            "There was an error while deserializing intermediate XML. {0}";
        public static readonly string NotIntermediateXml =
            "XML is not in the XNA intermediate format. Missing XnaContent root element.";
        public static readonly string CantWriteNullInFlattenContentMode =
            "Cannot serialize null values when the ContentSerializerAttribute.FlattenContent flag is set.";
        public static readonly string CantWriteDynamicTypesInFlattenContentMode =
            "Cannot serialize null values when the ContentSerializerAttribute.FlattenContent flag is set.";
        public static readonly string FoundCyclicReference =
            "Cyclic reference found while serializing {0}. You may be missing a ContentSerializerAttribute.SharedResource flag.";
        public static readonly string NotEnoughEntriesInXmlList =
            "XML does not have enough entries in space-separated list.";
        public static readonly string TooManyEntriesInXmlList =
            "XML has too many entries in the space-separated list.";
        
        //
        // Framework
        //
        
        public static readonly string InvalidStringFormat =
            "Invalid string format.";
        public static readonly string NullNotAllowed =
            "Null not allowed.";
    }
}
