#ifndef XNA_PLATFORMINIT_HPP
#define XNA_PLATFORMINIT_HPP

namespace xna {
	//Exposes functions that must be implemented by the platform
	struct Platform {
		//Initialization function, which must be implemented by the platform,
		//and be called before the game is executed
		static void Init();
	};
}

#endif