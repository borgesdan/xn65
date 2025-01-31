using System.Runtime.Serialization;
using System.Security.Permissions;

namespace Xn65.Csharp.ContentPipeline
{
    /// <summary>Thrown when errors are encountered in content during processing.</summary>
    [Serializable]
    public class InvalidContentException : Exception
    {
        /// <summary>Gets or sets the identity of the content item that caused the exception.</summary>
        public ContentIdentity? ContentIdentity { get; set; }

        /// <summary>Initializes a new instance of the InvalidContentException class</summary>
        public InvalidContentException()
        {
        }

        /// <summary>Initializes a new instance of the InvalidContentException class with the specified error message.</summary>
        /// <param name="message">A message that describes the error.</param>
        public InvalidContentException(string message)
          : base(message)
        {
        }

        /// <summary>Initializes a new instance of the InvalidContentException class with the specified error message and a reference to the inner exception that is the cause of this exception.</summary>
        /// <param name="message">A message that describes the error.</param>
        /// <param name="innerException">The exception that is the cause of the current exception. If innerException is not a null reference, the current exception is raised in a catch block that handles the inner exception.</param>
        public InvalidContentException(string message, Exception innerException)
          : base(message, innerException)
        {
        }

        /// <summary>Initializes a new instance of the InvalidContentException class with the specified error message and the identity of the content throwing the exception.</summary>
        /// <param name="message">A message that describes the error.</param>
        /// <param name="contentIdentity">Information about the content item that caused this error, including the file name. In some cases, a location within the file (of the problem) is specified.</param>
        public InvalidContentException(string message, ContentIdentity contentIdentity)
          : base(message)
        {
            ContentIdentity = contentIdentity;
        }

        /// <summary>Initializes a new instance of the InvalidContentException class with the specified error message, the identity of the content throwing the exception, and a reference to the inner exception that is the cause of this exception.</summary>
        /// <param name="message">A message that describes the error.</param>
        /// <param name="contentIdentity">Information about the content item that caused this error, including the file name. In some cases, a location within the file (of the problem) is specified.</param>
        /// <param name="innerException">The exception that is the cause of the current exception. If innerException is not a null reference, the current exception is raised in a catch block that handles the inner exception.</param>
        public InvalidContentException(string message, ContentIdentity contentIdentity, Exception? innerException)
            : base(message, innerException)
        {
            ContentIdentity = contentIdentity;
        }
    }
}
