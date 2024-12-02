#include "csharp/exception.hpp"

namespace csharp {
    Exception& Exception::GetBaseException() {
        auto inner = innerException.get();
        auto back = this;

        while (inner != nullptr) {
            back = inner;
            inner = inner->innerException.get();
        }

        return *back;
    }    
}


