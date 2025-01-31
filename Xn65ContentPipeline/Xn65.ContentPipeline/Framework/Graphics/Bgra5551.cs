using System.Globalization;
using System.Numerics;

namespace Xn65.Csharp.Framework.Graphics
{
    public struct Bgra5551 : IPackedVector<ushort>, IPackedVector, IEquatable<Bgra5551>
    {
        private ushort packedValue;

        [CLSCompliant(false)]
        public ushort PackedValue
        {
            readonly get => packedValue;
            set => packedValue = value;
        }

        public Bgra5551(float x, float y, float z, float w)
        {
            packedValue = PackHelper(x, y, z, w);
        }

        public Bgra5551(Vector4 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
        }

        void IPackedVector.PackFromVector4(Vector4 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
        }

        private static ushort PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW)
        {
            uint num = PackUtils.PackUNorm(31f, vectorX) << 10;
            uint num2 = PackUtils.PackUNorm(31f, vectorY) << 5;
            uint num3 = PackUtils.PackUNorm(31f, vectorZ);
            uint num4 = PackUtils.PackUNorm(1f, vectorW) << 15;
            return (ushort)(num | num2 | num3 | num4);
        }

        public readonly Vector4 ToVector4()
        {
            Vector4 result = default;
            result.X = PackUtils.UnpackUNorm(31u, (uint)packedValue >> 10);
            result.Y = PackUtils.UnpackUNorm(31u, (uint)packedValue >> 5);
            result.Z = PackUtils.UnpackUNorm(31u, packedValue);
            result.W = PackUtils.UnpackUNorm(1u, (uint)packedValue >> 15);
            return result;
        }

        public override readonly string ToString()
        {
            return packedValue.ToString("X4", CultureInfo.InvariantCulture);
        }

        public override readonly int GetHashCode()
        {
            return packedValue.GetHashCode();
        }

        public override readonly bool Equals(object? obj)
        {
            if (obj is Bgra5551 bgra)
            {
                return Equals(bgra);
            }

            return false;
        }

        public readonly bool Equals(Bgra5551 other)
        {
            return packedValue.Equals(other.packedValue);
        }

        public static bool operator ==(Bgra5551 a, Bgra5551 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Bgra5551 a, Bgra5551 b)
        {
            return !a.Equals(b);
        }
    }
}
