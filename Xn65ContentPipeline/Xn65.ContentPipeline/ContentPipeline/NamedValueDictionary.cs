using System.Collections;

namespace Xn65.Csharp.ContentPipeline
{
    /// <summary>Base class for dictionaries that map string identifiers to data values.</summary>
    public class NamedValueDictionary<T> :
      IDictionary<string, T>,
      ICollection<KeyValuePair<string, T>>,
      IEnumerable<KeyValuePair<string, T>>,
      IEnumerable
    {
        private Dictionary<string, T> items = new Dictionary<string, T>();

        /// <summary>Adds an element to the dictionary.</summary>
        /// <param name="key">Identity of the key of the new element.</param>
        /// <param name="value">The value of the new element.</param>
        protected virtual void AddItem(string key, T value)
        {
            if (string.IsNullOrEmpty(key))
                throw new ArgumentNullException(nameof(key));
            if (value == null)
                throw new ArgumentNullException(nameof(value));
            items.Add(key, value);
        }

        /// <summary>Removes all elements from the dictionary.</summary>
        protected virtual void ClearItems() => items.Clear();

        /// <summary>Removes the specified element from the dictionary.</summary>
        /// <param name="key">Identity of the key of the data pair to be removed.</param>
        protected virtual bool RemoveItem(string key) => !string.IsNullOrEmpty(key) ? items.Remove(key) : throw new ArgumentNullException(nameof(key));

        /// <summary>Modifies the value of an existing element.</summary>
        /// <param name="key">Identity of the element to be modified.</param>
        /// <param name="value">The value to be set.</param>
        protected virtual void SetItem(string key, T value)
        {
            if (string.IsNullOrEmpty(key))
                throw new ArgumentNullException(nameof(key));
            items[key] = value != null ? value : throw new ArgumentNullException(nameof(value));
        }

        /// <summary />
        protected internal virtual Type DefaultSerializerType => typeof(T);

        /// <summary>Adds the specified key and value to the dictionary.</summary>
        /// <param name="key">Identity of the key of the new data pair.</param>
        /// <param name="value">The value of the new data pair.</param>
        public void Add(string key, T value) => AddItem(key, value);

        /// <summary>Removes all keys and values from the dictionary.</summary>
        public void Clear() => ClearItems();

        /// <summary>Determines whether the specified key is present in the dictionary.</summary>
        /// <param name="key">Identity of a key.</param>
        public bool ContainsKey(string key) => items.ContainsKey(key);

        /// <summary>Gets the number of items in the dictionary.</summary>
        public int Count => items.Count;

        /// <summary>Removes the specified key and value from the dictionary.</summary>
        /// <param name="key">Identity of the key to be removed.</param>
        public bool Remove(string key) => RemoveItem(key);

        /// <summary>Gets the value associated with the specified key.</summary>
        /// <param name="key">Identity of the key of the element whose value is to be retrieved.</param>
        /// <param name="value">[OutAttribute] The current value of the element.</param>
        public bool TryGetValue(string key, out T value) => items.TryGetValue(key, out value);

        /// <summary>Gets all keys contained in the dictionary.</summary>
        public ICollection<string> Keys => items.Keys;

        /// <summary>Gets all values contained in the dictionary.</summary>
        public ICollection<T> Values => items.Values;

        /// <summary>Gets or sets the specified item.</summary>
        /// <param name="key" />
        public T this[string key]
        {
            get => items[key];
            set => SetItem(key, value);
        }

        /// <summary>Gets an enumerator that iterates through items in a dictionary.</summary>
        public IEnumerator<KeyValuePair<string, T>> GetEnumerator() => items.GetEnumerator();

        /// <summary>Returns an enumerator that can iterate through this collection.</summary>
        IEnumerator IEnumerable.GetEnumerator() => items.GetEnumerator();

        void ICollection<KeyValuePair<string, T>>.Add(KeyValuePair<string, T> item) => AddItem(item.Key, item.Value);

        bool ICollection<KeyValuePair<string, T>>.Contains(KeyValuePair<string, T> item) => ((ICollection<KeyValuePair<string, T>>)items).Contains(item);

        void ICollection<KeyValuePair<string, T>>.CopyTo(
          KeyValuePair<string, T>[] array,
          int arrayIndex)
        {
            ((ICollection<KeyValuePair<string, T>>)items).CopyTo(array, arrayIndex);
        }

        bool ICollection<KeyValuePair<string, T>>.IsReadOnly => false;

        bool ICollection<KeyValuePair<string, T>>.Remove(KeyValuePair<string, T> item) => ((ICollection<KeyValuePair<string, T>>)items).Contains(item) && RemoveItem(item.Key);
    }
}
