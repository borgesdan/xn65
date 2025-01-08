#ifndef CSHARP_IO_EXCEPTION_HPP
#define CSHARP_IO_EXCEPTION_HPP

#include "../exception.hpp"
#include <source_location>

namespace csharp {    
	class IOException : public SystemException {
    public:
        IOException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }

        IOException(OptionalString const& message, size_t hresult, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = hresult;
        }

        IOException(OptionalString const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(message.value_or(SR::Arg_IOException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }
	};

    class EndOfStreamException : public IOException {
    public:
        EndOfStreamException(OptionalString const& message = std::nullopt, std::source_location const& source = std::source_location::current())
            : IOException(message.value_or(SR::Arg_IOException), source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }        

        EndOfStreamException(OptionalString const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : IOException(message.value_or(SR::Arg_IOException), innerException, source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }
    };
}

#endif