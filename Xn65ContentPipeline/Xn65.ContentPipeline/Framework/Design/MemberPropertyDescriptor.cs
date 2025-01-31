using System.ComponentModel;
using System.Reflection;

namespace Xn65.Csharp.Framework.Design
{
    internal abstract class MemberPropertyDescriptor : PropertyDescriptor
    {
        private readonly MemberInfo _member;
        public override bool IsReadOnly => false;
        public override Type ComponentType 
            => _member.DeclaringType!;

        public MemberPropertyDescriptor(MemberInfo? member)
         : base(member?.Name ?? string.Empty, member?.GetCustomAttributes(typeof(Attribute), inherit: true) as Attribute[])
        {
            _member = member ?? throw new ArgumentNullException(nameof(member));
        }

        public override bool Equals(object? obj)
        {
            if (obj is MemberPropertyDescriptor memberPropertyDescriptor)
            {
                return memberPropertyDescriptor._member.Equals(_member);
            }

            return false;
        }

        public override int GetHashCode()
        {
            return _member.GetHashCode();
        }

        public override void ResetValue(object component)
        {
        }

        public override bool CanResetValue(object component)
        {
            return false;
        }

        public override bool ShouldSerializeValue(object component)
        {
            return true;
        }
    }
}
