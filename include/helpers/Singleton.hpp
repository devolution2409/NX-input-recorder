// https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <memory>
// context is needed for singleton(int) to call Context::init
// so init has to be static and must return a pointer to whatever the class you
// want to singletonize is
template <typename T, typename CONTEXT> class Singleton {

  public:
    // Very fast inlined operator accessors
    T *operator->() { return mpInstance; }
    const T *operator->() const { return mpInstance; }
    T &operator*() { return *mpInstance; }
    const T &operator*() const { return *mpInstance; }

    const T *GetAddress() { return std::addressof(*mpInstance); };

  protected:
    Singleton()
    {
        static bool static_init = []() -> bool {
            mpInstance = new T;
            return true;
        }();
        (void)static_init; // silence warning about unused var
    }
    // Call the Singleton constructor with an int for it to invoke your static
    // init() method.
    Singleton(int)
    {
        static bool static_init = []() -> bool {
            mpInstance = CONTEXT::init();
            return true;
        }();
        (void)static_init; // silence warning
    }

  private:
    static T *mpInstance;
};

template <typename T, typename CONTEXT> T *Singleton<T, CONTEXT>::mpInstance;

#endif // _SINGLETON_H