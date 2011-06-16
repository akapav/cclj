#ifndef _NDXSEQ_
#define _NDXSEQ_

#include <vector>
#include <memory>

#include "iseq.h"
#include "nil.h"

template <typename T, typename Seq>
struct NdxSeq : ISeq<T> {
private:
  typedef ISeq<T> seq_t;
  typedef typename seq_t::ref seq_ref;

public:
  NdxSeq(Seq seq, unsigned ndx) : seq_(seq), ndx_(ndx) {}

  virtual T first() const
  { return seq_->nth(ndx_); }

  virtual seq_ref rest() const {
    auto nndx = ndx_ + 1;
    if(nndx == seq_->size()) return nil<T>();
    return seq_ref(new NdxSeq(seq_, nndx));
  }

  virtual seq_ref conj(T el) const
  { return seq_ref(new NdxSeq(seq_->push(el), ndx_)); }

 private:
  Seq seq_;
  unsigned ndx_;
};

template <typename T>
struct DummyArr : std::enable_shared_from_this<DummyArr<T> > {
private:
  typedef ISeq<T> seq_t;
  typedef typename seq_t::ref seq_ref;

public:
  typedef std::shared_ptr<DummyArr<T> > ref;

  static ref make()
  { return ref(new DummyArr()); }

  ref push(T el) const {
    auto ret = ref(new DummyArr(v_));
    ret->v_.push_back(el);
    return ret;
  }

  ref set_nth(int ndx, T el) {
    auto ret = ref(new DummyArr(v_));
    ret->v_[ndx] = el;
    return ret;
  }

  int size() const
  { return v_.size(); }

  T nth(int ndx) const
  { return v_[ndx]; }
  
  // operator seq_ref() const
  // { return seq(); }

  seq_ref seq() const
  { return seq_ref(new NdxSeq<T, ref >(self(), 0)); }

private:
  ref self() const
  { return const_cast<DummyArr<T>*>(this)->shared_from_this(); }

  DummyArr() {}
  DummyArr(const std::vector<T>& v) : v_(v) {}
  std::vector<T> v_;
};

#endif //_NDXSEQ_
