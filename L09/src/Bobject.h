#ifndef MY_BOBJECT_H
#define MY_BOBJECT_H

#include "Object.h"
#include "Program.h"

/**
 * Defines a bucket object, hence the name. The template 
 * ArgsPrime defines the parameters needed for the init virtual function which gets called on construction.
*/
template<typename... ArgsPrime>
class Bobject
{
protected:
    std::map< std::string, std::shared_ptr<Object> > objs;

    std::string resourceDir;
    std::shared_ptr<Program> prog;
public:

    Bobject(const std::string& resourceDir, std::shared_ptr<Program> prog) : 
        objs(std::map< std::string, std::shared_ptr<Object>>()),
        resourceDir(resourceDir), 
        prog(prog)
        {};
    ~Bobject() {};

    virtual void init(ArgsPrime... args) = 0; // allow for arguments to be passed into an object like this.

    // Method to call a specified method on all objects in the list
    template<typename T, typename... Args>
    inline void CallMethodOnAll(void(T::*method)(T&, Args...), Args... args) {
        for(std::map<std::string, std::shared_ptr<Object>>::iterator it = this->objs.begin(); it != this->objs.end(); ++it) {
			it->second->*method(args...);
		}
    }
};

#endif
