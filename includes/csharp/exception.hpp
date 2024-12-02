#ifndef CSHARP_EXCEPTION_HPP
#define CSHARP_EXCEPTION_HPP

#include <stdexcept>
#include <string>
#include <source_location>
#include <memory>
#include "sr.hpp"

namespace csharp {
	struct HResults {
        inline static constexpr size_t S_OK = 0x00000000;
        inline static constexpr size_t S_FALSE = 0x1;
        inline static constexpr size_t COR_E_ABANDONEDMUTEX = 0x8013152D;
        inline static constexpr size_t COR_E_AMBIGUOUSIMPLEMENTATION = 0x8013106A;
        inline static constexpr size_t COR_E_AMBIGUOUSMATCH = 0x8000211D;
        inline static constexpr size_t COR_E_APPDOMAINUNLOADED = 0x80131014;
        inline static constexpr size_t COR_E_APPLICATION = 0x80131600;
        inline static constexpr size_t COR_E_ARGUMENT = 0x80070057;
        inline static constexpr size_t COR_E_ARGUMENTOUTOFRANGE = 0x80131502;
        inline static constexpr size_t COR_E_ARITHMETIC = 0x80070216;
        inline static constexpr size_t COR_E_ARRAYTYPEMISMATCH = 0x80131503;
        inline static constexpr size_t COR_E_BADEXEFORMAT = 0x800700C1;
        inline static constexpr size_t COR_E_BADIMAGEFORMAT = 0x8007000B;
        inline static constexpr size_t COR_E_CANNOTUNLOADAPPDOMAIN = 0x80131015;
        inline static constexpr size_t COR_E_CODECONTRACTFAILED = 0x80131542;
        inline static constexpr size_t COR_E_CONTEXTMARSHAL = 0x80131504;
        inline static constexpr size_t COR_E_CUSTOMATTRIBUTEFORMAT = 0x80131605;
        inline static constexpr size_t COR_E_DATAMISALIGNED = 0x80131541;
        inline static constexpr size_t COR_E_DIRECTORYNOTFOUND = 0x80070003;
        inline static constexpr size_t COR_E_DIVIDEBYZERO = 0x80020012;
        inline static constexpr size_t COR_E_DLLNOTFOUND = 0x80131524;
        inline static constexpr size_t COR_E_DUPLICATEWAITOBJECT = 0x80131529;
        inline static constexpr size_t COR_E_ENDOFSTREAM = 0x80070026;
        inline static constexpr size_t COR_E_ENTRYPOINTNOTFOUND = 0x80131523;
        inline static constexpr size_t COR_E_EXCEPTION = 0x80131500;
        inline static constexpr size_t COR_E_EXECUTIONENGINE = 0x80131506;
        inline static constexpr size_t COR_E_FAILFAST = 0x80131623;
        inline static constexpr size_t COR_E_FIELDACCESS = 0x80131507;
        inline static constexpr size_t COR_E_FILELOAD = 0x80131621;
        inline static constexpr size_t COR_E_FILENOTFOUND = 0x80070002;
        inline static constexpr size_t COR_E_FORMAT = 0x80131537;
        inline static constexpr size_t COR_E_INDEXOUTOFRANGE = 0x80131508;
        inline static constexpr size_t COR_E_INSUFFICIENTEXECUTIONSTACK = 0x80131578;
        inline static constexpr size_t COR_E_INSUFFICIENTMEMORY = 0x8013153D;
        inline static constexpr size_t COR_E_INVALIDCAST = 0x80004002;
        inline static constexpr size_t COR_E_INVALIDCOMOBJECT = 0x80131527;
        inline static constexpr size_t COR_E_INVALIDFILTERCRITERIA = 0x80131601;
        inline static constexpr size_t COR_E_INVALIDOLEVARIANTTYPE = 0x80131531;
        inline static constexpr size_t COR_E_INVALIDOPERATION = 0x80131509;
        inline static constexpr size_t COR_E_INVALIDPROGRAM = 0x8013153A;
        inline static constexpr size_t COR_E_IO = 0x80131620;
        inline static constexpr size_t COR_E_KEYNOTFOUND = 0x80131577;
        inline static constexpr size_t COR_E_MARSHALDIRECTIVE = 0x80131535;
        inline static constexpr size_t COR_E_MEMBERACCESS = 0x8013151A;
        inline static constexpr size_t COR_E_METHODACCESS = 0x80131510;
        inline static constexpr size_t COR_E_MISSINGFIELD = 0x80131511;
        inline static constexpr size_t COR_E_MISSINGMANIFESTRESOURCE = 0x80131532;
        inline static constexpr size_t COR_E_MISSINGMEMBER = 0x80131512;
        inline static constexpr size_t COR_E_MISSINGMETHOD = 0x80131513;
        inline static constexpr size_t COR_E_MISSINGSATELLITEASSEMBLY = 0x80131536;
        inline static constexpr size_t COR_E_MULTICASTNOTSUPPORTED = 0x80131514;
        inline static constexpr size_t COR_E_NOTFINITENUMBER = 0x80131528;
        inline static constexpr size_t COR_E_NOTSUPPORTED = 0x80131515;
        inline static constexpr size_t COR_E_OBJECTDISPOSED = 0x80131622;
        inline static constexpr size_t COR_E_OPERATIONCANCELED = 0x8013153B;
        inline static constexpr size_t COR_E_OUTOFMEMORY = 0x8007000E;
        inline static constexpr size_t COR_E_OVERFLOW = 0x80131516;
        inline static constexpr size_t COR_E_PATHTOOLONG = 0x800700CE;
        inline static constexpr size_t COR_E_PLATFORMNOTSUPPORTED = 0x80131539;
        inline static constexpr size_t COR_E_RANK = 0x80131517;
        inline static constexpr size_t COR_E_REFLECTIONTYPELOAD = 0x80131602;
        inline static constexpr size_t COR_E_RUNTIMEWRAPPED = 0x8013153E;
        inline static constexpr size_t COR_E_SAFEARRAYRANKMISMATCH = 0x80131538;
        inline static constexpr size_t COR_E_SAFEARRAYTYPEMISMATCH = 0x80131533;
        inline static constexpr size_t COR_E_SECURITY = 0x8013150A;
        inline static constexpr size_t COR_E_SERIALIZATION = 0x8013150C;
        inline static constexpr size_t COR_E_STACKOVERFLOW = 0x800703E9;
        inline static constexpr size_t COR_E_SYNCHRONIZATIONLOCK = 0x80131518;
        inline static constexpr size_t COR_E_SYSTEM = 0x80131501;
        inline static constexpr size_t COR_E_TARGET = 0x80131603;
        inline static constexpr size_t COR_E_TARGETINVOCATION = 0x80131604;
        inline static constexpr size_t COR_E_TARGETPARAMCOUNT = 0x8002000E;
        inline static constexpr size_t COR_E_THREADABORTED = 0x80131530;
        inline static constexpr size_t COR_E_THREADINTERRUPTED = 0x80131519;
        inline static constexpr size_t COR_E_THREADSTART = 0x80131525;
        inline static constexpr size_t COR_E_THREADSTATE = 0x80131520;
        inline static constexpr size_t COR_E_TIMEOUT = 0x80131505;
        inline static constexpr size_t COR_E_TYPEACCESS = 0x80131543;
        inline static constexpr size_t COR_E_TYPEINITIALIZATION = 0x80131534;
        inline static constexpr size_t COR_E_TYPELOAD = 0x80131522;
        inline static constexpr size_t COR_E_TYPEUNLOADED = 0x80131013;
        inline static constexpr size_t COR_E_UNAUTHORIZEDACCESS = 0x80070005;
        inline static constexpr size_t COR_E_VERIFICATION = 0x8013150D;
        inline static constexpr size_t COR_E_WAITHANDLECANNOTBEOPENED = 0x8013152C;
        inline static constexpr size_t CO_E_NOTINITIALIZED = 0x800401F0;
        inline static constexpr size_t DISP_E_PARAMNOTFOUND = 0x80020004;
        inline static constexpr size_t DISP_E_TYPEMISMATCH = 0x80020005;
        inline static constexpr size_t DISP_E_BADVARTYPE = 0x80020008;
        inline static constexpr size_t DISP_E_OVERFLOW = 0x8002000A;
        inline static constexpr size_t DISP_E_DIVBYZERO = 0x80020012;
        inline static constexpr size_t E_BOUNDS = 0x8000000B;
        inline static constexpr size_t E_CHANGED_STATE = 0x8000000C;
        inline static constexpr size_t E_FILENOTFOUND = 0x80070002;
        inline static constexpr size_t E_FAIL = 0x80004005;
        inline static constexpr size_t E_HANDLE = 0x80070006;
        inline static constexpr size_t E_INVALIDARG = 0x80070057;
        inline static constexpr size_t E_NOTIMPL = 0x80004001;
        inline static constexpr size_t E_POINTER = 0x80004003;
        inline static constexpr size_t ERROR_MRM_MAP_NOT_FOUND = 0x80073B1F;
        inline static constexpr size_t ERROR_TIMEOUT = 0x800705B4;
        inline static constexpr size_t RO_E_CLOSED = 0x80000013;
        inline static constexpr size_t RPC_E_CHANGED_MODE = 0x80010106;
        inline static constexpr size_t TYPE_E_TYPEMISMATCH = 0x80028CA0;
        inline static constexpr size_t STG_E_PATHNOTFOUND = 0x80030003;
        inline static constexpr size_t CTL_E_PATHNOTFOUND = 0x800A004C;
        inline static constexpr size_t CTL_E_FILENOTFOUND = 0x800A0035;
        inline static constexpr size_t FUSION_E_INVALID_NAME = 0x80131047;
        inline static constexpr size_t FUSION_E_REF_DEF_MISMATCH = 0x80131040;
        inline static constexpr size_t ERROR_TOO_MANY_OPEN_FILES = 0x80070004;
        inline static constexpr size_t ERROR_SHARING_VIOLATION = 0x80070020;
        inline static constexpr size_t ERROR_LOCK_VIOLATION = 0x80070021;
        inline static constexpr size_t ERROR_OPEN_FAILED = 0x8007006E;
        inline static constexpr size_t ERROR_DISK_CORRUPT = 0x80070571;
        inline static constexpr size_t ERROR_UNRECOGNIZED_VOLUME = 0x800703ED;
        inline static constexpr size_t ERROR_DLL_INIT_FAILED = 0x8007045A;
        inline static constexpr size_t MSEE_E_ASSEMBLYLOADINPROGRESS = 0x80131016;
        inline static constexpr size_t ERROR_FILE_INVALID = 0x800703EE;
	};

