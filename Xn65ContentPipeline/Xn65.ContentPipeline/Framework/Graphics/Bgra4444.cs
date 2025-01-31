using System.Globalization;
using System.Numerics;

namespace Xn65.Csharp.Framework.Graphics
{
    public struct Bgra4444 : IPackedVector<ushort>, IPackedVector, IEquatable<Bgra4444>
    {
        private ushort packedValue;

        [CLSCompliant(false)]
        public ushort PackedValue
        {
            readonly get => packedValue;
            set => packedValue = value;
        }

        public Bgra4444(float x, float y, float z, float w)
        {
            packedValue = PackHelper(x, y, z, w);
        }

        public Bgra4444(Vector4 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
        }

        void IPackedVector.PackFromVector4(Vector4 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z, vector.W);
        }

        private static ushort PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW)
        {
            uint num = PackUtils.PackUNorm(15f, vectorX) << 8;
            uint num2 = PackUtils.PackUNorm(15f, vectorY) << 4;
            uint num3 = PackUtils.PackUNorm(15f, vectorZ);
            uint num4 = PackUtils.PackUNorm(15f, vectorW) << 12;
            return (ushort)(num | num2 | num3 | num4);
        }

        public readonly Vector4 ToVector4()
        {
            Vector4 result = default(Vector4);
            result.X = PackUtils.UnpackUNorm(15u, (uint)packedValue >> 8);
            result.Y = PackUtils.UnpackUNorm(15u, (uint)packedValue >> 4);
            result.Z = PackUtils.UnpackUNorm(15u, packedValue);
            result.W = PackUtils.UnpackUNorm(15u, (uint)packedValue >> 12);
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
            if (obj is Bgra4444 bgra)
            {
                return Equals(bgra);
            }

            return false;
        }

        public readonly bool Equals(Bgra4444 other)
        {
            return packedValue.Equals(other.packedValue);
        }

        public static bool operator ==(Bgra4444 a, Bgra4444 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Bgra4444 a, Bgra4444 b)
        {
            return !a.Equals(b);
        }
    }
}
