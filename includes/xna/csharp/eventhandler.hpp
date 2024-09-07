#ifndef XNA_CSHARP_EVENTHANDLER_HPP
#define XNA_CSHARP_EVENTHANDLER_HPP

#include <functional>
#include <vector>

namespace xna {
    struct EventArgs {
        virtual ~EventArgs() {
            sender = nullptr;                
        }

        void** sender = nullptr;
    };

    template <typename TEventArgs>
    struct EventHandler {

        template <typename _Ptr>
        using HANDLER_CALLBACK = void(_Ptr::*&)(TEventArgs& args);

        template <class _Ptr>
        void Add(HANDLER_CALLBACK<_Ptr> function, _Ptr* ptr) {
            using std::placeholders::_1;
            std::function<void(TEventArgs&)> func = std::bind(function, ptr, _1);
            funcs.push_back(func);
        }        

        void operator()(TEventArgs& args) {
            for (size_t i = 0; i < funcs.size(); ++i)
            {
                auto& func = funcs[i];

                if (func)
                    func(args);
            }
        }

    private:
        std::vector<std::function<void(TEventArgs&)>> funcs;
    };
}

#endif