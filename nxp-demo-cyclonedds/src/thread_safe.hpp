#ifndef __thread_safe_hpp__
#define __thread_safe_hpp__ 

#include <shared_mutex>
#include <utility>

template<class T>
struct thread_safe {
    template<class F>
    auto read( F&& f ) const {
        std::shared_lock<decltype(mutex)> lock(mutex);
        return std::forward<F>(f)(t);
    }
    template<class F>
    auto write( F&& f ) {
        std::unique_lock<decltype(mutex)> lock(mutex);
        return std::forward<F>(f)(t);
    }
    template<class O>
    thread_safe(O&&o):t(std::forward<O>(o)) {}

    thread_safe() = default;

    operator T() const {
        return t.read([](T const& t){return t;});
    }

    // it is really this simple:
    thread_safe( thread_safe const& o ):t( o ) {}

    // forward to above thread safe copy ctor:
    thread_safe( thread_safe & o ):thread_safe( const_cast<thread_safe const&>(o) ) {}
    thread_safe( thread_safe && o ):thread_safe(o) {}
    thread_safe( thread_safe const&& o ):thread_safe(o) {}

    thread_safe& operator=( thread_safe const& o ) {
        write( [&o](auto& target) {
                target = o;
                });
        return *this;
    }
    template<class O>
    thread_safe& operator=( O&& o ) {
        write([&o](auto& t){ t = std::forward<O>(o); });
        return *this;
    }
private:
    T t;
    mutable std::shared_timed_mutex mutex;
};

#endif //__thread_safe_hpp__ 
