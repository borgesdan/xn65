using System.Globalization;
using System.Reflection;
using System.Xml;

namespace Xn65.Csharp.ContentPipeline.Serialization.Intermediate
{
    internal class TypeNameHelper
    {
        private const string ArraySuffix = "[]";
        private const string NestedTypeSeparator = "+";
        private const string GenericArgumentStart = "[";
        private const string GenericArgumentSeparator = ",";
        private const string GenericArgumentClose = "]";
        private const string ClrGenericMarker = "`";
        private const char ClrNamespaceSeparator = '.';
        private const char XmlNamespaceSeparator = ':';
        private readonly char[] XmlNamespaceBeginnings = new char[2]
        {
      '[',
      ','
        };
        private Dictionary<Type, string> typeToXmlName = new Dictionary<Type, string>();
        private Dictionary<string, Type> xmlNameToType = new Dictionary<string, Type>();

        public void WriteTypeName(XmlWriter output, Type type)
        {
            if (type.IsByRef || type.IsPointer)
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.BadTypePointerOrReference, (object)type));
            if (type.ContainsGenericParameters)
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.BadTypeOpenGeneric, (object)type));
            string text;
            if (typeToXmlName.TryGetValue(type, out text))
                output.WriteString(text);
            else if (type.IsArray)
            {
                if (type.GetArrayRank() != 1)
                    throw new RankException(Resources.CantSerializeMultidimensionalArrays);
                WriteTypeName(output, type.GetElementType());
                output.WriteString("[]");
            }
            else
                WriteClrType(output, type);
        }

        private void WriteClrType(XmlWriter output, Type type)
        {
            string typeName = type.Name;
            int length = typeName.IndexOf("`");
            if (length >= 0)
                typeName = typeName.Substring(0, length);
            Type declaringType = GetDeclaringType(type);
            if (declaringType != null)
            {
                WriteClrType(output, declaringType);
                output.WriteString("+" + typeName);
            }
            else
                WriteTypeAndNamespace(output, typeName, type.Namespace);
            WriteGenericArguments(output, type, declaringType);
        }

        private static void WriteTypeAndNamespace(
          XmlWriter output,
          string typeName,
          string clrNamespace)
        {
            if (string.IsNullOrEmpty(clrNamespace))
            {
                output.WriteString(typeName);
            }
            else
            {
                string str = output.LookupPrefix(clrNamespace);
                if (str != null)
                {
                    output.WriteString(str + ':' + typeName);
                }
                else
                {
                    int length = clrNamespace.LastIndexOf('.');
                    typeName = clrNamespace.Substring(length + 1) + '.' + typeName;
                    if (length > 0)
                    {
                        clrNamespace = clrNamespace.Substring(0, length);
                        WriteTypeAndNamespace(output, typeName, clrNamespace);
                    }
                    else
                        output.WriteString(typeName);
                }
            }
        }

        private void WriteGenericArguments(XmlWriter output, Type type, Type declaringType)
        {
            if (!type.IsGenericType)
                return;
            Type[] genericArguments = type.GetGenericArguments();
            int num1 = 0;
            if (declaringType != null && declaringType.IsGenericType)
                num1 += declaringType.GetGenericArguments().Length;
            if (num1 >= genericArguments.Length)
                return;
            output.WriteString("[");
            XmlWriter output1 = output;
            Type[] typeArray = genericArguments;
            int index = num1;
            int num2 = index + 1;
            Type type1 = typeArray[index];
            WriteTypeName(output1, type1);
            for (; num2 < genericArguments.Length; WriteTypeName(output, genericArguments[num2++]))
                output.WriteString(",");
            output.WriteString("]");
        }

        private static Type GetDeclaringType(Type type)
        {
            Type declaringType = type.DeclaringType;
            if (declaringType == null)
                return null;
            if (!declaringType.IsGenericTypeDefinition)
                return declaringType;
            int length = declaringType.GetGenericArguments().Length;
            Type[] destinationArray = new Type[length];
            Array.Copy(type.GetGenericArguments(), destinationArray, length);
            return declaringType.MakeGenericType(destinationArray);
        }

        public Type ParseTypeName(XmlReader input, string typeName)
        {
            typeName = ExpandXmlNamespaces(input, typeName);
            Type typeName1;
            if (xmlNameToType.TryGetValue(typeName, out typeName1))
                return typeName1;
            Type typeName2;
            if (typeName.EndsWith("[]"))
            {
                string typeName3 = typeName.Substring(0, typeName.Length - "[]".Length);
                typeName2 = ParseTypeName(input, typeName3).MakeArrayType();
            }
            else
            {
                List<Type> genericArguments = new List<Type>();
                typeName2 = FindType(GetClrTypeName(input, typeName, genericArguments));
                if (genericArguments.Count > 0)
                    typeName2 = typeName2.MakeGenericType(genericArguments.ToArray());
            }
            xmlNameToType[typeName] = typeName2;
            return typeName2;
        }

        private string GetClrTypeName(XmlReader input, string typeName, List<Type> genericArguments)
        {
            int length = IndexOfTypeSeparator(typeName);
            if (length > 0)
            {
                string typeName1 = typeName.Substring(0, length);
                string typeName2 = typeName.Substring(length + "+".Length);
                typeName = GetClrTypeName(input, typeName1, genericArguments) + "+" + GetClrTypeName(input, typeName2, genericArguments);
            }
            else if (typeName.EndsWith("]"))
                typeName = ParseGenericArguments(input, typeName, genericArguments);
            return typeName;
        }

        private static int IndexOfTypeSeparator(string input)
        {
            int num = 0;
            for (int pos = 0; pos < input.Length; ++pos)
            {
                if (SubstringMatches(input, pos, "["))
                    ++num;
                else if (SubstringMatches(input, pos, "]"))
                    --num;
                else if (SubstringMatches(input, pos, "+") && num == 0)
                    return pos;
            }
            return -1;
        }

        private string ParseGenericArguments(
          XmlReader input,
          string typeName,
          List<Type> genericArguments)
        {
            int length1 = typeName.IndexOf("[");
            if (length1 <= 0)
                throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.BadTypeNameString, (object)typeName));
            int startIndex = length1 + "[".Length;
            int length2 = typeName.Length - startIndex - "]".Length;
            List<string> stringList = SplitGenericArguments(typeName.Substring(startIndex, length2));
            foreach (string typeName1 in stringList)
                genericArguments.Add(ParseTypeName(input, typeName1));
            typeName = typeName.Substring(0, length1);
            typeName = typeName + "`" + stringList.Count.ToString(CultureInfo.InvariantCulture);
            return typeName;
        }

        private static List<string> SplitGenericArguments(string input)
        {
            List<string> stringList = new List<string>();
            int num1 = 0;
            int num2 = 0;
            while (num2 < input.Length)
            {
                if (SubstringMatches(input, num2, ",") && num1 == 0)
                {
                    stringList.Add(input.Substring(0, num2));
                    input = input.Substring(num2 + ",".Length);
                    num2 = 0;
                }
                else
                {
                    if (SubstringMatches(input, num2, "["))
                        ++num1;
                    else if (SubstringMatches(input, num2, "]"))
                        --num1;
                    ++num2;
                }
            }
            if (!string.IsNullOrEmpty(input))
                stringList.Add(input);
            return stringList;
        }

        private static bool SubstringMatches(string input, int pos, string token)
        {
            if (token.Length == 1)
                return input[pos] == token[0];
            throw new NotSupportedException();
        }

        private string ExpandXmlNamespaces(XmlReader input, string typeName)
        {
            int startIndex1;
            string str1;
            for (int startIndex2 = 0; (startIndex1 = typeName.IndexOf(':', startIndex2)) > 0; startIndex2 = typeName.Length - str1.Length)
            {
                int num = typeName.LastIndexOfAny(XmlNamespaceBeginnings, startIndex1) + 1;
                string str2 = typeName.Substring(0, num);
                string prefix = typeName.Substring(num, startIndex1 - num);
                str1 = typeName.Substring(startIndex1 + 1);
                string str3 = input.LookupNamespace(prefix);
                typeName = str2 + str3 + '.' + str1;
            }
            return typeName;
        }

        private static Type FindType(string typeName)
        {
            Type type1 = Type.GetType(typeName);
            if (type1 != null)
                return type1;
            foreach (Assembly assembly in new AssemblyScanner(true))
            {
                Type type2 = assembly.GetType(typeName);
                if (type2 != null)
                    return type2;
            }
            throw new ArgumentException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.CantFindType, (object)typeName));
        }

        public void AddXmlTypeName(ContentTypeSerializer serializer)
        {
            Type targetType = serializer.TargetType;
            string xmlTypeName = serializer.XmlTypeName;
            if (xmlNameToType.ContainsKey(xmlTypeName))
                throw new InvalidOperationException(string.Format((IFormatProvider)CultureInfo.CurrentCulture, Resources.DuplicateXmlTypeName, (object)serializer.GetType().AssemblyQualifiedName, (object)xmlNameToType[xmlTypeName].AssemblyQualifiedName, (object)xmlTypeName));
            typeToXmlName.Add(targetType, xmlTypeName);
            xmlNameToType.Add(xmlTypeName, targetType);
        }
    }
}
