using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using Xn65.Csharp.ContentPipeline.Serialization.Intermediate;

namespace Xn65.Csharp.ContentPipeline
{
    /// <summary>Provides properties that define opaque data for a game asset.</summary>
    [ContentSerializerCollectionItemName("Data")]
    public sealed class OpaqueDataDictionary : NamedValueDictionary<object>
    {
        private string contentAsXml;

        /// <summary>Gets the value of the specified key/value pair of the asset.</summary>
        /// <param name="key">The name of the key.</param>
        /// <param name="defaultValue">The value to return if the key cannot be found. This can be null for reference types, 0 for primitive types, and a zero-filled structure for structure types.</param>
        public T GetValue<T>(string key, T defaultValue)
        {
            object obj1;
            return TryGetValue(key, out obj1) && obj1 is T obj2 ? obj2 : defaultValue;
        }

        /// <summary />
        public string GetContentAsXml()
        {
            if (contentAsXml == null)
            {
                if (Count == 0)
                {
                    contentAsXml = string.Empty;
                }
                else
                {
                    StringBuilder output1 = new StringBuilder();
                    using (XmlWriter output2 = XmlWriter.Create(output1))
                        IntermediateSerializer.Serialize(output2, this, null);
                    contentAsXml = output1.ToString();
                }
            }
            return contentAsXml;
        }

        protected override void AddItem(string key, object value)
        {
            contentAsXml = null;
            base.AddItem(key, value);
        }

        protected override void ClearItems()
        {
            contentAsXml = null;
            base.ClearItems();
        }

        protected override bool RemoveItem(string key)
        {
            contentAsXml = null;
            return base.RemoveItem(key);
        }

        protected override void SetItem(string key, object value)
        {
            contentAsXml = null;
            base.SetItem(key, value);
        }

        protected internal override Type DefaultSerializerType => typeof(string);
    }
}