	class Exception : public std::runtime_error {
	public:
        Exception(std::source_location const& source = std::source_location::current()) 
            : Source(source), std::runtime_error("") {};
		Exception(std::string const& message, std::source_location source = std::source_location::current())
            : Source(source), message(message), std::runtime_error(message) {}
		Exception(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
			: Source(source), message(message), innerException(innerException), std::runtime_error(message) {}

        constexpr virtual std::string Message() const { return message; }
		const std::shared_ptr<Exception>& InnerException() const { return innerException; }

        // Retrieves the lowest exception (inner most) for the given Exception.
        // This will traverse exceptions using the innerException property.
        virtual Exception& GetBaseException();

	public:
		std::string HelpLink;
        std::source_location Source;
		size_t HRresult{ HResults::COR_E_EXCEPTION };

	private:
		std::string message;
		std::shared_ptr<Exception> innerException;
	};    

    //The exception that is thrown when one of the arguments provided to a method is not valid.
    class ArgumentException : public Exception {
    public:
        ArgumentException(std::source_location const& source = std::source_location::current()): Exception(SR::Arg_ArgumentException, source)
        {
            HRresult = HResults::COR_E_ARGUMENT;
        }
        ArgumentException(std::string const& message, std::source_location const& source = std::source_location::current())
            : Exception(!message.empty() ? message : SR::Arg_ArgumentException, source)
        {
            HRresult = HResults::COR_E_ARGUMENT;
        };

