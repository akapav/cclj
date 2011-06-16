#ifndef _NIL_
#define _NIL_

#include <memory>
#include <thread>

#include "iseq.h"
#include "cons.h"

struct no_elements_exception {};

template <typename T>
struct Nil : std::enable_shared_from_this<Nil<T> >, ISeq<T> {
private:
  typedef ISeq<T> seq_t;
  typedef typename seq_t::ref seq_ref;

public:
  typedef std::shared_ptr<Nil<T> > ref;

  static ref make() { return ensure_cached(); }
  T first() const { throw no_elements_exception(); }
  seq_ref rest() const { return seq(); }
  seq_ref conj(T el) const { return cons(el, seq()); }
  bool nilp() const { return true; }

private:
  seq_ref seq() const
  { return std::dynamic_pointer_cast<Nil<T>, ISeq<T> >(self()); }

  ref self() const
  { return const_cast<Nil<T>*>(this)->shared_from_this(); }

  //
  static ref ensure_cached() {
    std::unique_lock<std::mutex> lck(mtx_);
    return this_ ? this_ : (this_ = ref(new Nil()));
  }

  static std::mutex mtx_;
  static ref this_;
};

template <typename T>
typename ISeq<T>::ref nil() { return Nil<T>::make(); }

template <typename T>
std::mutex Nil<T>::mtx_;

template <typename T>
typename Nil<T>::ref Nil<T>::this_ = nullptr;

#endif //_NIL_
