﻿using System.Numerics;

namespace Xn65.Csharp.Framework.Graphics
{
    public interface IPackedVector
    {
        Vector4 ToVector4();

        void PackFromVector4(Vector4 vector);
    }

    public interface IPackedVector<TPacked> : IPackedVector
    {
        TPacked PackedValue { get; set; }
    }
}
