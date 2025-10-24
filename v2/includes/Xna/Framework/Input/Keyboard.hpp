#ifndef XNA_INPUT_KEYBOARD_HPP
#define XNA_INPUT_KEYBOARD_HPP

#include <memory>
#include "KeyboardState.hpp"

namespace Xna {
    //Allows retrieval of keystrokes from a keyboard input device. 
    class Keyboard {
    public:
        //Returns the current keyboard. 
        static KeyboardState GetState();

    private:
        friend class Game;
        static void Initialize();

        Keyboard() = default;
        Keyboard(Keyboard&) = default;
        Keyboard(Keyboard&&) = default;

    private:
        friend class GameWindow;

        struct Implementation;
        static std::unique_ptr<Implementation> implementation;                
    };
}

#endif