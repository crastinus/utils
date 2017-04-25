
#pragma once

#include <memory>
#include <utility>

template <typename Derived, typename NextPtr = std::unique_ptr<Derived> >
struct crtp_chain {
    // apply all functions into chain
    // chain a->b->c
    // a.f()->b.f()->c.f()
    template <typename Function, typename... Args>
    void apply(Derived* d, Function f, Args&&... args) {
        (*d.*f)(std::forward<Args>(args)...);
        if (d->next_)
            apply(d->next_.get(), f, std::forward<Args>(args)...);
    }

    // terminatable apply function
    // chain a->b->c
    // if (a.f() || b.f() || c.f())
    template <typename Function, typename... Args>
    bool apply_stop_if(Derived* d, Function f, Args&&... args) {
        if ((*d.*f)(std::forward<Args>(args)...))
            return true;
        return d->next_ &&
            apply_stop_if(d->next_.get(), f, std::forward<Args>(args)...);
    }

    // use flag to run function
    // chain a->b->c
    // 
    // if (a.flag()) 
    //   a.f() 
    // if (b.flag())
    //   .....
    //
    // flag is function because this is applied to multiple elements in chain
    template <typename Function, typename FlagFunction, typename... Args>
    void apply_by_flag(Derived* d, Function f, FlagFunction flag, Args&&... args) {
        if ((*d.*flag)())
            (*d.*f)(std::forward<Args>(args)...);
        if (d->next_)
            apply_by_flag(d->next_.get(), f, flag, std::forward<Args>(args)...);
    }


    // function of first object in chain will be run after all. last run first
    // chain a->b->c
    // c.f()->b.f()->a.f()
    template <typename Function, typename... Args>
    void apply_reverse(Derived* d, Function f, Args&&... args) {
        if (d->next_)
            apply_reverse(d->next_.get(), f, std::forward<Args>(args)...);
        (*d.*f)(std::forward<Args>(args)...);
    }

    // same as above for function-terminator
    // chain a->b->c
    // if (c.f() || b.f() || a.f())
    template <typename Function, typename... Args>
    bool apply_stop_if_reverse(Derived* d, Function f, Args&&... args) {
        if (d->next_ && apply_stop_if_reverse(d->next_.get(), f, std::forward<Args>(args)...))
            return true;
        return (*d.*f)(std::forward<Args>(args)...);
    }

    void next(NextPtr&& ptr = NextPtr()) { next_ = std::move(ptr); }

    void insert_back(NextPtr&& ptr) {
        if (next_)
            next_->insert_back(std::move(ptr));
        else
            next_ = std::move(ptr);
    }

    // node1
    //    node2
    //       nullptrnode <- this place
    NextPtr& first_after_back() {
        if (!next_)
            return next_;
        return next_->first_after_back();
    }

    NextPtr next_;
}; 
//template <typename Derived>
//using crtp_chain = chain_of_responsibility<
//    Derived,
//    std::unique_ptr<chain_of_responsibility<Derived> > >;
//
