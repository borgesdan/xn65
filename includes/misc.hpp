#ifndef MISC_HPP
#define MISC_HPP

#include <memory>
#include <utility>

namespace misc {
	//Realiza um std::make_shared<TDERIVED> e converte para classe base com reinterpret_pointer_cast<TBASE>
	template <typename TBASE, typename TDERIVED, class... _Types>
	std::shared_ptr<TBASE> reinterpret_make_shared(_Types&&... _Args) {
		auto derived = std::make_shared<TDERIVED>(std::forward<_Types>(_Args)...);
		return reinterpret_pointer_cast<TBASE>(derived);
	}
}

#endif