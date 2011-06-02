#ifndef _ISEQ_
#define _ISEQ_

#include <memory>

template <typename T>
struct ISeq {
  typedef T element_type;
  typedef std::shared_ptr<ISeq<T> > ref;

  static ref nil()
  { return ref(static_cast<ISeq<T>*>(0)); }

  virtual T first() const = 0; 
  virtual ref rest() const = 0;
  virtual ref conj(T) const = 0;
  virtual ~ISeq() {}
};

template <typename T>
typename ISeq<T>::ref nil() { return ISeq<T>::nil(); }

#endif //_ISEQ_