        ArgumentException(std::string const& message, std::string const& paramName, std::source_location const& source = std::source_location::current())
            : paramName(paramName), Exception(!message.empty() ? message : SR::Arg_ArgumentException, source)
        {
            HRresult = HResults::COR_E_ARGUMENT;
        };
        ArgumentException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : Exception(!message.empty() ? message : SR::Arg_ArgumentException, innerException, source)
        {
            HRresult = HResults::COR_E_ARGUMENT;
        }
        ArgumentException(std::string const& message, std::string const& paramName, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : paramName(paramName), Exception(!message.empty() ? message : SR::Arg_ArgumentException, innerException, source)
        {
            HRresult = HResults::COR_E_ARGUMENT;
        }

        constexpr std::string Message() const override {
            return Exception::Message().append(" Parameter: " + paramName + ".");            
        }

    private:
        std::string paramName;        
    };

    //The exception that is thrown when one of the arguments provided to a method is null.
    class ArgumentNullException : public ArgumentException {
    public:
        ArgumentNullException(std::source_location const& source = std::source_location::current()) : ArgumentException(SR::ArgumentNull_Generic, source)
        {
            HRresult = HResults::E_POINTER;
        }
        ArgumentNullException(std::string const& message, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::ArgumentNull_Generic, source)
        {
            HRresult = HResults::E_POINTER;
        };

