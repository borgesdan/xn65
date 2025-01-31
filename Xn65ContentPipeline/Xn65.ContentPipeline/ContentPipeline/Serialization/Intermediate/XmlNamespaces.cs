namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class XmlNamespaces
    {
        private IntermediateWriter writer;
        private Dictionary<object, bool> seenObjects = new Dictionary<object, bool>(new ReferenceEqualityComparer<object>());
        private Dictionary<string, bool> seenNamespaces = new Dictionary<string, bool>();
        private List<string> namespaces = new List<string>();

        public XmlNamespaces(IntermediateWriter writer) => this.writer = writer;

        public void WriteNamespaces(object value)
        {
            ScanObject(null, value);
            Dictionary<string, bool> dictionary = new Dictionary<string, bool>();
            foreach (string str1 in namespaces)
            {
                int num = str1.LastIndexOf('.');
                string str2 = str1.Substring(num + 1);
                if (!(str2 == str1) && !dictionary.ContainsKey(str2))
                {
                    writer.Xml.WriteAttributeString("xmlns", str2, null, str1);
                    dictionary.Add(str2, true);
                }
            }
        }

        private void ScanObject(ContentTypeSerializer typeSerializer, object value)
        {
            if (value == null)
                return;
            Type type = value.GetType();
            if (!type.IsValueType)
            {
                if (seenObjects.ContainsKey(value))
                    return;
                seenObjects.Add(value, true);
            }
            if (typeSerializer == null || type != typeSerializer.BoxedTargetType)
            {
                RecordType(type);
                typeSerializer = writer.Serializer.GetTypeSerializer(type);
            }
            typeSerializer.ScanChildren(writer.Serializer, new ContentTypeSerializer.ChildCallback(ScanObject), value);
        }

        private void RecordType(Type type)
        {
            if (type.IsArray)
                type = type.GetElementType();
            string key = type.Namespace;
            if (string.IsNullOrEmpty(key))
                return;
            if (!seenNamespaces.ContainsKey(key))
            {
                seenNamespaces.Add(key, true);
                namespaces.Add(key);
            }
            if (!type.IsGenericType)
                return;
            foreach (Type genericArgument in type.GetGenericArguments())
                RecordType(genericArgument);
        }
    }
}
