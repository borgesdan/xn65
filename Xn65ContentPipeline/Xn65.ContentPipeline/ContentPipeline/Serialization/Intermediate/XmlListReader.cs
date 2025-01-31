namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class XmlListReader
    {
        private static char[] listSeparators =
        [
            ' ',
            '\t',
            '\r',
            '\n'
        ];

        private readonly IntermediateReader reader;
        private readonly IEnumerator<string> enumerator;
        private bool atEnd;

        public XmlListReader(IntermediateReader reader)
        {
            this.reader = reader ?? throw new ArgumentNullException(nameof(reader));
            enumerator = ((IEnumerable<string>)reader.Xml.ReadContentAsString()
                .Split(listSeparators, StringSplitOptions.RemoveEmptyEntries))
                .GetEnumerator();

            atEnd = !enumerator.MoveNext();
        }

        public string ReadString()
        {
            if (atEnd)
                throw reader.CreateInvalidContentException(SR.NotEnoughEntriesInXmlList);

            var current = enumerator.Current;
            atEnd = !enumerator.MoveNext();
            return current;
        }

        public bool AtEnd
            => atEnd;
    }
}
