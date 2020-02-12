
/*
 *  Represent a Lua virtual machine
 */


#ifndef __VM_H__
#define __VM_H__ 

#include <string>
#include <utils/singleton.h>

struct lua_State;

namespace app { namespace vm {

    struct WindowConfig {
        unsigned int Width = 1280u, Height = 720u;
        std::string Title = "Application Window";
    };

    class VM : public utils::Singleton<VM> {
        friend class utils::Singleton<VM>;
    private:
        VM();
        ~VM();
    public:
        void Run(const char * mainFile);
        void Update();
        void DoString(const char * str);
        WindowConfig ReadConfigFile(const char * path);
    private:
        lua_State * L, * S;
    };

} }

#endif //__VM_H__
