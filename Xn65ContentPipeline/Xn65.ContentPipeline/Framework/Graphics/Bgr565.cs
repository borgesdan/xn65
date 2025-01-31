using System.Globalization;
using System.Numerics;

namespace Xn65.Csharp.Framework.Graphics
{
    public struct Bgr565 : IPackedVector<ushort>, IPackedVector, IEquatable<Bgr565>
    {
        private ushort packedValue;

        [CLSCompliant(false)]
        public ushort PackedValue
        {
            readonly get => packedValue;
            set => packedValue = value;
        }

        public Bgr565(float x, float y, float z)
        {
            packedValue = PackHelper(x, y, z);
        }

        public Bgr565(Vector3 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z);
        }

        void IPackedVector.PackFromVector4(Vector4 vector)
        {
            packedValue = PackHelper(vector.X, vector.Y, vector.Z);
        }

        private static ushort PackHelper(float vectorX, float vectorY, float vectorZ)
        {
            uint num = PackUtils.PackUNorm(31f, vectorX) << 11;
            uint num2 = PackUtils.PackUNorm(63f, vectorY) << 5;
            uint num3 = PackUtils.PackUNorm(31f, vectorZ);
            return (ushort)(num | num2 | num3);
        }

        public readonly Vector3 ToVector3()
        {
            Vector3 result = default;
            result.X = PackUtils.UnpackUNorm(31u, (uint)packedValue >> 11);
            result.Y = PackUtils.UnpackUNorm(63u, (uint)packedValue >> 5);
            result.Z = PackUtils.UnpackUNorm(31u, packedValue);
            return result;
        }

        readonly Vector4 IPackedVector.ToVector4()
        {
            Vector3 vector = ToVector3();
            return new Vector4(vector.X, vector.Y, vector.Z, 1f);
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
            if (obj is Bgr565 bgr)
            {
                return Equals(bgr);
            }

            return false;
        }

        public readonly bool Equals(Bgr565 other)
        {
            return packedValue.Equals(other.packedValue);
        }

        public static bool operator ==(Bgr565 a, Bgr565 b)
        {
            return a.Equals(b);
        }

        public static bool operator !=(Bgr565 a, Bgr565 b)
        {
            return !a.Equals(b);
        }
    }
}
