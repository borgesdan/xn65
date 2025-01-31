namespace Xn65.Csharp.ContentPipeline
{
    internal class ReferenceEqualityComparer<T> : IEqualityComparer<T>
    {
        public bool Equals(T? x, T? y)
            => ReferenceEquals(x, y);

        public int GetHashCode(T? obj)
            => obj?.GetHashCode() ?? 0;
    }
}
