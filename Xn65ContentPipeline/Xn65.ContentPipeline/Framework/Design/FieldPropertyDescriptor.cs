using System.Reflection;

namespace Xn65.Csharp.Framework.Design
{
    internal class FieldPropertyDescriptor : MemberPropertyDescriptor
    {
        private readonly FieldInfo _field;

        public override Type PropertyType
            => _field.FieldType;

        public FieldPropertyDescriptor(FieldInfo? field)
            : base(field)
        {
            ArgumentNullException.ThrowIfNull(field);

            _field = field;
        }

        public override object GetValue(object? component)
        {
            return _field.GetValue(component)!;
        }

        public override void SetValue(object? component, object? value)
        {
            _field.SetValue(component, value);
            OnValueChanged(component, EventArgs.Empty);
        }
    }
}
