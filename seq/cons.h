#ifndef _CONS_
#define _CONS_

#include <memory>
#include "iseq.h"

template <typename T>
struct Cons : std::enable_shared_from_this<Cons<T> >, ISeq<T> {
private:
  typedef ISeq<T> seq_t;
  typedef typename seq_t::ref seq_ref;

public:
  typedef std::shared_ptr<Cons<T> > ref;

  static ref make(T car, seq_ref cdr)
  { return ref(new Cons(car, cdr)); }

  T first() const { return car_; }

  seq_ref rest() const { return cdr_; }

  seq_ref conj(T el) const { return cons(el, seq()); }
private:
  Cons(T car, seq_ref cdr): car_(car), cdr_(cdr) {}

  seq_ref seq() const
  { return std::dynamic_pointer_cast<Cons<T>, ISeq<T> >(self()); }

  ref self() const
  { return const_cast<Cons<T>*>(this)->shared_from_this(); }

  T car_;
  seq_ref cdr_;
};

template <typename T>
typename ISeq<T>::ref cons(T car, typename ISeq<T>::ref cdr)
{ return Cons<T>::make(car, cdr); }

template <typename T>
typename ISeq<T>::ref cons_nil(T car)
{ return cons(car, nil<T>()); }

#endif //_CONS_
