#ifndef CSHARP_IO_EXCEPTION_HPP
#define CSHARP_IO_EXCEPTION_HPP

#include "../exception.hpp"
#include <source_location>

namespace csharp {    
	class IOException : public SystemException {
    public:
        IOException(std::source_location const& source = std::source_location::current())
            : SystemException(SR::Arg_IOException, source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }

        IOException(std::string const& message, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_IOException, source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }

        IOException(std::string const& message, size_t hresult, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_IOException, source)
        {
            HRresult = hresult;
        }

        IOException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_IOException, innerException, source)
        {
            HRresult = HResults::HR_COR_E_IO;
        }
	};

    class EndOfStreamException : public IOException {
    public:
        EndOfStreamException(std::source_location const& source = std::source_location::current())
            : IOException(SR::Arg_IOException, source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }

        EndOfStreamException(std::string const& message, std::source_location const& source = std::source_location::current())
            : IOException(!message.empty() ? message : SR::Arg_IOException, source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }        

        EndOfStreamException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : IOException(!message.empty() ? message : SR::Arg_IOException, innerException, source)
        {
            HRresult = HResults::HR_COR_E_ENDOFSTREAM;
        }
    };
}

#endif