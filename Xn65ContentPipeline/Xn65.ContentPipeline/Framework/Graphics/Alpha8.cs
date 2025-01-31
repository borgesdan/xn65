using System.Globalization;
using System.Numerics;

namespace Xn65.Csharp.Framework.Graphics
{
    public struct Alpha8(float alpha) : IPackedVector<byte>, IPackedVector, IEquatable<Alpha8>
    {
        private byte packedValue = PackHelper(alpha);

        public byte PackedValue
        {
            readonly get => packedValue;
            set => packedValue = value;
        }

        void IPackedVector.PackFromVector4(Vector4 vector)
        {
            packedValue = PackHelper(vector.W);
        }

        private static byte PackHelper(float alpha)
        {
            return (byte)PackUtils.PackUNorm(255f, alpha);
        }

        public readonly float ToAlpha()
        {
            return PackUtils.UnpackUNorm(255u, packedValue);
        }

        readonly Vector4 IPackedVector.ToVector4()
        {
            float w = ToAlpha();
            return new Vector4(0f, 0f, 0f, w);
        }

        public override readonly string ToString()
        {
            return packedValue.ToString("X2", CultureInfo.InvariantCulture);
        }

        public override readonly int GetHashCode()
        {
            return packedValue.GetHashCode();
        }

        public override readonly bool Equals(object? obj)
        {
            if (obj is Alpha8 alphaObj)
            {
                return Equals(alphaObj);
            }

            return false;
        }

        public readonly bool Equals(Alpha8 other)
        {
            return packedValue.Equals(other.packedValue);
        }

        public static bool operator ==(Alpha8 a, Alpha8 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Alpha8 a, Alpha8 b)
        {
            return !a.Equals(b);
        }
    }
}