        ArgumentNullException(std::string const& message, std::string const& paramName, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::ArgumentNull_Generic, paramName, source)
        {
            HRresult = HResults::E_POINTER;
        };
        ArgumentNullException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::ArgumentNull_Generic, innerException, source)
        {
            HRresult = HResults::E_POINTER;
        }
        ArgumentNullException(std::string const& message, std::string const& paramName, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::ArgumentNull_Generic, paramName, innerException, source)
        {
            HRresult = HResults::E_POINTER;
        }
    };

    //The exception that is thrown when the value of an argument is outside the allowable range of values as defined by the invoked method.
    class ArgumentOutOfRangeException : public ArgumentException {
    public:
        ArgumentOutOfRangeException(std::source_location const& source = std::source_location::current()) : ArgumentException(SR::Arg_ArgumentOutOfRangeException, source)
        {
            HRresult = HResults::E_POINTER;
        }
        ArgumentOutOfRangeException(std::string const& message, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::Arg_ArgumentOutOfRangeException, source)
        {
            HRresult = HResults::E_POINTER;
        };

        ArgumentOutOfRangeException(std::string const& message, std::string const& paramName, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::Arg_ArgumentOutOfRangeException, paramName, source)
        {
            HRresult = HResults::E_POINTER;
        };
        ArgumentOutOfRangeException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::Arg_ArgumentOutOfRangeException, innerException, source)
        {
            HRresult = HResults::E_POINTER;
        }
        ArgumentOutOfRangeException(std::string const& message, std::string const& paramName, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : ArgumentException(!message.empty() ? message : SR::Arg_ArgumentOutOfRangeException, paramName, innerException, source)
        {
            HRresult = HResults::E_POINTER;
        }        
    };

    class SystemException : public Exception {
    public:
        SystemException(std::source_location const& source = std::source_location::current()) : Exception(SR::Arg_SystemException, source)
        {
            HRresult = HResults::COR_E_SYSTEM;
        }

        SystemException(std::string const& message, std::source_location const& source = std::source_location::current())
            : Exception(!message.empty() ? message : SR::Arg_SystemException, source)
        {
            HRresult = HResults::COR_E_SYSTEM;
        }

        SystemException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : Exception(!message.empty() ? message : SR::Arg_ArgumentException, innerException, source)
        {
            HRresult = HResults::COR_E_SYSTEM;
        }
    };

    //The exception that is thrown when a method call is invalid for the object's current state.
    class InvalidOperationException : public SystemException {
    public:
        InvalidOperationException(std::source_location const& source = std::source_location::current())
            : SystemException(SR::Arg_SystemException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }

        InvalidOperationException(std::string const& message, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_InvalidOperationException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }

        InvalidOperationException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_InvalidOperationException, innerException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }
    };

    //The exception that is thrown when an invoked method is not supported,
    //typically because it should have been implemented on a subclass.
    class NotSupportedException : public SystemException {
    public:
        NotSupportedException(std::source_location const& source = std::source_location::current())
            : SystemException(SR::Arg_NotSupportedException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }

        NotSupportedException(std::string const& message, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_NotSupportedException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }

        NotSupportedException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_NotSupportedException, innerException, source)
        {
            HRresult = HResults::COR_E_INVALIDOPERATION;
        }
    };

    //The exception that is thrown when the operating system denies access because of an I/O error or a specific type of security error.
    class UnauthorizedAccessException : public SystemException {
    public:
        UnauthorizedAccessException(std::source_location const& source = std::source_location::current())
            : SystemException(SR::Arg_UnauthorizedAccessException, source)
        {
            HRresult = HResults::COR_E_UNAUTHORIZEDACCESS;
        }

        UnauthorizedAccessException(std::string const& message, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_UnauthorizedAccessException, source)
        {
            HRresult = HResults::COR_E_UNAUTHORIZEDACCESS;
        }

        UnauthorizedAccessException(std::string const& message, std::shared_ptr<Exception>& innerException, std::source_location const& source = std::source_location::current())
            : SystemException(!message.empty() ? message : SR::Arg_UnauthorizedAccessException, innerException, source)
        {
            HRresult = HResults::COR_E_UNAUTHORIZEDACCESS;
        }
    };
}

#endif