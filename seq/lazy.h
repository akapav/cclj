#ifndef _LAZY_
#define _LAZY_

#include <memory>
#include <thread>

#include "iseq.h"

template <typename T>
struct Lazy : std::enable_shared_from_this<Lazy<T> >, ISeq<T> {
private:
  typedef ISeq<T> seq_t;
  typedef typename seq_t::ref seq_ref;
  typedef std::function<seq_ref()> fn;

public:
  typedef std::shared_ptr<Lazy<T> > ref;

  static ref make(const fn f)
  { return ref(new Lazy(f)); }

  seq_ref seq() const
  { return std::dynamic_pointer_cast<Lazy<T>, ISeq<T> >(self()); }

  T first() const {
    ensure_cached();
    return seq_->first();
  }

  seq_ref rest() const {
    ensure_cached();
    return seq_->rest();
  }

  seq_ref conj(T el) const { return cons(el, seq()); }
private:
  Lazy(fn f) : f_(f) {}

  ref self() const
  { return const_cast<Lazy<T>*>(this)->shared_from_this(); }

  void ensure_cached() const {
    std::unique_lock<std::mutex> lck(mtx_);
    if(seq_) return;
    seq_ = f_();
  }

  fn f_;
  mutable seq_ref seq_;
  mutable std::mutex mtx_;
};

template <typename Fn>
auto lazy_seq_fn(Fn fn) -> decltype(fn()) {
  typedef decltype(fn()) ret_type;
  typedef typename ret_type::element_type::element_type T;
  return Lazy<T>::make(fn);
}

#define lazy_seq(exp) \
  lazy_seq_fn([=]() { return exp; })
#endif //_LAZY_
