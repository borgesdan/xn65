using System.Xml;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class FakeEmptyElementReader : XmlReader
    {
        public static readonly FakeEmptyElementReader Instance = new FakeEmptyElementReader();

        public override int AttributeCount => 0;

        public override string BaseURI => null!;

        public override void Close()
        {
        }

        public override int Depth => 0;
        public override bool EOF => false;
        public override string GetAttribute(int i) => null!;
        public override string? GetAttribute(string name, string? namespaceURI) => null!;
        public override string GetAttribute(string name) => null!;
        public override bool HasValue => false;
        public override bool IsEmptyElement => true;
        public override string LocalName => null!;
        public override string LookupNamespace(string prefix) => null!;
        public override bool MoveToAttribute(string name, string? ns) => false;
        public override bool MoveToAttribute(string name) => false;
        public override bool MoveToElement() => false;
        public override bool MoveToFirstAttribute() => false;
        public override bool MoveToNextAttribute() => false;
        public override XmlNameTable NameTable => null!;
        public override string NamespaceURI => null!;
        public override XmlNodeType NodeType => XmlNodeType.Text;
        public override string Prefix => null!;
        public override bool Read() => false;
        public override bool ReadAttributeValue() => false;
        public override ReadState ReadState => ReadState.EndOfFile;

        public override void ResolveEntity()
        {
        }

        public override string Value => string.Empty;
    }
}
